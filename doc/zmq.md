# Fibercoin ZeroMQ Notifications

Fibercoin can publish block, transaction, and SwiftTX events through ZeroMQ.

This allows external software to subscribe to node events without repeatedly polling RPC.

ZeroMQ notifications are read-only and unauthenticated. Subscribers should validate received data and should only connect through trusted or firewall-protected endpoints.

## Build support

Fibercoin requires ZeroMQ API version 4.x or newer.

On Linux, the development package is commonly named:

    libzmq3-dev

ZeroMQ support is enabled automatically when a compatible library is found.

To disable it when configuring Fibercoin:

    ./configure --disable-zmq

## Supported notifications

Fibercoin currently supports:

    -zmqpubhashblock=<address>
    -zmqpubhashtx=<address>
    -zmqpubhashtxlock=<address>
    -zmqpubrawblock=<address>
    -zmqpubrawtx=<address>
    -zmqpubrawtxlock=<address>

The SwiftTX-specific notifications are:

    -zmqpubhashtxlock=<address>
    -zmqpubrawtxlock=<address>

## Example

Publish transaction hashes and raw transactions over TCP:

    fibercoind \
      -zmqpubhashtx=tcp://127.0.0.1:28332 \
      -zmqpubrawtx=tcp://127.0.0.1:28333

The same options can be placed in `fibercoin.conf`.

Example:

    zmqpubhashtx=tcp://127.0.0.1:28332
    zmqpubrawtx=tcp://127.0.0.1:28333

## Message format

Each ZeroMQ notification is sent as three message parts:

1. topic
2. payload
3. 4-byte little-endian sequence number

For example, `-zmqpubhashtx` uses the topic:

    hashtx

and sends the 32-byte transaction hash as the payload.

The sequence number increments for each published message and can be used by subscribers to detect missed notifications.

## SwiftTX topics

SwiftTX transaction-lock notifications use these topics:

    hashtxlock
    rawtxlock

`hashtxlock` publishes the transaction hash.

`rawtxlock` publishes the serialized transaction.

## Subscriber behavior

ZeroMQ PUB sockets do not provide authentication or request/response behavior.

Subscribers should:

- validate incoming block and transaction data
- handle reconnects
- detect gaps using the sequence number
- retrieve missing blockchain data through RPC when necessary

A subscriber must set a ZeroMQ subscription prefix before messages will be delivered.

## Security

Do not expose ZeroMQ endpoints directly to untrusted networks unless protected by firewalling or another trusted transport layer.

Fibercoin does not authenticate ZeroMQ subscribers.

## Example subscriber

If available in the source tree, the example subscriber can be found at:

    contrib/zmq/zmq_sub.py
