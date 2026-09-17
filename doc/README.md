# Fibercoin Documentation

This directory contains user, operator, developer, build, and release documentation for Fibercoin Core.

For the main project overview, see the repository root `README.md`.

## Getting Started

- [Blockchain Bootstrap](bootstrap.md) - Import trusted historical block data to speed up initial synchronization.
- [Data Directory and Files](files.md) - Reference for Fibercoin wallet, blockchain, network, and runtime files.
- [Tor Support](tor.md) - Configure Fibercoin to use a Tor SOCKS5 proxy.

## Masternodes

- [Masternode Configuration](masternode_conf.md) - Configure `masternode.conf` and manage Fibercoin masternodes.
- [Masternode Start-Many Guide](masternode-start-many.md) - Configure and start multiple masternodes from a controller wallet.
- [Masternode Budget System](masternode-budget.md) - Reference for the legacy Fibercoin budget subsystem.

## Network and Interfaces

- [SwiftTX](swifttx.md) - Fibercoin transaction-locking functionality.
- [ZeroMQ Notifications](zmq.md) - Subscribe to block, transaction, and SwiftTX events.
- [Unauthenticated REST Interface](REST-interface.md) - REST interface documentation.
- [DNS Seed Policy](dnsseed-policy.md) - Requirements and policy for Fibercoin DNS seed operators.

## Building Fibercoin

- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Dependencies](dependencies.md)
- [Gitian Building Guide](gitian-building.md)

## Running Servers

- [Running fibercoind as a System Service](init.md) - Run Fibercoin as a background service on a Linux server or VPS.

## Development

- [Developer Notes](developer-notes.md)
- [Translation Process](translation_process.md)
- [Unit Tests](unit-tests.md)

Source-code documentation can be generated locally with Doxygen:

    doxygen doc/Doxyfile

## Releases

- [Fibercoin Core v2.0.2.6 Release Notes](release-notes-2.0.2.6.md)
- [Release Process](release-process.md)

## Additional Documentation

- [Assets Attribution](assets-attribution.md)

## Platform Notes

### Windows

Fibercoin release packages provide the Fibercoin Qt wallet and command-line applications for supported Windows systems.

### macOS

Install Fibercoin from the macOS release package and launch Fibercoin-Qt normally.

### Linux and Unix

Fibercoin can run as the Qt wallet or as the headless `fibercoind` daemon.

See the [Unix Build Notes](build-unix.md) for source builds and the [system service guide](init.md) for server deployments.

## Community and Resources

- [Fibercoin Website](https://fibercoin.info/) - Official Fibercoin website with project information and resources.
- [Fibercoin Block Explorer](https://explorer.fibercoin.info/) - Official blockchain explorer for checking blocks, transactions, addresses, and network activity.
- [Fiberchange](https://fiberchange.com/) - Fibercoin exchange for trading FBC.
- [Fibercoin Discord](https://discord.gg/hUvXmJj) - Community server for Fibercoin discussions, support, and project updates.

## License

Fibercoin Core is distributed under the MIT software license.

See the repository `COPYING` file for the complete license terms.
