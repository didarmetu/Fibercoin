# Fibercoin Masternode Budget System

Fibercoin contains legacy decentralized budget and finalized-budget functionality.

As of Fibercoin Core v2.0.2.6, budget enforcement and superblock payments are not active on the mainnet by default.

The related RPC interfaces remain available for compatibility and development purposes.

## Budget RPCs

    mnbudget
    mnfinalbudget

Use RPC help for the commands supported by the current build:

    help mnbudget
    help mnfinalbudget

## Important

Do not create or submit budget proposals expecting mainnet payments unless budget and superblock functionality has been explicitly activated on the Fibercoin network.

The behavior of budget proposals, voting, finalization, and payments depends on the current network configuration and spork state.
