# Fibercoin Core v2.0.2.6

Fibercoin Core v2.0.2.6 is a maintenance release focused on stability, compatibility, masternode operation, and modern platform support.

## Changes

- Improved masternode synchronization.
- Enabled masternode payment enforcement.
- Deprecated unused legacy sporks 7, 11, and 12.
- Improved spork RPC handling.
- Added a new mainnet checkpoint.
- Improved block parent handling for side-chain and out-of-order blocks.
- Improved recovery after an unclean shutdown.
- Improved compatibility with modern build dependencies.
- Updated macOS, Windows, and Ubuntu build workflows.
- Updated release packaging for v2.0.2.6.
- Updated Fibercoin splash screen and wallet artwork.
- Updated project copyright information through 2026.

## Compatibility

Legacy spork IDs 7, 11, and 12 remain reserved for compatibility and are disabled.

Existing Fibercoin wallets and blockchain data remain compatible with this release.

## Upgrade

Back up your wallet before upgrading.

Shut down the previous Fibercoin wallet normally, replace the application/binaries with v2.0.2.6, and restart Fibercoin.
