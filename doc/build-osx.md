# Fibercoin macOS Build Guide

This guide describes the current macOS build environment for Fibercoin Core.

The release build process supports:

- Apple Silicon (`arm64`)
- Intel (`x86_64`)

The current release workflow builds on macOS 15.

## Requirements

Install Apple's Xcode Command Line Tools:

    xcode-select --install

Install Homebrew if it is not already available.

The build requires a C++11-capable compiler and the standard GNU/Autotools build tools.

## Homebrew dependencies

Install the dependencies used by the current Fibercoin macOS build:

    brew update

    brew install \
      autoconf \
      automake \
      libtool \
      pkg-config \
      boost \
      openssl@3 \
      libevent \
      zeromq \
      miniupnpc \
      qt@5 \
      qrencode \
      dylibbundler

Fibercoin v2.0.2.6 uses Qt 5.

Qt 6 migration is a separate future modernization project.

## Protobuf

The current macOS release build uses protobuf 3.20.3.

A known-compatible static protobuf build can be created with:

    PROTOBUF_VERSION="3.20.3"
    PROTOBUF_PREFIX="$HOME/fibercoin-deps/protobuf"

    mkdir -p "$HOME/fibercoin-deps/src" "$PROTOBUF_PREFIX"
    cd "$HOME/fibercoin-deps/src"

    curl --location --fail \
      --output "protobuf-cpp-${PROTOBUF_VERSION}.tar.gz" \
      "https://github.com/protocolbuffers/protobuf/releases/download/v${PROTOBUF_VERSION}/protobuf-cpp-${PROTOBUF_VERSION}.tar.gz"

    tar -xzf "protobuf-cpp-${PROTOBUF_VERSION}.tar.gz"
    cd "protobuf-${PROTOBUF_VERSION}"

    ./configure \
      --prefix="$PROTOBUF_PREFIX" \
      --disable-shared \
      --enable-static \
      CXXFLAGS="-std=c++11"

    make -j"$(sysctl -n hw.logicalcpu)"
    make install

## Berkeley DB 4.8

Fibercoin wallet compatibility uses Berkeley DB 4.8.30.NC.

The current macOS build applies compatibility changes required by modern Apple compilers.

Set a local installation prefix:

    BDB_PREFIX="$HOME/fibercoin-deps/bdb48"

Download and verify Berkeley DB:

    mkdir -p "$HOME/fibercoin-deps/src" "$BDB_PREFIX"
    cd "$HOME/fibercoin-deps/src"

    curl --location --fail \
      --output db-4.8.30.NC.tar.gz \
      https://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz

    echo "12edc0df75bf9abd7f82f821795bcee50f42cb2e5f76a6a281b85732798364ef  db-4.8.30.NC.tar.gz" \
      | shasum -a 256 -c -

Extract it:

    tar -xzf db-4.8.30.NC.tar.gz
    cd db-4.8.30.NC

Apply the compatibility changes:

    sed -i.bak \
      's/__atomic_compare_exchange/__atomic_compare_exchange_db/g' \
      dbinc/atomic.h

    sed -i.bak \
      's/atomic_init/atomic_init_db/g' \
      dbinc/atomic.h \
      mp/mp_region.c \
      mp/mp_mvcc.c \
      mp/mp_fget.c \
      mutex/mut_method.c \
      mutex/mut_tas.c

Build Berkeley DB:

    cd build_unix

    CFLAGS="-Wno-error=implicit-function-declaration" \
      ../dist/configure \
      --prefix="$BDB_PREFIX" \
      --enable-cxx \
      --disable-shared \
      --disable-replication

    make -j"$(sysctl -n hw.logicalcpu)"
    make install

## Configure the build environment

Return to the Fibercoin source directory.

Set the dependency paths:

    BREW_PREFIX="$(brew --prefix)"
    QT_PREFIX="$(brew --prefix qt@5)"
    OPENSSL_PREFIX="$(brew --prefix openssl@3)"
    PROTOBUF_PREFIX="$HOME/fibercoin-deps/protobuf"
    BDB_PREFIX="$HOME/fibercoin-deps/bdb48"

    export PATH="$QT_PREFIX/bin:$PROTOBUF_PREFIX/bin:$PATH"

    export CPPFLAGS="-I$PROTOBUF_PREFIX/include -I$OPENSSL_PREFIX/include -I$BDB_PREFIX/include -I$BREW_PREFIX/include"

    export LDFLAGS="-L$PROTOBUF_PREFIX/lib -L$OPENSSL_PREFIX/lib -L$BDB_PREFIX/lib -L$BREW_PREFIX/lib"

    export PKG_CONFIG_PATH="$QT_PREFIX/lib/pkgconfig:$OPENSSL_PREFIX/lib/pkgconfig:$PROTOBUF_PREFIX/lib/pkgconfig:$BREW_PREFIX/lib/pkgconfig"

For builds matching the current release workflow:

    export MACOSX_DEPLOYMENT_TARGET=15.0

## Build Fibercoin

Generate the build system:

    ./autogen.sh

Configure:

    ./configure \
      --disable-tests \
      --with-gui=qt5 \
      --with-qtdbus=no \
      --with-boost="$(brew --prefix boost)" \
      --with-boost-libdir="$(brew --prefix boost)/lib" \
      --with-miniupnpc

Compile:

    make -j"$(sysctl -n hw.logicalcpu)"

The primary binaries are:

    src/fibercoind
    src/fibercoin-cli
    src/fibercoin-tx
    src/qt/fibercoin-qt

## Running Fibercoin

Run the Qt wallet:

    ./src/qt/fibercoin-qt

Run the daemon:

    ./src/fibercoind -daemon

Use the command-line client:

    ./src/fibercoin-cli getinfo

The default macOS data directory is:

    ~/Library/Application Support/Fibercoin

RPC cookie authentication is used automatically when an explicit RPC password is not configured.

## Tests

The current macOS release workflow builds with:

    --disable-tests

For development builds, tests can be enabled separately:

    ./configure --enable-tests ...

and run with:

    make check

See:

    doc/unit-tests.md

## Portable macOS packages

The release workflow builds separate `arm64` and `x86_64` packages.

The Qt application bundle contains:

    Fibercoin-Qt.app

Command-line binaries are packaged separately under:

    bin/fibercoind
    bin/fibercoin-cli
    bin/fibercoin-tx

Qt frameworks and other dynamic libraries are bundled into the release package so the final application does not depend on Homebrew paths on the destination computer.

The release workflow audits Mach-O dependencies with `otool` and rejects packages that still reference Homebrew installation paths.

## Universal builds

The release workflow combines the separately built Apple Silicon and Intel packages into a portable universal macOS package.

Local developers normally do not need to reproduce the universal packaging process manually.

For the exact release packaging procedure, see:

    .github/workflows/macos-build.yml

## Platform support

Fibercoin v2.0.2.6 currently targets macOS 15 for its release workflow.

Intel macOS support depends on the availability of a compatible Intel build environment and should be reviewed separately for future releases.
