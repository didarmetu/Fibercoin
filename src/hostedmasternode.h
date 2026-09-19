// Copyright (c) 2026 The Fibercoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING.

#ifndef FIBERCOIN_HOSTEDMASTERNODE_H
#define FIBERCOIN_HOSTEDMASTERNODE_H

#include "key.h"
#include "masternode.h"
#include "net.h"
#include "sync.h"

#include <cstddef>
#include <string>
#include <vector>

class CHostedMasternode
{
public:
    std::string alias;
    CService service;
    CTxIn vin;

    CKey keyMasternode;
    CPubKey pubKeyMasternode;

    std::string lastError;
    int64_t lastHostedPing;

    CHostedMasternode();

    bool Configure(
        const std::string& strAlias,
        const std::string& strService,
        const std::string& strKey,
        const std::string& strTxHash,
        const std::string& strOutputIndex,
        std::string& errorMessage);

    bool SendPing(std::string& errorMessage);
};

struct CHostedMasternodeStatus
{
    std::string alias;
    std::string service;
    std::string vin;
    std::string lastError;
    bool registered;
    int64_t networkLastPing;
    int64_t hostLastPing;
};

class CHostedMasternodeManager
{
private:
    mutable CCriticalSection cs;
    std::vector<CHostedMasternode> nodes;

public:
    bool LoadFromConfig(std::string& errorMessage);
    void ManageStatus();
    void Clear();

    size_t Size() const;
    std::vector<CHostedMasternodeStatus> GetStatus() const;
};

extern CHostedMasternodeManager hostedMasternodes;

#endif // FIBERCOIN_HOSTEDMASTERNODE_H
