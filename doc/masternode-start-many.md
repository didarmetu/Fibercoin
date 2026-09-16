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

## 5. Configure the remote masternodes

Each remote masternode requires its own Fibercoin configuration.

The remote configuration should contain the corresponding masternode private key:

    masternode=1
    masternodeprivkey=MASTERNODE_PRIVATE_KEY

Start or restart the Fibercoin daemon after updating the configuration.

Check the remote node:

    fibercoin-cli getinfo

Confirm that the node is synchronized with the Fibercoin network.

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

## Security

- Back up your wallet before changing masternode configuration.
- Keep wallet private keys offline and secure.
- Never place the private key controlling the 10,000 FBC collateral on the remote server.
- Use a unique masternode private key for every masternode.
