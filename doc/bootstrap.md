# Fibercoin Blockchain Bootstrap

Fibercoin normally synchronizes the blockchain directly from other Fibercoin nodes.

For a fresh installation, synchronization may take some time. Fibercoin can also import historical block data from a `bootstrap.dat` file before continuing normal network synchronization.

## Important

Only use a `bootstrap.dat` file from an official or trusted Fibercoin source.

The latest official Fibercoin bootstrap can be downloaded from:

    https://github.com/didarmetu/Fibercoin/releases

The official Fibercoin block explorer is available at:

    https://explorer.fibercoin.info/

You can use the explorer to verify block heights and block hashes after importing the bootstrap. The latest `bootstrap.dat` is also available through the official Fibercoin explorer.

Do not use untrusted copies of:

- `wallet.dat`
- `chainstate/`
- `blocks/index/`
- LevelDB database files
- configuration files containing private keys or RPC credentials

Fibercoin validates imported blocks before accepting them.

## Default Fibercoin data directories

### Windows

    %APPDATA%\Fibercoin

### macOS

    ~/Library/Application Support/Fibercoin

### Linux

    ~/.fibercoin

## Download and verify

Download the latest official `bootstrap.dat` from:

    https://github.com/didarmetu/Fibercoin/releases

If a SHA-256 checksum is provided with the release, verify the downloaded file before importing it.

On Linux:

    sha256sum bootstrap.dat

On macOS:

    shasum -a 256 bootstrap.dat

## Importing bootstrap.dat

1. Shut down Fibercoin completely.
2. Copy `bootstrap.dat` into the Fibercoin data directory.
3. Start Fibercoin normally.
4. Fibercoin will detect and import `bootstrap.dat`.
5. After the import finishes, Fibercoin will continue synchronizing from the network.

During import, the client may display:

    Importing blocks from disk...

After processing, Fibercoin renames the bootstrap file to:

    bootstrap.dat.old

## Importing another block file

Fibercoin also supports importing an external block file explicitly:

    fibercoind -loadblock=/path/to/blockfile.dat

The same option can be passed when starting the Qt wallet.

## Checking synchronization

You can monitor synchronization with:

    fibercoin-cli getblockcount
    fibercoin-cli getbestblockhash
    fibercoin-cli getconnectioncount

For masternode operation, also check:

    fibercoin-cli mnsync status

Wait until blockchain and masternode synchronization are complete before relying on staking or masternode services.

## Verification

After importing historical blocks, Fibercoin continues normal block validation and network synchronization.

Check your local node with:

    fibercoin-cli getblockcount
    fibercoin-cli getbestblockhash

You can compare the block height and block hash with the official Fibercoin explorer:

    https://explorer.fibercoin.info/

For additional verification, compare the same block height against another trusted Fibercoin node.

## Security

A bootstrap file should contain raw block data only.

Never download or replace wallet files, chainstate databases, or private configuration files from untrusted sources.
