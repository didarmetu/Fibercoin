# Fibercoin Dependencies

Fibercoin Core requires several libraries for networking, wallet support, the Qt GUI, and optional features.

Exact dependency versions may vary by platform. The current GitHub Actions workflows and the `depends/` build system are the authoritative sources for release-build dependencies.

## Core dependencies

### Boost

Fibercoin uses Boost for filesystem, threading, networking, program options, chrono, and testing support.

The build system requires a C++11-compatible Boost installation.

### OpenSSL

Fibercoin supports modern OpenSSL, including OpenSSL 3.

OpenSSL provides cryptographic functionality used by the wallet, networking, and signature verification code.

### libevent

libevent is required for networking and event handling.

### Berkeley DB

Wallet builds use Berkeley DB **4.8** for wallet database compatibility.

The Ubuntu and macOS release workflows build Berkeley DB 4.8.30.NC explicitly rather than relying on the operating system's default Berkeley DB version.

Do not migrate an existing wallet to another Berkeley DB version without compatibility testing and backups.

## GUI dependencies

### Qt 5

Fibercoin v2.0.2.6 uses Qt 5 for the graphical wallet.

Qt 6 migration is not part of the v2.0.2.6 release.

### protobuf

Protocol Buffers is required by the Qt/payment-request components.

The macOS release workflow currently builds protobuf **3.20.3** explicitly for C++11 compatibility.

Other supported platforms may use their platform-provided protobuf package when compatible.

### qrencode

libqrencode provides QR-code support in the Qt wallet.

QR-code support is enabled when Qt and libqrencode are available.

## Optional dependencies

### MiniUPnPc

MiniUPnPc provides UPnP support.

It can be disabled at configure time:

    ./configure --without-miniupnpc

### ZeroMQ

ZeroMQ support is available for applications that use Fibercoin's ZMQ notification interface.

It is not required for basic wallet or daemon operation.

## Build tools

Building Fibercoin from source normally requires:

- a C++11-capable compiler
- autoconf
- automake
- libtool
- pkg-config
- make
- Python for selected build and test utilities

Platform-specific packages and additional tools are documented in the corresponding build instructions and CI workflows.

## Deterministic dependency builds

Fibercoin includes a `depends/` system containing pinned recipes for dependencies used by deterministic and cross-platform builds.

Important package recipes include:

    depends/packages/boost.mk
    depends/packages/openssl.mk
    depends/packages/libevent.mk
    depends/packages/bdb.mk
    depends/packages/miniupnpc.mk
    depends/packages/protobuf.mk
    depends/packages/qrencode.mk
    depends/packages/qt.mk
    depends/packages/zeromq.mk

The versions used by `depends/` may differ from native system packages used by GitHub Actions.

## Release build environments

Fibercoin currently maintains release workflows for:

- Ubuntu 24.04 x86-64
- Windows 11 x86-64
- macOS

Release builders should follow the repository's current CI workflows rather than relying on old dependency-version tables.

## Dependency modernization

Some dependencies are intentionally retained for compatibility with the existing Fibercoin codebase and wallet format.

Major dependency migrations, including Qt 6 and broader protobuf modernization, should be handled separately from maintenance releases and tested across all supported platforms.
