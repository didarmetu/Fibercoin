// Copyright (c) 2026 The Fibercoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING.

#ifndef FIBERCOIN_MULTIMASTERCONFIG_H
#define FIBERCOIN_MULTIMASTERCONFIG_H

#include <cstddef>
#include <string>
#include <vector>

class CMultiMasternodeConfig;
extern CMultiMasternodeConfig multiMasternodeConfig;

class CMultiMasternodeConfig
{
public:
    class CEntry
    {
    private:
        std::string alias;
        std::string privKey;
        std::string txHash;
        std::string outputIndex;

    public:
        CEntry(
            const std::string& aliasIn,
            const std::string& privKeyIn,
            const std::string& txHashIn,
            const std::string& outputIndexIn)
            : alias(aliasIn),
              privKey(privKeyIn),
              txHash(txHashIn),
              outputIndex(outputIndexIn)
        {
        }

        const std::string& getAlias() const
        {
            return alias;
        }

        const std::string& getPrivKey() const
        {
            return privKey;
        }

        const std::string& getTxHash() const
        {
            return txHash;
        }

        const std::string& getOutputIndex() const
        {
            return outputIndex;
        }
    };

    void clear();
    bool read(std::string& errorMessage);

    void add(
        const std::string& alias,
        const std::string& privKey,
        const std::string& txHash,
        const std::string& outputIndex);

    std::vector<CEntry>& getEntries()
    {
        return entries;
    }

    const std::vector<CEntry>& getEntries() const
    {
        return entries;
    }

    size_t getCount() const
    {
        return entries.size();
    }

private:
    std::vector<CEntry> entries;
};

#endif // FIBERCOIN_MULTIMASTERCONFIG_H
