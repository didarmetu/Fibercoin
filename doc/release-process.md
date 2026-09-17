# Fibercoin Release Process

This document describes the current Fibercoin Core release process.

The active release build system uses GitHub Actions for:

- Ubuntu 18.04 x86-64
- Ubuntu 24.04 x86-64
- Ubuntu 26.04 x86-64
- Windows 11 x86-64
- macOS Apple Silicon (`arm64`)
- macOS Intel (`x86_64`)
- macOS universal packaging

Legacy Gitian infrastructure remains in the repository for historical and deterministic-build reference, but it is not the primary v2.0.2.6 release pipeline.

## 1. Prepare the release branch

Before creating a release, make sure the release branch contains only the intended changes.

Check the working tree:

    git status

Review recent commits:

    git log --oneline --decorate -20

The working tree should be clean before final validation.

## 2. Update the version

Update the Fibercoin version in:

    configure.ac

For a final release, make sure:

    CLIENT_VERSION_IS_RELEASE

is enabled appropriately.

Also verify any version values used by release packaging and CI workflows.

For v2.0.2.6, the expected client version is:

    2.0.2.6

## 3. Update release notes

Create or update the release-specific notes:

    doc/release-notes-2.0.2.6.md

The release notes should describe user-visible changes, compatibility information, and upgrade instructions.

Do not include unverified changes.

## 4. Update translations

Review the translation workflow described in:

    doc/translation_process.md

When user-visible strings have changed, regenerate the translation sources:

    cd src
    make translate

Review the resulting changes before committing them.

## 5. Review network and release-critical configuration

Before release, review:

- checkpoints
- fixed seeds
- DNS seeds
- default network ports
- protocol version
- masternode configuration
- spork configuration
- release version values

For major releases, also review any estimated blockchain-size values and chain statistics used by the GUI.

## 6. Push the release candidate branch

Push the release branch to GitHub so the build workflows can run:

    git push origin <branch>

For the v2.0.2.6 preparation branch:

    git push origin upgrade/build-modernization

## 7. Run GitHub Actions builds

The active workflows are:

    .github/workflows/ubuntu-18.04-build.yml
    .github/workflows/ubuntu-build.yml
    .github/workflows/ubuntu-26.04-build.yml
    .github/workflows/windows-build.yml
    .github/workflows/macos-build.yml

They can run automatically on the configured branch or manually through GitHub Actions using `workflow_dispatch`.

All release workflows must complete successfully before tagging the release.

## 8. Validate Ubuntu artifacts

The active Ubuntu release workflows provide:

    Ubuntu 18.04 x86-64
    Ubuntu 24.04 x86-64
    Ubuntu 26.04 x86-64

The release artifacts are:

    fibercoin-Ubuntu-18.04-x86_64
    fibercoin-Ubuntu-24.04-x86_64
    fibercoin-Ubuntu-26.04-x86_64

Build environments:

- Ubuntu 18.04 is built inside an `ubuntu:18.04` Docker userspace running on a GitHub-hosted Ubuntu 24.04 runner.
- Ubuntu 24.04 is built directly on the GitHub-hosted Ubuntu 24.04 runner.
- Ubuntu 26.04 is built directly on the GitHub-hosted Ubuntu 26.04 runner.

Diagnostic artifacts are also available for failed or suspicious builds.

For each Ubuntu release artifact, confirm that the expected binaries are present and executable.

At minimum, validate:

    fibercoind
    fibercoin-cli
    fibercoin-tx
    fibercoin-qt

Run basic version checks on each produced package.

For v2.0.2.6, confirm:

    fibercoind --version
    fibercoin-cli --version

report version:

    2.0.2.6

Also verify that the packaged binaries start without missing-library errors on their target Ubuntu version.

## 9. Validate Windows artifacts

The Windows workflow is:

    Windows 11 x86-64 Build

The release artifact is published as:

    fibercoin-Windows-11-x86_64

The diagnostic artifact is:

    fibercoin-windows-11-x86_64-diagnostics

Verify that the Windows package contains the expected executables and that Fibercoin-Qt launches successfully.

Perform a basic wallet and network smoke test on a supported Windows system before release.

## 10. Validate macOS artifacts

The macOS workflow is:

    macOS Universal Build

It builds separate packages for:

    arm64
    x86_64

