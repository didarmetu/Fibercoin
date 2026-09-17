# Fibercoin Core v2.0.2.6

Fibercoin Core v2.0.2.6 is a maintenance release focused on stability, compatibility, masternode operation, and modern platform support.

## Highlights

- Improved masternode synchronization.
- Enabled masternode payment enforcement.
- Deprecated unused legacy sporks 7, 11, and 12.
- Improved spork RPC handling.
- Added a new mainnet checkpoint.
- Improved block parent handling for side-chain and out-of-order blocks.
- Improved recovery after an unclean shutdown.
- Improved compatibility with modern build dependencies.
- Modernized Linux, Windows, and macOS release workflows.
- Updated release packaging for v2.0.2.6.
- Updated Fibercoin splash screen and wallet artwork.
- Updated project copyright information through 2026.

## Masternodes

Fibercoin v2.0.2.6 includes changes intended to improve masternode synchronization and payment handling.

Masternode payment enforcement is enabled.

Legacy spork IDs 7, 11, and 12 remain reserved for compatibility but are deprecated and disabled.

After upgrading a masternode or controller wallet, verify synchronization with:

    fibercoin-cli mnsync status

and check masternode state with:

    fibercoin-cli getmasternodestatus

or:

    fibercoin-cli listmasternodes

## Blockchain and recovery

This release includes:

- a new mainnet checkpoint
- improved handling of blocks whose parent block is not yet available
- improved handling of side-chain and out-of-order blocks
- improved recovery after an unclean shutdown

After upgrading, verify the local chain with:

    fibercoin-cli getblockcount
    fibercoin-cli getbestblockhash

Operators may compare the current height and block hash with the official Fibercoin explorer:

    https://explorer.fibercoin.info/

## Platform support

The v2.0.2.6 release workflow provides builds for:

### Linux

    Ubuntu 18.04 x86-64
    Ubuntu 24.04 x86-64
    Ubuntu 26.04 x86-64

Ubuntu 24.04 is the primary current Linux release environment.

Ubuntu 18.04 is also provided for older Fibercoin systems and existing masternode deployments.

Ubuntu 26.04 is provided as an additional modern compatibility target.

### Windows

    Windows 11 x86-64

The Windows package includes:

    fibercoin-qt.exe
    fibercoind.exe
    fibercoin-cli.exe
    fibercoin-tx.exe

### macOS

The macOS release workflow builds:

    arm64
    x86_64

and produces a universal macOS package.

## Build system

The active Fibercoin release process uses GitHub Actions.

Current release workflows include:

    .github/workflows/ubuntu-18.04-build.yml
    .github/workflows/ubuntu-build.yml
    .github/workflows/ubuntu-26.04-build.yml
    .github/workflows/windows-build.yml
    .github/workflows/macos-build.yml

Legacy Gitian infrastructure remains in the repository for historical reference and possible future reproducible-build modernization, but it is not the primary v2.0.2.6 release path.

## Dependency compatibility

The build system has been modernized for current toolchains and libraries while retaining compatibility requirements of the existing Fibercoin codebase.

Important retained compatibility components include:

- Berkeley DB 4.8 for wallet database compatibility
- Qt 5 for the graphical wallet
- OpenSSL 3 support where applicable
- protobuf compatibility with the current Qt/payment-request code
- Boost, libevent, MiniUPnPc, ZeroMQ, and qrencode support

## Wallet compatibility

The v2.0.2.6 release retains the existing Berkeley DB 4.8 wallet format and does not intentionally introduce a wallet-format migration.

The default wallet continues to use Berkeley DB 4.8-compatible wallet storage.

Back up `wallet.dat` before upgrading.

Do not copy or replace a live wallet database while Fibercoin is running.

## Data compatibility

Existing blockchain data, block indexes, chainstate, peer data, and masternode cache data remain part of the normal Fibercoin data directory layout.

A normal upgrade should not require deleting blockchain or wallet data.

If database recovery or reindexing is required for a specific system, shut down Fibercoin completely before performing maintenance.

## RPC authentication

Fibercoin supports automatic RPC cookie authentication.

When no explicit `rpcpassword` is configured, Fibercoin generates a temporary authentication cookie in the data directory.

This allows local RPC use without requiring a static RPC password in normal desktop configurations.

## Tor support

Fibercoin can route outbound network connections through a Tor SOCKS5 proxy.

Example:

    proxy=127.0.0.1:9050

Modern Tor onion-service support remains limited by the legacy onion-address implementation in Fibercoin v2.0.2.6.

See:

    doc/tor.md

## Bootstrap synchronization

Fibercoin can import historical block data from a trusted `bootstrap.dat` file.

Official bootstrap downloads may be published through:

    https://github.com/didarmetu/Fibercoin/releases

The official Fibercoin explorer can be used to compare block heights and block hashes after import:

    https://explorer.fibercoin.info/

See:

    doc/bootstrap.md

## Upgrade instructions

Before upgrading:

1. Back up `wallet.dat`.
2. Back up important configuration files.
3. Shut down Fibercoin normally.
4. Wait until the process has fully exited.

Then replace the previous application or binaries with the v2.0.2.6 release.

After restarting, verify:

    fibercoin-cli getblockcount
    fibercoin-cli getbestblockhash
    fibercoin-cli getconnectioncount

For masternodes, also verify:

    fibercoin-cli mnsync status
    fibercoin-cli getmasternodestatus

## Release downloads

Official Fibercoin releases are published at:

    https://github.com/didarmetu/Fibercoin/releases

Verify release checksums when they are provided.

## Support and resources

Website:

    https://fibercoin.info/

Explorer:

    https://explorer.fibercoin.info/

Discord:

    https://discord.gg/hUvXmJj

Fiberchange:

    https://fiberchange.com/
