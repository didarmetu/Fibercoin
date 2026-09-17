# Fibercoin Unix and Ubuntu Build Guide

This guide describes the current Linux build process for Fibercoin Core.

The active Fibercoin Linux release workflows provide builds for:

    Ubuntu 18.04 x86_64
    Ubuntu 24.04 x86_64
    Ubuntu 26.04 x86_64

Ubuntu 24.04 is the current primary stable Linux release environment.

Ubuntu 18.04 is provided for older Fibercoin systems and existing masternode deployments.

Ubuntu 26.04 is provided for users on the newest Ubuntu LTS generation. The GitHub-hosted Ubuntu 26.04 runner is currently in public preview, so this build should be treated as an additional compatibility target until the runner reaches general availability.

## Build requirements

Install the packages used by the current Ubuntu build:

    sudo apt-get update

    sudo apt-get install -y \
      build-essential \
      libtool \
      autotools-dev \
      automake \
      autoconf \
      pkg-config \
      curl \
      file \
      libssl-dev \
      libevent-dev \
      libboost-system-dev \
      libboost-filesystem-dev \
      libboost-program-options-dev \
      libboost-thread-dev \
      libboost-chrono-dev \
      libboost-test-dev \
      libminiupnpc-dev \
      libzmq3-dev \
      qtbase5-dev \
      qttools5-dev \
      qttools5-dev-tools \
      libprotobuf-dev \
      protobuf-compiler \
      libqrencode-dev

Fibercoin v2.0.2.6 uses Qt 5.

Qt 6 migration is a separate future modernization project.

## Berkeley DB 4.8

Fibercoin wallet compatibility uses Berkeley DB 4.8.30.NC.

The current Ubuntu build compiles Berkeley DB locally rather than using a system Berkeley DB package.

Choose build and installation directories:

    BDB_SOURCE="$HOME/fibercoin-deps/bdb-source"
    BDB_PREFIX="$HOME/fibercoin-deps/bdb48"

    mkdir -p "$BDB_SOURCE" "$BDB_PREFIX"
    cd "$BDB_SOURCE"

Download Berkeley DB:

    curl --location --fail \
      --output db-4.8.30.NC.tar.gz \
      https://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz

Verify the archive:

    echo \
      "12edc0df75bf9abd7f82f821795bcee50f42cb2e5f76a6a281b85732798364ef db-4.8.30.NC.tar.gz" \
      | sha256sum -c -

Extract it:

    tar -xzf db-4.8.30.NC.tar.gz
    cd db-4.8.30.NC

Apply the compatibility changes used by the current build:

    sed -i \
      's/__atomic_compare_exchange/__atomic_compare_exchange_db/g' \
      dbinc/atomic.h

    sed -i \
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
      --with-pic \
      --disable-replication

    make -j"$(nproc)"
    make install

## Build Fibercoin

Return to the Fibercoin source directory.

Generate the build system:

    ./autogen.sh

Configure using the locally built Berkeley DB:

    BDB_PREFIX="$HOME/fibercoin-deps/bdb48"

    CPPFLAGS="-I$BDB_PREFIX/include" \
    LDFLAGS="-L$BDB_PREFIX/lib" \
    ./configure \
      --disable-tests \
      --with-gui=qt5 \
      --with-miniupnpc

Compile:

    make -j"$(nproc)"

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

Check the running daemon:

    ./src/fibercoin-cli getinfo

The default Linux data directory is:

    ~/.fibercoin

RPC cookie authentication is used automatically when an explicit RPC password is not configured.

## Tests

The current Ubuntu release workflow builds with:

    --disable-tests

For development builds, enable tests explicitly:

    CPPFLAGS="-I$BDB_PREFIX/include" \
    LDFLAGS="-L$BDB_PREFIX/lib" \
    ./configure \
      --enable-tests \
      --with-gui=qt5 \
      --with-miniupnpc

Then run:

    make check

See:

    doc/unit-tests.md

## Headless build

To build without the Qt GUI:

    CPPFLAGS="-I$BDB_PREFIX/include" \
    LDFLAGS="-L$BDB_PREFIX/lib" \
    ./configure \
      --disable-tests \
      --without-gui \
      --with-miniupnpc

Then:

    make -j"$(nproc)"

This is useful for servers and masternodes that only require `fibercoind` and command-line tools.

## Optional features

### MiniUPnPc

MiniUPnPc provides UPnP support.

It can be disabled with:

    ./configure --without-miniupnpc

### ZeroMQ

ZeroMQ support is available when `libzmq3-dev` is installed.

It can be disabled with:

    ./configure --disable-zmq

See:

    doc/zmq.md

### QR codes

The Qt wallet uses `libqrencode` for QR-code support.

Install:

    libqrencode-dev

before configuring the GUI build.

## Release packaging

The current Ubuntu release workflow creates a package containing:

    fibercoin-qt
    bin/fibercoind
    bin/fibercoin-cli
    bin/fibercoin-tx

The release binaries are stripped using:

    strip --strip-unneeded

The workflow audits runtime library dependencies with:

    ldd

and rejects packages with missing libraries.

## Version smoke tests

The release workflow verifies:

    fibercoind --version
    fibercoin-cli --version
    fibercoin-tx -?
    fibercoin-qt --version

For v2.0.2.6, the expected version is:

    2.0.2.6

## Release archive

The current Ubuntu workflow creates:

    Fibercoin-2.0.2.6-Ubuntu-24.04-x86_64.tar.gz

and a SHA-256 checksum file:

    Fibercoin-2.0.2.6-Ubuntu-24.04-x86_64.tar.gz.sha256

## Security

Fibercoin enables build hardening where supported by the compiler and platform.

Do not disable hardening for release builds unless there is a specific compatibility reason and the resulting binaries have been reviewed.

For server deployments:

- run `fibercoind` under a dedicated non-root user
- restrict RPC access
- protect wallet and configuration files
- expose the P2P port only when inbound connectivity is required
- keep the operating system and dependencies updated

## Current release environments

The active Fibercoin Linux release workflows are:

    .github/workflows/ubuntu-18.04-build.yml
    .github/workflows/ubuntu-build.yml
    .github/workflows/ubuntu-26.04-build.yml

These workflows are the authoritative references for the Ubuntu 18.04, Ubuntu 24.04, and Ubuntu 26.04 release builds.
