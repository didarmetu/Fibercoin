// Copyright (c) 2026 The Fibercoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING.

#include "hostedmasternode.h"

#include "masternode-helpers.h"
#include "masternodeconfig.h"
#include "masternodeman.h"
#include "util.h"

#include <exception>

CHostedMasternodeManager hostedMasternodes;

CHostedMasternode::CHostedMasternode()
    : lastHostedPing(0)
{
}

bool CHostedMasternode::Configure(
    const std::string& strAlias,
    const std::string& strService,
    const std::string& strKey,
    const std::string& strTxHash,
    const std::string& strOutputIndex,
    std::string& errorMessage)
{
    alias = strAlias;
    service = CService(strService);

    if (!service.IsValid()) {
        errorMessage = strprintf(
            "Invalid service address for hosted masternode %s: %s",
            alias,
            strService);
        return false;
    }

    if (!masternodeSigner.SetKey(
            strKey,
            errorMessage,
            keyMasternode,
            pubKeyMasternode)) {
        errorMessage = strprintf(
            "Invalid masternode key for %s: %s",
            alias,
            errorMessage);
        return false;
    }

    uint256 txHash = uint256S(strTxHash);

    if (txHash.IsNull()) {
        errorMessage = strprintf(
            "Invalid collateral transaction hash for hosted masternode %s",
            alias);
        return false;
    }

    int outputIndex;

    try {
        outputIndex = std::stoi(strOutputIndex);
    } catch (const std::exception& e) {
        errorMessage = strprintf(
            "Invalid collateral output index for hosted masternode %s: %s",
            alias,
            e.what());
        return false;
    }

    if (outputIndex < 0) {
        errorMessage = strprintf(
            "Invalid negative collateral output index for hosted masternode %s",
            alias);
        return false;
    }

    vin = CTxIn(txHash, static_cast<uint32_t>(outputIndex));

    lastError.clear();

    LogPrintf(
        "CHostedMasternode::Configure() - loaded %s, service=%s, vin=%s\n",
        alias,
        service.ToString(),
        vin.ToString());

    return true;
}

bool CHostedMasternode::SendPing(std::string& errorMessage)
{
    CMasternode* pmn = mnodeman.Find(vin);

    if (pmn == NULL) {
        errorMessage = strprintf(
            "Hosted masternode %s is not present in the masternode list",
            alias);
        lastError = errorMessage;
        return false;
    }

    // The masternode is now known to the network, so clear any
    // earlier "not present" diagnostic.
    lastError.clear();

    CMasternodePing mnp(vin);

    if (!mnp.Sign(keyMasternode, pubKeyMasternode)) {
        errorMessage = strprintf(
            "Could not sign masternode ping for hosted masternode %s",
            alias);
        lastError = errorMessage;
        return false;
    }

    if (pmn->IsPingedWithin(MASTERNODE_PING_SECONDS, mnp.sigTime)) {
        // This is normal and not an error. Another ping is not required yet.
        return true;
    }

    pmn->lastPing = mnp;
    mnodeman.mapSeenMasternodePing.insert(
        std::make_pair(mnp.GetHash(), mnp));

    CMasternodeBroadcast mnb(*pmn);
    uint256 hash = mnb.GetHash();

    if (mnodeman.mapSeenMasternodeBroadcast.count(hash)) {
        mnodeman.mapSeenMasternodeBroadcast[hash].lastPing = mnp;
    }

    mnp.Relay();

    lastHostedPing = mnp.sigTime;
    lastError.clear();

    LogPrintf(
        "CHostedMasternode::SendPing() - relayed ping for %s, vin=%s\n",
        alias,
        vin.ToString());

    return true;
}

bool CHostedMasternodeManager::LoadFromConfig(std::string& errorMessage)
{
    LOCK(cs);

    nodes.clear();

    std::vector<CMasternodeConfig::CMasternodeEntry>& entries =
        masternodeConfig.getEntries();

    BOOST_FOREACH (
        CMasternodeConfig::CMasternodeEntry& entry,
        entries) {

        if (entry.getAlias().empty()) {
            continue;
        }

        CHostedMasternode node;
        std::string entryError;

        if (!node.Configure(
                entry.getAlias(),
                entry.getIp(),
                entry.getPrivKey(),
                entry.getTxHash(),
                entry.getOutputIndex(),
                entryError)) {
            errorMessage = entryError;
            nodes.clear();
            return false;
        }

        nodes.push_back(node);
    }

    LogPrintf(
        "CHostedMasternodeManager::LoadFromConfig() - loaded %u hosted masternodes\n",
        static_cast<unsigned int>(nodes.size()));

    return true;
}

void CHostedMasternodeManager::ManageStatus()
{
    LOCK(cs);

    BOOST_FOREACH (CHostedMasternode& node, nodes) {
        std::string errorMessage;

        if (!node.SendPing(errorMessage)) {
            LogPrintf(
                "CHostedMasternodeManager::ManageStatus() - %s: %s\n",
                node.alias,
                errorMessage);
        }
    }
}

size_t CHostedMasternodeManager::Size() const
{
    LOCK(cs);
    return nodes.size();
}

std::vector<CHostedMasternodeStatus> CHostedMasternodeManager::GetStatus() const
{
    LOCK(cs);

    std::vector<CHostedMasternodeStatus> result;
    result.reserve(nodes.size());

    BOOST_FOREACH (const CHostedMasternode& node, nodes) {
        CHostedMasternodeStatus status;

        status.alias = node.alias;
        status.service = node.service.ToString();
        status.vin = node.vin.ToString();
        status.lastError = node.lastError;
        status.registered = false;
        status.networkLastPing = 0;
        status.hostLastPing = node.lastHostedPing;

        CMasternode* pmn = mnodeman.Find(node.vin);

        if (pmn != NULL) {
            status.registered = true;
            status.networkLastPing = pmn->lastPing.sigTime;
        }

        result.push_back(status);
    }

    return result;
}

void CHostedMasternodeManager::Clear()
{
    LOCK(cs);

    LogPrintf(
        "CHostedMasternodeManager::Clear() - releasing %u hosted identities\n",
        static_cast<unsigned int>(nodes.size()));

    nodes.clear();
}