and combines them into a universal package.

Architecture artifacts include:

    fibercoin-macos-arm64
    fibercoin-macos-x86_64

The final universal release artifact is:

    fibercoin-macOS-universal-unsigned

The macOS workflow also audits runtime dependencies to ensure the portable package does not retain Homebrew library paths.

Verify that the final application bundle and command-line binaries run without depending on the build machine's Homebrew installation.

## 11. Final binary validation

Release validation must be performed on the actual built artifacts, not only on locally compiled binaries.

Check the reported version:

    fibercoin-cli --version
    fibercoind --version

Confirm that the version is:

    v2.0.2.6

Perform a clean restart and verify:

    fibercoin-cli getblockcount
    fibercoin-cli getbestblockhash
    fibercoin-cli getconnectioncount
    fibercoin-cli getnetworkinfo

Compare the final chain height and best block hash with a trusted established Fibercoin node.

## 12. Masternode and spork validation

For v2.0.2.6, verify masternode synchronization:

    fibercoin-cli mnsync status

Verify the masternode list:

    fibercoin-cli listmasternodes

Verify current sporks:

    fibercoin-cli spork show
    fibercoin-cli spork active

Confirm:

- masternode synchronization completes
- SPORK_8 masternode payment enforcement remains active
- deprecated sporks 7, 11, and 12 behave as expected
- deprecated-spork administrative updates are rejected
- masternode payment validation continues normally

Monitor several blocks after restart before final release.

## 13. Wallet smoke tests

Using the built release artifacts, test basic wallet operation.

At minimum verify:

- wallet startup
- wallet unlock and lock
- receive address generation
- transaction creation
- transaction history
- RPC connectivity
- blockchain synchronization
- normal shutdown and restart

Where appropriate, also test staking and masternode controller functionality.

Use test funds or controlled release-validation wallets when possible.

## 14. Verify clean shutdown and restart

Shut down Fibercoin normally.

Restart using the same data directory.

Confirm that:

- the node starts without database errors
- chainstate loads correctly
- peers reconnect
- masternode synchronization resumes
- the wallet loads correctly
- the node remains on the expected best block

The release includes improved recovery after unclean shutdown, but a normal release validation should still include clean shutdown testing.

## 15. Review documentation

Before tagging, run:

    git diff --check

Check documentation links and remove stale references.

Review at minimum:

    doc/README.md
    doc/release-notes-2.0.2.6.md
    doc/build-osx.md
    doc/build-unix.md
    doc/dependencies.md
    doc/init.md
    doc/files.md
    doc/tor.md
    doc/masternode_conf.md

## 16. Generate checksums

After selecting the final release artifacts, generate SHA-256 checksums.

On Linux:

    sha256sum <release-files> > SHA256SUMS

On macOS:

    shasum -a 256 <release-files> > SHA256SUMS

Review the checksum file carefully before publishing it.

If release signing is used, sign the checksum file with the appropriate release key.

## 17. Tag the tested commit

Only tag the exact commit whose artifacts were validated.

Example:

    git tag -s v2.0.2.6

Verify the tag:

    git show v2.0.2.6

Then push it:

    git push origin v2.0.2.6

Do not create the final tag before artifact and network validation is complete.

## 18. Create the GitHub release

Create the release at:

    https://github.com/didarmetu/Fibercoin/releases

Use the tested tag:

    v2.0.2.6

Attach the validated release artifacts and checksum file.

Use:

    doc/release-notes-2.0.2.6.md

as the basis for the GitHub release description.

## 19. Post-release validation

After publishing the release:

- download the public release artifacts
- verify their checksums
- install or extract them normally
- confirm the reported version
- connect to the Fibercoin network
- verify blockchain synchronization
- verify masternode synchronization
- verify current spork state
- verify the published files match the previously tested artifacts

## Legacy Gitian infrastructure

Fibercoin still contains historical Gitian infrastructure under:

    contrib/gitian-build.sh
    contrib/gitian-descriptors/
    doc/gitian-building.md

These files are retained for deterministic-build reference and possible future modernization.

They are not the primary release path for Fibercoin Core v2.0.2.6.

Do not follow the old macOS SDK, detached-signature, or Gitian multi-builder instructions as the active v2.0.2.6 release procedure unless the Gitian pipeline has been separately restored and validated.
