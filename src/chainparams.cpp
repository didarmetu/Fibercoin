// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2019-2024 The Fibercoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "bignum.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{

    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}


static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x00000e27f86446a58eb1ed444b3d76e9956827ca233a2c728524f48dba269865"))
    (1, uint256("0x2a36dbd1bba0875b2939977eab157fa6f82b428fc9c517a5a11c75e0812e9718"))
    (200, uint256("0x000000361219e67839a688bfb963d5a584ebbcb22037f2c20d1982e82e2c11fa"))
    (201, uint256("0x9dde4d97aaf484be89c1913b29e7eb1cfc29781dc6809ecdcf112317345b32a7"))
    (425, uint256("0x1c206da9aeecdc628f5c7d7a8c1c5ee34a690aa914a294ab2098c6ef0458bf58"))
    (3333, uint256("0x10b43087d0e1baa13cd29aa04633b68fcbf874cc465613bce9e9bfa6f5d1bcfa"))
    (100050, uint256("0xcadce96fa4112bad8508be191ae7cafe2b20af793a2190fc815ac722782edf41"))
    (358840, uint256("7714dd858ea46ad076477189cabc5fb8b247cf12340aff42a8b1d47404137479"))
    (516400, uint256("c368d7ec4dbf10caf66e0c488c65149d7b6fd344a4fd411bc49e8796a22661de"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1725045016, // * UNIX timestamp of last checkpoint block
    1033836,        // * total number of transactions between genesis and last checkpoint   (the tx=... number in the SetBestChain debug.log lines)
    1000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0xab;
        pchMessageStart[2] = 0xb1;
        pchMessageStart[3] = 0xac;
        vAlertPubKey = ParseHex("04419719320f408520c2203aba2a60df7df09f13c1b4bc51054c479c2d1453244fa49aeef2a9389a82dc18fe2080275b1e1b4d4faaaf97568378545857cffc7e56");
        nDefaultPort = 30114;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nSubsidyHalvingInterval = 1050000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 40 * 60;
        nTargetSpacing = 4 * 60;  // Fibercoin: 5 minute blocks during POW (block 1-200)
        nMaturity = 30; // 30 block maturity (+1 elsewhere)
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 21000000 * COIN; // 21 million max supply
        nLastPOWBlock = 200;
        nModifierUpdateBlock = 1; // we use the version 2 for FBC

        const char* pszTimestamp = "Bitcoin in now on 628,850 block -  4th May, 2020, Bitcoin Halving is coming.";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04c5ce0fa3566ba875620bd495cad1941519edc9c2a991305df1b4d288060915fea74c881210b5f73b176554a10624244738d77701fd164121fce5bc3309f79411") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1588572000;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 24024825;
        // uncommet for generate new genesis.
       // if(genesis.GetHash() != uint256("0x"))
       // {
       //               printf("MSearching for genesis block...\n");
       //               uint256 hashTarget;
       //               hashTarget.SetCompact(genesis.nBits);
       //               while(uint256(genesis.GetHash()) > uint256(hashTarget))
       //               {
       //                   ++genesis.nNonce;
       //                   if (genesis.nNonce == 0)
       //                   {
       //                       printf("Mainnet NONCE WRAPPED, incrementing time");
       //                       std::cout << std::string("Mainnet NONCE WRAPPED, incrementing time:\n");
       //                       ++genesis.nTime;
       //                   }
       //                   if (genesis.nNonce % 10000 == 0)
       //                   {
       //                       printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
       //                   }
       //               }
       //               printf("Mainnet block.nTime = %u \n", genesis.nTime);
       //               printf("Mainnet block.nNonce = %u \n", genesis.nNonce);
       //               printf("Mainnet block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
       //               printf("Mainnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
       // }
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000e27f86446a58eb1ed444b3d76e9956827ca233a2c728524f48dba269865"));
        assert(genesis.hashMerkleRoot == uint256("0x59c37f2ec4e3db6745ed9819960f98d972733c493ddd78a105905e3bc4de2368"));

        // Fibercoin addresses start with 'F'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 35);
        // Fibercoin script addresses start with 'D'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 30);
        // Fibercoin private keys start with 'f'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 95);
        // Fibercoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Fibercoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // Fibercoin BIP44 coin type is '222' (0x800000de)
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0xde).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04c5ce0fa3566ba875620bd495cad1941519edc9c2a991305df1b4d288060915fea74c881210b5f73b176554a10624244738d77701fd164121fce5bc3309f79411";
        strMasternodePoolDummyAddress = "FJ3ivxb7fwrMCdyYeeRd7TsRTQfhNRcTeh";
        nStartMasternodePayments = genesis.nTime + 86400; // 24 hours after genesis

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x4a;
        pchMessageStart[1] = 0x2d;
        pchMessageStart[2] = 0x32;
        pchMessageStart[3] = 0xbc;
        vAlertPubKey = ParseHex("041b2b4c86273359acac3522471911ed2b303eaab65e8a1de01c06e89f2eab1e55234a4b504f3ce20c6f661f007d0ca15623b4358d9855c7c8ba793a24fa315e22");
        nDefaultPort = 16942;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 40 * 60; // test Fibercoin:
        nTargetSpacing = 4 * 60;  // test Fibercoin: 5 minute
        nLastPOWBlock = 200;
        nMaturity = 15;  //test
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 1;
        nMaxMoneyOut = 21000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1516926684;
        genesis.nNonce = 21256609;

        hashGenesisBlock = genesis.GetHash();
       // assert(hashGenesisBlock == uint256("0x000008467c3a9c587533dea06ad9380cded3ed32f9742a6c0c1aebc21bf2bc9b"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Testnet Fibercoin addresses start with 'g'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 98);
        // Testnet Fibercoin script addresses start with '5' or '6'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 12);
        // Testnet private keys start with 'k'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 108);
        // Testnet Fibercoin BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Fibercoin BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet fibercoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04abb5e65280dda6a113fadfb9877f9c399532245fe1acb61de293ab298034d5084277fab3768774a3b68cbbe5021cc5049ec8c9997a13f64da1afa0bcfb156db1";
        strMasternodePoolDummyAddress = "gbJ4Qad4xc77PpLzMx6rUegAs6aUPWkcUq";
        nStartMasternodePayments = genesis.nTime + 86400; // 24 hours after genesis
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x20;
        pchMessageStart[1] = 0xee;
        pchMessageStart[2] = 0x32;
        pchMessageStart[3] = 0xbc;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 40 * 60; //  regtest Fibercoin: 10 min
        nTargetSpacing = 4 * 60;        // regtest Fibercoin: 4 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1516926684;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 20542300;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 26942;
       // assert(hashGenesisBlock == uint256("0x229874aa8a92df3347600978e226ba57bc994b9fa291ea50519afafca2d50ed3"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
