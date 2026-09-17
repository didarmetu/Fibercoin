# Fibercoin Unit Tests

Fibercoin includes unit tests for core, wallet, networking, RPC, consensus, serialization, cryptography, and Qt functionality.

Tests are compiled by default when the required dependencies are available.

## Build and run the test suite

After configuring Fibercoin, run:

    make check

This executes the configured test targets, including the core Fibercoin test binary.

## Core unit tests

The main core test executable is:

    src/test/test_fibercoin

Run it directly with:

    src/test/test_fibercoin

The test suite uses Boost.Test.

Core test sources are located in:

    src/test/

Examples include tests for:

- block validation
- checkpoints
- RPC behavior
- wallet behavior
- networking
- transaction validation
- signatures
- serialization
- cryptography
- mempool behavior
- mining
- DoS handling

## Adding core tests

Add new `BOOST_AUTO_TEST_CASE` tests to an existing test source file, or add a new source file under:

    src/test/

New test files must also be added to the test build configuration.

## Qt tests

Qt test sources are located in:

    src/qt/test/

Current Qt tests include:

- URI handling
- payment server behavior

The Qt test entry point is:

    src/qt/test/test_main.cpp

## Configure options

Tests are enabled by default.

They can be explicitly enabled with:

    ./configure --enable-tests

Test compilation can be disabled with:

    ./configure --disable-tests

## Release validation

Before release, run the complete configured test suite:

    make check

CI release builds should also complete successfully on all supported platforms before tagging a release.
