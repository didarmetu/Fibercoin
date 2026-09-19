// Copyright (c) 2026 The Fibercoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING.

#include "multimasterconfig.h"

#include "util.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <cstdio>
#include <cstring>
#include <sstream>

CMultiMasternodeConfig multiMasternodeConfig;

void CMultiMasternodeConfig::clear()
{
    entries.clear();
}

void CMultiMasternodeConfig::add(
    const std::string& alias,
    const std::string& privKey,
    const std::string& txHash,
    const std::string& outputIndex)
{
    entries.push_back(
        CEntry(alias, privKey, txHash, outputIndex));
}

bool CMultiMasternodeConfig::read(std::string& errorMessage)
{
    clear();

    const boost::filesystem::path pathConfig =
        GetMultiMasternodeConfigFile();

    boost::filesystem::ifstream streamConfig(pathConfig);

    if (!streamConfig.good()) {
        FILE* configFile =
            fopen(pathConfig.string().c_str(), "a");

        if (configFile != NULL) {
            const std::string header =
                "# Fibercoin MultiMaster host configuration\n"
                "# Format: alias masternodeprivkey collateral_output_txid collateral_output_index\n"
                "# Example: mn1 93HaYBVUCYjEMeeH1Y4sBGLALQZE1Yc1K64xiqgX37tGBDQL8Xg "
                "2bcd3c84c84f87eaa86e4e56834c92927a07f9e18718810b92e0d0324456a67c 0\n";

            fwrite(
                header.c_str(),
                std::strlen(header.c_str()),
                1,
                configFile);

            fclose(configFile);
        }

        return true;
    }

    int lineNumber = 1;

    for (std::string line;
         std::getline(streamConfig, line);
         lineNumber++) {

        if (line.empty())
            continue;

        std::istringstream iss(line);

        std::string first;
        if (!(iss >> first))
            continue;

        if (!first.empty() && first[0] == '#')
            continue;

        iss.clear();
        iss.str(line);

        std::string alias;
        std::string privKey;
        std::string txHash;
        std::string outputIndex;
        std::string extra;

        if (!(iss >> alias >> privKey >> txHash >> outputIndex) ||
            (iss >> extra)) {

            errorMessage =
                "Could not parse multimaster.conf\n" +
                strprintf("Line: %d", lineNumber);

            streamConfig.close();
            return false;
        }

        add(alias, privKey, txHash, outputIndex);
    }

    streamConfig.close();
    return true;
}
