# Fibercoin SwiftTX

SwiftTX provides transaction locking through the Fibercoin masternode network.

A successfully locked transaction can be treated by wallet RPCs as having additional confirmation depth before normal blockchain confirmations accumulate.

## SwiftTX depth

The default SwiftTX confirmation depth is:

    5

It can be configured with:

    -swifttxdepth=<n>

Fibercoin currently limits this value to the range `0` through `60`.

If SwiftTX is disabled, the SwiftTX confirmation depth is set to zero.

## RPC confirmations

Wallet RPC results can contain both:

    confirmations
    bcconfirmations

For a successfully locked SwiftTX transaction, `confirmations` includes the configured SwiftTX depth in addition to blockchain confirmations.

`bcconfirmations` reports only actual blockchain confirmations.

With the default SwiftTX depth of 5:

### Newly locked SwiftTX transaction

    confirmations: 5
    bcconfirmations: 0

### SwiftTX transaction with one blockchain confirmation

    confirmations: 6
    bcconfirmations: 1

### Normal transaction with no blockchain confirmations

    confirmations: 0
    bcconfirmations: 0

### Normal transaction with one blockchain confirmation

    confirmations: 1
    bcconfirmations: 1

## ZMQ notifications

Fibercoin can publish SwiftTX transaction-lock notifications through ZeroMQ.

Publish the transaction hash when a SwiftTX lock is completed:

    -zmqpubhashtxlock=<address>

Publish the raw locked transaction:

    -zmqpubrawtxlock=<address>

These options require Fibercoin to be built with ZeroMQ support.

## Network operation

SwiftTX uses transaction-lock requests and masternode consensus votes.

The related network message types include transaction-lock requests and transaction-lock votes.

SwiftTX behavior can also depend on the current network spork state.

Check the current spork values with:

    fibercoin-cli spork show

and their active state with:

    fibercoin-cli spork active

## Configuration

SwiftTX-related options can be supplied on the command line or in `fibercoin.conf`.

Use the current executable help output to see the options supported by your build:

    fibercoind --help
