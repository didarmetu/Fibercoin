Fibercoin Core for Windows
==========================

Fibercoin Core includes the graphical Fibercoin Qt wallet and command-line tools.

Included applications
---------------------

fibercoin-qt.exe
    Graphical Fibercoin wallet.

fibercoind.exe
    Headless Fibercoin daemon.

fibercoin-cli.exe
    Command-line RPC client for a running Fibercoin node.

fibercoin-tx.exe
    Transaction utility.

Getting started
---------------

1. Extract or install the Fibercoin package.

2. Start:

       fibercoin-qt.exe

3. Allow Fibercoin to synchronize with the network.

Initial synchronization may take some time depending on your computer,
network connection, and current blockchain state.

Default data directory
----------------------

Fibercoin stores its wallet, blockchain, configuration, and network data under:

    %APPDATA%\Fibercoin

Important files include:

    wallet.dat
    fibercoin.conf
    masternode.conf
    debug.log

Back up wallet.dat before upgrading or making major configuration changes.

Configuration
-------------

Optional settings can be placed in:

    %APPDATA%\Fibercoin\fibercoin.conf

For normal desktop use, no RPC password is required. Fibercoin can use
automatic cookie authentication for local RPC access.

Network
-------

The default Fibercoin mainnet P2P port is:

    30114

Fibercoin normally discovers and connects to peers automatically.

If synchronization does not start, check:

    fibercoin-cli.exe getconnectioncount
    fibercoin-cli.exe getblockcount
    fibercoin-cli.exe getnetworkinfo

Masternodes
-----------

Controller-wallet masternode configuration is stored in:

    %APPDATA%\Fibercoin\masternode.conf

Each masternode requires its own masternode private key and a valid 10,000 FBC
collateral configuration.

Do not place the wallet private key controlling the collateral on the remote
masternode server.

Tor proxy
---------

Fibercoin can use a local Tor SOCKS5 proxy.

Example fibercoin.conf setting:

    proxy=127.0.0.1:9050

Make sure the Tor SOCKS5 service is running before starting Fibercoin.

Support and resources
---------------------

Website:
    https://fibercoin.info/

Block explorer:
    https://explorer.fibercoin.info/

Releases:
    https://github.com/didarmetu/Fibercoin/releases

Discord:
    https://discord.gg/hUvXmJj

Fiberchange:
    https://fiberchange.com/

License
-------

Fibercoin Core is distributed under the MIT software license.

See COPYING for the complete license terms.
