# Fibercoin Masternode Start-Many Guide

This guide explains how to configure and start multiple Fibercoin masternodes from one controller wallet.

Each Fibercoin masternode requires exactly **10,000 FBC** collateral.

## 1. Create the collateral addresses

Open the Fibercoin Qt wallet and create a separate receiving address for each masternode.

Send exactly **10,000 FBC** to each masternode collateral address.

Do not combine multiple masternode collateral amounts into one transaction output.

## 2. Generate masternode private keys

Open **Tools > Debug Console** and generate a separate masternode private key for each masternode:

    masternode genkey

Each masternode must use its own masternode private key.

The masternode private key is different from the wallet private key controlling the 10,000 FBC collateral.

Never place the collateral wallet private key on the remote masternode server.

## 3. Find the collateral outputs

In the controller wallet Debug Console, run:

    masternode outputs

Record the transaction ID and output index for each 10,000 FBC collateral output.

## 4. Configure masternode.conf

The default masternode configuration file is:

    masternode.conf

Each line contains five space-separated fields:

    alias IP:port masternodeprivkey collateral_output_txid collateral_output_index

Example:

    mn01 203.0.113.10:30114 MASTERNODE_PRIVATE_KEY TXID 0
    mn02 203.0.113.11:30114 MASTERNODE_PRIVATE_KEY TXID 1

Replace the example IP addresses, private keys, transaction IDs, and output indexes with your actual masternode information.

## 5. Choose the masternode hosting model

After preparing the controller configuration, masternodes can be hosted
using either legacy single-masternode daemons or a MultiMaster host.

### Option A: Legacy single-masternode host

Each masternode runs its own Fibercoin daemon.

The remote configuration contains the corresponding masternode private key:

    masternode=1
    masternodeprivkey=MASTERNODE_PRIVATE_KEY

Start or restart the Fibercoin daemon after updating the configuration.

Check the node with:

    fibercoin-cli getinfo

and:

    fibercoin-cli getmasternodestatus

### Option B: MultiMaster host

Multiple masternode identities can be maintained by one shared
`fibercoind` process.

Configure the host with:

    disablewallet=1
    multimaster=1
    txindex=1

Create `multimaster.conf` in the host data directory.

Each line uses:

    alias masternodeprivkey collateral_output_txid collateral_output_index

Example:

    mn01 MASTERNODE_PRIVATE_KEY TXID 0
    mn02 MASTERNODE_PRIVATE_KEY TXID 1

The `IP:port` does not need to be duplicated in this file. It is learned
from the masternode registration already broadcast by the controller.

The aliases, masternode private keys, transaction IDs, and output indexes
must correspond to the entries in the controller wallet's `masternode.conf`.

Check the host with:

    fibercoin-cli multimasterstatus

## 6. Verify the controller configuration

Open the controller wallet and use the Debug Console:

    masternode list-conf

Verify the alias, address, collateral transaction ID, and output index for every masternode.

## 7. Start the masternodes

Start one configured masternode:

    masternode start-alias mn01

Start all configured masternodes:

    masternode start-many

Additional supported start modes include:

    masternode start-all
    masternode start-missing
    masternode start-disabled

## MultiMaster startup behavior

The controller wallet still performs the initial masternode registration
using the collateral wallet.

After a masternode is registered, the MultiMaster host recognizes the
collateral VIN and maintains future masternode pings using the associated
masternode private key.

Restarting the MultiMaster host does not require restarting already
registered masternodes from the controller. Once synchronized, the host
resumes maintaining their pings automatically.

## Security

- Back up your wallet before changing masternode configuration.
- Keep wallet private keys offline and secure.
- Never place the private key controlling the 10,000 FBC collateral on the remote server.
- Use a unique masternode private key for every masternode.
