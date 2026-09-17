# Fibercoin Gitian Builds

Gitian is a reproducible build system historically used by Fibercoin and other Bitcoin-derived projects.

It allows multiple independent builders to compile the same source code in controlled build environments and compare the resulting outputs.

When independently produced binaries match, builders can cryptographically sign build assertions. This provides additional assurance that published binaries correspond to the reviewed source code.

## Current status

Gitian is **not the primary release build system for Fibercoin Core v2.0.2.6**.

The active Fibercoin release builds use GitHub Actions.

Current release workflows include:

    .github/workflows/ubuntu-18.04-build.yml
    .github/workflows/ubuntu-build.yml
    .github/workflows/ubuntu-26.04-build.yml
    .github/workflows/windows-build.yml
    .github/workflows/macos-build.yml

See:

    doc/release-process.md

for the current release procedure.

## Legacy infrastructure

Fibercoin retains Gitian-related infrastructure under:

    contrib/gitian-build.sh
    contrib/gitian-descriptors/

This infrastructure is retained for historical reference, reproducible-build research, and possible future modernization.

It should not be assumed to work with current operating systems, dependencies, compilers, or Fibercoin release procedures without additional validation.

## Historical build model

The traditional Gitian process used build descriptors that specified:

- source revisions
- build environments
- dependencies
- compilation commands
- packaging commands

Multiple builders could independently execute the same descriptor.

Conceptually:

    Fibercoin source
          |
          +-- Builder A --> build result
          |
          +-- Builder B --> build result
          |
          +-- Builder C --> build result
                              |
                              v
                       compare results

Matching results could then be accompanied by cryptographic build assertions.

## Gitian descriptors

Historical Fibercoin descriptors are stored under:

    contrib/gitian-descriptors/

Depending on the retained infrastructure, descriptors may exist for Linux, Windows, macOS, signing, or additional architectures.

These descriptors represent the historical deterministic-build system and are separate from the current GitHub Actions workflows.

## Reproducible builds and GitHub Actions

GitHub Actions provides automated and controlled CI and release builds, but this does not by itself provide the same guarantee as independent reproducible Gitian builds.

The current Fibercoin release process focuses on:

- automated platform builds
- controlled dependency configuration
- runtime-library auditing
- executable smoke tests
- release artifact validation
- SHA-256 checksums

Reintroducing fully reproducible independent builds would require separately modernizing and validating the deterministic build infrastructure.

## Do not use the historical setup instructions

Older Fibercoin Gitian documentation relied on obsolete environments and tooling, including old Debian and Ubuntu releases, historical LXC configurations, old macOS SDK cross-compilation procedures, and deprecated dependency sources.

Those instructions are no longer the supported Fibercoin release procedure.

Do not use historical Gitian-generated binaries for a current release unless the complete build pipeline has been independently reviewed and validated.

## Future modernization

If Gitian or another reproducible-build system is restored, the work should include:

1. updating build environments
2. auditing all dependency sources and checksums
3. updating platform toolchains
4. validating the current Fibercoin source tree
5. verifying reproducibility across independent builders
6. documenting signing and verification procedures
7. integrating reproducibility checks with the current release process

Until that work is completed, Gitian should be considered legacy Fibercoin build infrastructure.
