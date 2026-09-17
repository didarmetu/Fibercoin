# Fibercoin Data Directory

Fibercoin stores blockchain data, wallet data, network state, configuration, and runtime files in its data directory.

Default locations are:

### Windows

    %APPDATA%\Fibercoin

### macOS

    ~/Library/Application Support/Fibercoin

### Linux

    ~/.fibercoin

A different data directory can be selected with:

    -datadir=<path>

## Configuration files

### fibercoin.conf

Main Fibercoin configuration file.

It can contain networking, RPC, wallet, staking, masternode, proxy, and other node settings.

### masternode.conf

Controller-wallet configuration for one or more Fibercoin masternodes.

See:

    doc/masternode_conf.md

## Wallet files

### wallet.dat

The default Berkeley DB wallet containing private keys, addresses, transactions, and wallet metadata.

This is one of the most important files to back up.

Do not replace or copy a live `wallet.dat` while Fibercoin is running.

### backups/

Fibercoin may store automatic wallet backups in this directory.

Backup filenames can include timestamps.

### database/

Berkeley DB environment files used by the wallet database.

### db.log

Berkeley DB diagnostic log.

## Blockchain data

### blocks/blkNNNNN.dat

Raw validated blockchain data.

Fibercoin rotates block files at a maximum size of 128 MiB.

Examples:

    blocks/blk00000.dat
    blocks/blk00001.dat

### blocks/revNNNNN.dat

Undo data used when disconnecting blocks during chain reorganizations.

Examples:

    blocks/rev00000.dat
    blocks/rev00001.dat

### blocks/index/

LevelDB block index.

### chainstate/

LevelDB database containing the current unspent transaction output state.

Do not download or replace `chainstate/`, `blocks/index/`, or LevelDB files from untrusted sources.

## Network data

### peers.dat

Persistent peer-address database.

Fibercoin periodically saves known peer addresses to this file.

If the file is missing or invalid, Fibercoin can recreate it as new peers are discovered.

### banlist.dat

Stores persistent peer-ban information.

### fee_estimates.dat

Stores transaction fee-estimation statistics.

## Masternode data

### mncache.dat

Cached Fibercoin masternode-list data.

Fibercoin can recreate this file from network synchronization if it is missing or invalid.

### mnpayments.dat

Cached masternode-payment information.

Fibercoin can recreate this information from the network when necessary.

### budget.dat

Cached data for the legacy masternode budget subsystem.

The budget subsystem remains present for compatibility even when budget payments are not active on the network.

## Spork data

### sporks/

LevelDB database containing persisted Fibercoin spork messages.

This directory is part of Fibercoin's network state and can be rebuilt through synchronization when appropriate.

## Runtime files

### debug.log

Main Fibercoin diagnostic and runtime log.

This is normally the first file to inspect when troubleshooting startup, synchronization, networking, wallet, or masternode problems.

### fibercoind.pid

Contains the process ID of a running `fibercoind` process when PID-file support is in use.

### .lock

Prevents multiple Fibercoin processes from using the same data directory simultaneously.

Do not manually remove this file while Fibercoin is running.

### .cookie

Temporary RPC authentication cookie created when automatic cookie authentication is used.

The cookie contains authentication credentials and should not be shared.

Fibercoin normally removes it during clean shutdown.

## Blockchain import files

### bootstrap.dat

Optional raw block-data file imported automatically when Fibercoin starts.

After processing, Fibercoin renames it to:

    bootstrap.dat.old

See:

    doc/bootstrap.md

## Files that can usually be rebuilt

Some data can be recreated by Fibercoin, including network and cache data such as:

    peers.dat
    mncache.dat
    mnpayments.dat
    budget.dat

Blockchain indexes and chainstate can also be rebuilt from validated block data, although this may take considerable time.

Do not delete files merely as a troubleshooting step unless you understand what Fibercoin will rebuild and what will need to be downloaded again.

## Files that require special protection

The most important user-owned file is:

    wallet.dat

Also protect:

    fibercoin.conf
    masternode.conf
    .cookie

These files can contain private keys, masternode credentials, RPC credentials, or other sensitive configuration.

## Backups

Before upgrading or performing database maintenance:

1. Shut down Fibercoin normally.
2. Confirm the process has exited.
3. Back up `wallet.dat`.
4. Back up any important configuration files.
5. Keep backups in a secure location separate from the active data directory.

Never rely only on blockchain or cache files as a wallet backup.
