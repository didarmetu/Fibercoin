// Copyright (c) 2013-2014 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block.CheckBlock()
//



#include "clientversion.h"
#include "main.h"
#include "utiltime.h"

#include <cstdio>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(CheckBlock_tests)

bool read_block(const std::string& filename, CBlock& block)
{
    namespace fs = boost::filesystem;
    fs::path testFile = fs::current_path() / "data" / filename;
#ifdef TEST_DATA_DIR
    if (!fs::exists(testFile))
    {
        testFile = fs::path(BOOST_PP_STRINGIZE(TEST_DATA_DIR)) / filename;
    }
#endif
    FILE* fp = fopen(testFile.string().c_str(), "rb");
    if (!fp) return false;

    fseek(fp, 8, SEEK_SET); // skip msgheader/size

    CAutoFile filein(fp, SER_DISK, CLIENT_VERSION);
    if (filein.IsNull()) return false;

    filein >> block;

    return true;
}

BOOST_AUTO_TEST_CASE(May15)
{
    // Putting a 1MB binary file in the git repository is not a great
    // idea, so this test is only run if you manually download
    // test/data/Mar12Fork.dat from
    // http://sourceforge.net/projects/bitcoin/files/Bitcoin/blockchain/Mar12Fork.dat/download
    unsigned int tMay15 = 1368576000;
    SetMockTime(tMay15); // Test as if it was right at May 15

    CBlock forkingBlock;
    if (read_block("Mar12Fork.dat", forkingBlock))
    {
        CValidationState state;

        // After May 15'th, big blocks are OK:
        forkingBlock.nTime = tMay15; // Invalidates PoW
        BOOST_CHECK(CheckBlock(forkingBlock, state, false, false));
    }

    SetMockTime(0);
}


BOOST_AUTO_TEST_CASE(block_prev_index_active_tip)
{
    CBlockIndex parent;
    parent.nHeight = 760700;

    CBlockHeader header;
    header.nVersion = 1;
    header.nTime = 1;
    header.nBits = 1;
    header.nNonce = 1;

    parent.phashBlock = new uint256(header.GetHash());

    CBlockIndex* originalTip = chainActive.Tip();
    chainActive.SetTip(&parent);

    CBlock block;
    block.hashPrevBlock = parent.GetBlockHash();

    CBlockIndex* result = GetBlockPrevIndex(block);

    BOOST_REQUIRE(result != NULL);
    BOOST_CHECK(result == &parent);
    BOOST_CHECK_EQUAL(result->nHeight, 760700);

    chainActive.SetTip(originalTip);

    delete parent.phashBlock;
    parent.phashBlock = NULL;
}

BOOST_AUTO_TEST_CASE(block_prev_index_side_chain)
{
    CBlockIndex activeTip;
    activeTip.nHeight = 760750;

    CBlockHeader activeHeader;
    activeHeader.nVersion = 1;
    activeHeader.nTime = 10;
    activeHeader.nBits = 1;
    activeHeader.nNonce = 10;

    activeTip.phashBlock = new uint256(activeHeader.GetHash());

    CBlockIndex sideParent;
    sideParent.nHeight = 760700;

    CBlockHeader sideHeader;
    sideHeader.nVersion = 1;
    sideHeader.nTime = 20;
    sideHeader.nBits = 1;
    sideHeader.nNonce = 20;

    sideParent.phashBlock = new uint256(sideHeader.GetHash());

    CBlockIndex* originalTip = chainActive.Tip();
    chainActive.SetTip(&activeTip);

    mapBlockIndex[sideParent.GetBlockHash()] = &sideParent;

    CBlock block;
    block.hashPrevBlock = sideParent.GetBlockHash();

    CBlockIndex* result = GetBlockPrevIndex(block);

    BOOST_REQUIRE(result != NULL);
    BOOST_CHECK(result == &sideParent);
    BOOST_CHECK(result != chainActive.Tip());
    BOOST_CHECK_EQUAL(result->nHeight, 760700);

    mapBlockIndex.erase(sideParent.GetBlockHash());
    chainActive.SetTip(originalTip);

    delete activeTip.phashBlock;
    activeTip.phashBlock = NULL;

    delete sideParent.phashBlock;
    sideParent.phashBlock = NULL;
}

BOOST_AUTO_TEST_CASE(block_prev_index_unknown_parent)
{
    CBlock block;
    block.hashPrevBlock =
        uint256S("0000000000000000000000000000000000000000000000000000000012345678");

    BlockMap::iterator existing = mapBlockIndex.find(block.hashPrevBlock);
    CBlockIndex* previous = NULL;

    if (existing != mapBlockIndex.end()) {
        previous = existing->second;
        mapBlockIndex.erase(existing);
    }

    BOOST_CHECK(GetBlockPrevIndex(block) == NULL);

    if (previous != NULL)
        mapBlockIndex[block.hashPrevBlock] = previous;
}

BOOST_AUTO_TEST_SUITE_END()
