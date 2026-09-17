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

Each remote masternode should use the masternode private key corresponding to its entry in `masternode.conf`.

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

## Security

- Back up the controller wallet before changing masternode configuration.
- Keep collateral wallet private keys secure.
- Use a unique masternode private key for each masternode.
- Never share RPC credentials or wallet private keys.
- Verify the collateral transaction ID and output index before starting a masternode.
