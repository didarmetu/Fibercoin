# Fibercoin Tor Support

Fibercoin can route network connections through a Tor SOCKS5 proxy.

Fibercoin Core v2.0.2.6 also contains legacy Tor onion-service support. However, the current networking implementation uses the obsolete Tor v2 onion-address format and should not be relied upon for modern Tor onion services.

## Using Tor as a SOCKS5 proxy

Run a Tor SOCKS5 proxy locally. A common Tor SOCKS port is:

    127.0.0.1:9050

Then start Fibercoin with:

    fibercoind -proxy=127.0.0.1:9050

The equivalent `fibercoin.conf` setting is:

    proxy=127.0.0.1:9050

When a proxy is configured, Fibercoin disables listening and local address discovery by default for privacy unless explicitly enabled.

## Using Tor with Fibercoin-Qt

The Fibercoin Qt wallet can use the same Tor SOCKS5 proxy.

Start Fibercoin-Qt with:

    fibercoin-qt -proxy=127.0.0.1:9050

On macOS, when using the application bundle directly:

    /Applications/Fibercoin-Qt.app/Contents/MacOS/Fibercoin-Qt -proxy=127.0.0.1:9050

To make the proxy configuration persistent, add the following line to `fibercoin.conf`:

    proxy=127.0.0.1:9050

The default configuration file locations are:

### Windows

    %APPDATA%\Fibercoin\fibercoin.conf

### macOS

    ~/Library/Application Support/Fibercoin/fibercoin.conf

### Linux

    ~/.fibercoin/fibercoin.conf

When `proxy` is configured, Fibercoin normally disables incoming listening and local address discovery automatically for privacy.

There is normally no need to add `onlynet=onion`. Fibercoin v2.0.2.6 has legacy direct onion-address support, so the recommended configuration for normal use is simply:

    proxy=127.0.0.1:9050

Make sure the Tor SOCKS5 proxy is running before starting Fibercoin.

## Tor-only outbound connections

Fibercoin recognizes both `tor` and `onion` as Tor network names.

To restrict outbound connections to the Tor network:

    fibercoind -proxy=127.0.0.1:9050 -onlynet=onion

However, direct `.onion` peer connectivity in Fibercoin v2.0.2.6 is limited by the legacy onion-address implementation described below.

## Separate Tor proxy

A separate SOCKS5 proxy can be configured for onion destinations:

    -onion=<ip:port>

Example:

    fibercoind -onion=127.0.0.1:9050

If `-onion` is not specified, Fibercoin normally uses the proxy configured with `-proxy`.

Onion connectivity can be disabled with:

    -noonion

or:

    -onion=0

## Listening and privacy

When `-proxy` is used, Fibercoin disables incoming listening by default.

Listening can be explicitly enabled with:

    -listen=1

Address discovery can be controlled with:

    -discover

A public address can be specified with:

    -externalip=<address>

Be aware that enabling listening or address discovery while using Tor can reduce privacy.

## Fibercoin network port

The default Fibercoin mainnet P2P port is:

    30114

The testnet P2P port is:

    16942

## Legacy onion-service support

Fibercoin contains Tor control support using:

    -listenonion
    -torcontrol=<ip:port>

The implementation can authenticate with a Tor control port and issue `ADD_ONION` automatically.

However, Fibercoin v2.0.2.6 represents onion addresses using the legacy Tor v2 format and requests legacy RSA1024 onion-service keys.

Tor v2 onion services are obsolete and are not supported by the modern Tor network.

For this reason, automatic Fibercoin onion-service creation and direct `.onion` peer operation should be considered legacy functionality in v2.0.2.6.

Do not use old 16-character `.onion` addresses from historical Fibercoin documentation.

## Modern Tor support

Full modern onion-service support requires Fibercoin networking changes to support Tor v3 addresses.

Until that work is completed and tested, Tor should primarily be used with Fibercoin as a SOCKS5 proxy for outbound IPv4/IPv6 connections.

## Checking network state

You can inspect Fibercoin's current network information with:

    fibercoin-cli getnetworkinfo

Check active peer connections with:

    fibercoin-cli getpeerinfo

## Verifying Tor proxy operation

Before starting Fibercoin, verify that the Tor SOCKS5 proxy is listening:

    nc -vz 127.0.0.1 9050

A successful connection confirms that a local service is accepting connections on the configured proxy port.

After starting Fibercoin, check the current network configuration:

    fibercoin-cli getnetworkinfo

When the proxy is active, the network entries should show:

    "proxy": "127.0.0.1:9050"

You can check active Fibercoin peer connections with:

    fibercoin-cli getconnectioncount
    fibercoin-cli getpeerinfo

When using the proxy, normal wallet connections should normally appear as outbound connections.

Tor connections may have higher latency and may take longer to establish than direct P2P connections.

If Fibercoin shows zero connections, first confirm that the Tor SOCKS5 proxy is still running and listening on port 9050.

## Security and privacy

Tor can improve network privacy, but using a proxy alone does not make every aspect of wallet or node activity anonymous.

For stronger privacy:

- keep Tor updated
- avoid exposing unnecessary listening ports
- avoid publishing identifying node information
- do not reuse sensitive RPC credentials
- keep RPC interfaces restricted to trusted systems
