// Copyright (c) 2026 The Fibercoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING.

#include "hostedmasternode.h"

#include "masternode-helpers.h"
#include "multimasterconfig.h"
#include "masternodeman.h"
#include "util.h"

#include <exception>
#include <set>

CHostedMasternodeManager hostedMasternodes;

CHostedMasternode::CHostedMasternode()
    : lastHostedPing(0)
{
}

void CHostedMasternode::SetLastError(const std::string& error)
{
    LOCK(csState);
    lastError = error;
}

void CHostedMasternode::ClearLastError()
{
    LOCK(csState);
    lastError.clear();
}

void CHostedMasternode::SetLastHostedPing(int64_t sigTime)
{
    LOCK(csState);
    lastHostedPing = sigTime;
}

void CHostedMasternode::GetRuntimeStatus(
    std::string& error,
    int64_t& hostedPing) const
{
    LOCK(csState);
    error = lastError;
    hostedPing = lastHostedPing;
}


bool CHostedMasternode::Configure(
    const std::string& strAlias,
    const std::string& strKey,
    const std::string& strTxHash,
    const std::string& strOutputIndex,
    std::string& errorMessage)
{
    alias = strAlias;

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

    ClearLastError();

    LogPrintf(
        "CHostedMasternode::Configure() - loaded %s, vin=%s\n",
        alias,
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
        SetLastError(errorMessage);
        return false;
    }

    // The masternode is now known to the network, so clear any
    // earlier "not present" diagnostic.
    ClearLastError();

    CMasternodePing mnp(vin);

    if (!mnp.Sign(keyMasternode, pubKeyMasternode)) {
        errorMessage = strprintf(
            "Could not sign masternode ping for hosted masternode %s",
            alias);
        SetLastError(errorMessage);
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

    SetLastHostedPing(mnp.sigTime);
    ClearLastError();

    LogPrintf(
        "CHostedMasternode::SendPing() - relayed ping for %s, vin=%s\n",
        alias,
        vin.ToString());

    return true;
}

bool CHostedMasternodeManager::LoadFromConfig(std::string& errorMessage)
{
    std::vector<std::shared_ptr<CHostedMasternode> > loadedNodes;

    std::set<std::string> aliases;
    std::set<COutPoint> vins;
    std::set<CPubKey> hotKeys;

    std::vector<CMultiMasternodeConfig::CEntry>& entries =
        multiMasternodeConfig.getEntries();

    BOOST_FOREACH (
        CMultiMasternodeConfig::CEntry& entry,
        entries) {

        if (entry.getAlias().empty()) {
            continue;
        }

        std::shared_ptr<CHostedMasternode> node(
            new CHostedMasternode());

        std::string entryError;

        if (!node->Configure(
                entry.getAlias(),
                entry.getPrivKey(),
                entry.getTxHash(),
                entry.getOutputIndex(),
                entryError)) {
            errorMessage = entryError;
            return false;
        }

        if (!aliases.insert(node->alias).second) {
            errorMessage = strprintf(
                "Duplicate hosted masternode alias: %s",
                node->alias);
            return false;
        }

        if (!vins.insert(node->vin.prevout).second) {
            errorMessage = strprintf(
                "Duplicate hosted masternode VIN for alias %s: %s",
                node->alias,
                node->vin.prevout.ToString());
            return false;
        }

        if (!hotKeys.insert(node->pubKeyMasternode).second) {
            errorMessage = strprintf(
                "Duplicate hosted masternode hot key for alias %s",
                node->alias);
            return false;
        }

        loadedNodes.push_back(node);
    }

    {
        LOCK(cs);
        nodes.swap(loadedNodes);
    }

    LogPrintf(
        "CHostedMasternodeManager::LoadFromConfig() - loaded %u hosted masternodes\n",
        static_cast<unsigned int>(Size()));

    return true;
}

void CHostedMasternodeManager::ManageStatus()
{
    std::vector<std::shared_ptr<CHostedMasternode> > activeNodes;

    {
        LOCK(cs);
        activeNodes = nodes;
    }

    BOOST_FOREACH (
        const std::shared_ptr<CHostedMasternode>& node,
        activeNodes) {

        std::string errorMessage;

        if (!node->SendPing(errorMessage)) {
            LogPrintf(
                "CHostedMasternodeManager::ManageStatus() - %s: %s\n",
                node->alias,
                errorMessage);
        }
    }

    // Compact operational summary after each MultiMaster management cycle.
    const std::vector<CHostedMasternodeStatus> statuses = GetStatus();

    unsigned int registeredCount = 0;
    unsigned int hostPingedCount = 0;
    unsigned int errorCount = 0;

    BOOST_FOREACH (
        const CHostedMasternodeStatus& status,
        statuses) {

        if (status.registered)
            registeredCount++;

        if (status.hostLastPing > 0)
            hostPingedCount++;

        if (!status.lastError.empty())
            errorCount++;
    }

    LogPrintf(
        "MULTI-MASTERNODE HOST STATUS: loaded=%u registered=%u host_pinged=%u errors=%u\n",
        static_cast<unsigned int>(statuses.size()),
        registeredCount,
        hostPingedCount,
        errorCount);
}

size_t CHostedMasternodeManager::Size() const
{
    LOCK(cs);
    return nodes.size();
}

std::vector<CHostedMasternodeStatus>
CHostedMasternodeManager::GetStatus() const
{
    std::vector<std::shared_ptr<CHostedMasternode> > activeNodes;

    {
        LOCK(cs);
        activeNodes = nodes;
    }

    std::vector<CHostedMasternodeStatus> result;
    result.reserve(activeNodes.size());

    BOOST_FOREACH (
        const std::shared_ptr<CHostedMasternode>& node,
        activeNodes) {

        CHostedMasternodeStatus status;

        status.alias = node->alias;
        status.service.clear();
        status.vin = node->vin.ToString();
        status.registered = false;
        status.networkLastPing = 0;

        node->GetRuntimeStatus(
            status.lastError,
            status.hostLastPing);

        CMasternode* pmn = mnodeman.Find(node->vin);

        if (pmn != NULL) {
            status.registered = true;
            status.service = pmn->addr.ToString();
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
