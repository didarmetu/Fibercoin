# Fibercoin Masternode Configuration

The `masternode.conf` file allows a controller wallet to manage multiple Fibercoin masternodes.

Each masternode requires exactly **10,000 FBC** in a valid collateral transaction output.

## Configuration file location

By default, `masternode.conf` is stored in the Fibercoin data directory.

### Windows

    %APPDATA%\Fibercoin\masternode.conf

### macOS

    ~/Library/Application Support/Fibercoin/masternode.conf

### Linux

    ~/.fibercoin/masternode.conf

A different configuration file can be selected with:

    -mnconf=<file>

## masternode.conf format

Each non-comment line contains five space-separated fields:

    alias IP:port masternodeprivkey collateral_output_txid collateral_output_index

The fields are:

1. `alias` - Local name used to identify the masternode.
2. `IP:port` - Public network address of the remote masternode.
3. `masternodeprivkey` - Masternode private key generated for that node.
4. `collateral_output_txid` - Transaction ID containing the 10,000 FBC collateral.
5. `collateral_output_index` - Output index of the collateral transaction.

Example:

    mn01 203.0.113.10:30114 MASTERNODE_PRIVATE_KEY TXID 0
    mn02 203.0.113.11:30114 MASTERNODE_PRIVATE_KEY TXID 1

Replace the example addresses, keys, transaction IDs, and output indexes with the actual values for your masternodes.

The Fibercoin mainnet port is:

    30114

## Generate a masternode private key

From the Fibercoin Debug Console:

    masternode genkey

The individual RPC equivalent is:

    createmasternodekey

Use a unique masternode private key for every masternode.

A masternode private key is not the same as the wallet private key controlling the 10,000 FBC collateral.

Never place the collateral wallet private key on the remote server.

## Find collateral outputs

From the controller wallet:

    masternode outputs

The individual RPC equivalent is:

    getmasternodeoutputs

The command returns compatible collateral transaction IDs and output indexes.

## Check masternode.conf

To display the parsed configuration:

    masternode list-conf

The individual RPC equivalent is:

    listmasternodeconf

Review the alias, network address, collateral transaction ID, output index, and status before starting the masternodes.

## Start masternodes

The legacy `masternode` command is retained for backward compatibility.

Start a single configured masternode:

    masternode start-alias mn01

Start all configured masternodes:

    masternode start-many

Additional supported start modes are:

    masternode start-all
    masternode start-missing
    masternode start-disabled

The preferred individual RPC is `startmasternode`.

Examples:

    startmasternode "alias" false "mn01"
    startmasternode "all" false
    startmasternode "missing" false
    startmasternode "disabled" false

The boolean argument controls whether the wallet is locked after the operation.

For complete RPC usage:

    help startmasternode

## Remote masternode configuration

For legacy single-masternode deployments, each remote masternode runs its own daemon and uses the masternode private key corresponding to its entry in `masternode.conf`.

Typical remote settings include:

    masternode=1
    masternodeprivkey=MASTERNODE_PRIVATE_KEY

Do not place the wallet private key controlling the 10,000 FBC collateral on the remote server.

## Check masternode status

On a remote masternode:

    fibercoin-cli getmasternodestatus

From a controller wallet, you can inspect configured nodes with:

    masternode list-conf

and the network masternode list with:

    listmasternodes

## MultiMaster host mode

Fibercoin can also host multiple masternode identities from a single
`fibercoind` process.

This mode is intended for operators running many masternodes who want to
share the blockchain, network connections, transaction index, and other
full-node resources instead of running one full daemon for every masternode.

A MultiMaster host is configured with:

    disablewallet=1
    multimaster=1
    txindex=1

The hosted masternode identities are stored separately from the controller
configuration in:

    multimaster.conf

By default, `multimaster.conf` is stored in the Fibercoin data directory.

A different file can be selected with:

    -multimasterconf=<file>

Each non-comment line contains four space-separated fields:

    alias masternodeprivkey collateral_output_txid collateral_output_index

Example:

    mn01 MASTERNODE_PRIVATE_KEY TXID 0
    mn02 MASTERNODE_PRIVATE_KEY TXID 1

The public `IP:port` is intentionally not stored in `multimaster.conf`.

The controller wallet supplies the service address when it creates the
masternode broadcast. After the masternode is registered, the MultiMaster
host obtains the service address from the network masternode list.

The controller wallet still uses the normal five-field `masternode.conf`
entry and performs the initial masternode start or broadcast.

After registration, the MultiMaster host maintains masternode pings using
the corresponding masternode private keys.

### MultiMaster status

Check all identities hosted by the current daemon with:

    fibercoin-cli multimasterstatus

The summary includes:

    enabled
    loaded
    registered
    host_pinged
    unregistered
    waiting_for_host_ping
    errors

`getmasternodestatus` and `masternodedebug` are legacy single-masternode
commands and should not be used to determine MultiMaster host status.

Network-wide masternode information remains available through:

    getmasternodecount
    listmasternodes
    mnsync status

### MultiMaster configuration validation

The MultiMaster host rejects duplicate:

- aliases
- collateral transaction outputs (VINs)
- masternode private keys

Multiple masternodes may use the same service address or `IP:port`.

### MultiMaster security

`multimaster.conf` contains masternode hot private keys.

It does not contain the wallet private keys controlling the collateral,
and collateral wallet private keys should never be copied to the
MultiMaster host.

Restrict access to the configuration file appropriately. On Unix-like
systems, for example:

    chmod 600 multimaster.conf

## Security

- Back up the controller wallet before changing masternode configuration.
- Keep collateral wallet private keys secure.
- Use a unique masternode private key for each masternode.
- Never share RPC credentials or wallet private keys.
- Verify the collateral transaction ID and output index before starting a masternode.
