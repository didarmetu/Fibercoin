# Fibercoin Unauthenticated REST Interface

Fibercoin provides a small read-only REST interface for accessing transaction and block data.

The REST interface is disabled by default.

Enable it with:

    fibercoind -rest

or add the following to `fibercoin.conf`:

    rest=1

## Important security note

The REST interface is unauthenticated.

Do not expose the Fibercoin HTTP/RPC listener to untrusted networks unless you understand the security and privacy implications.

REST access should normally be restricted to trusted systems or localhost.

## Supported output formats

REST endpoints support the following output formats:

    .bin
    .hex
    .json

The requested format is appended to the object hash.

Examples:

    /rest/tx/TX_HASH.json
    /rest/block/BLOCK_HASH.hex

## Transaction endpoint

Retrieve a transaction by transaction ID:

    GET /rest/tx/TX_HASH.bin
    GET /rest/tx/TX_HASH.hex
    GET /rest/tx/TX_HASH.json

`TX_HASH` must be a valid 64-character hexadecimal transaction hash.

Example:

    curl http://127.0.0.1:33114/rest/tx/TX_HASH.json

If the transaction cannot be found, Fibercoin returns an HTTP 404 response.

For complete historical transaction lookup, enabling the transaction index may be necessary:

    txindex=1

Changing `txindex` may require rebuilding or reindexing blockchain data before historical transactions become available through the index.

## Block endpoint

Retrieve a block by block hash:

    GET /rest/block/BLOCK_HASH.bin
    GET /rest/block/BLOCK_HASH.hex
    GET /rest/block/BLOCK_HASH.json

`BLOCK_HASH` must be a valid 64-character hexadecimal block hash.

Example:

    curl http://127.0.0.1:33114/rest/block/BLOCK_HASH.json

The JSON form includes transaction details.

## Block endpoint without transaction details

For a smaller JSON response containing transaction hashes instead of complete transaction details, use:

    GET /rest/block/notxdetails/BLOCK_HASH.json

Example:

    curl http://127.0.0.1:33114/rest/block/notxdetails/BLOCK_HASH.json

The `notxdetails` form affects JSON output only.

Binary and hexadecimal block serialization remain based on the complete block data.

## HTTP status responses

Typical responses include:

    200 OK

The requested transaction or block was found.

    400 Bad Request

The supplied hash is malformed.

    404 Not Found

The requested transaction, block, or output format was not found.

    503 Service Unavailable

Fibercoin is temporarily unable to service the request, such as during certain startup or synchronization states.

## RPC and HTTP configuration

REST uses Fibercoin's HTTP/RPC server infrastructure.

Relevant options include:

    -rest
    -rpcbind=<address>
    -rpcallowip=<address>
    -rpcport=<port>
    -rpcthreads=<n>
    -rpckeepalive

The default Fibercoin mainnet RPC port is:

    33114

The testnet RPC port is:

    169422

Be especially careful when changing `rpcbind` or `rpcallowip`, because doing so can make REST and RPC services reachable from other systems.

## Local example

Start Fibercoin with REST enabled:

    fibercoind -rest

Then request network data locally.

Example block request:

    curl http://127.0.0.1:33114/rest/block/BLOCK_HASH.json

Example transaction request:

    curl http://127.0.0.1:33114/rest/tx/TX_HASH.json

Replace `BLOCK_HASH` and `TX_HASH` with actual Fibercoin hashes.

## Privacy considerations

A REST-enabled node can reveal blockchain and transaction information to any client that can reach the HTTP service.

Running a web browser on the same machine as an exposed REST service can also create privacy risks if malicious web content is able to make requests to the local service.

For normal local administration:

- bind RPC/REST access only where needed
- avoid exposing the RPC port directly to the public Internet
- use firewall rules to restrict access
- keep REST disabled when it is not required

## REST versus JSON-RPC

The REST interface is intended for simple unauthenticated read access to selected blockchain objects.

For administrative operations, wallet operations, node control, or the full RPC command set, use `fibercoin-cli` or the authenticated JSON-RPC interface instead.
