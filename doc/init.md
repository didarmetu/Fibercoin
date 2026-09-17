# Running fibercoind as a System Service

Fibercoin can run as a background service on a Linux server or VPS.

This is useful for public nodes, masternodes, and other systems that should start `fibercoind` automatically at boot.

Sample service files are available under:

    contrib/init/

## Dedicated service user

For a server installation, run `fibercoind` under a dedicated non-root user such as:

    fibercoin

The Fibercoin configuration and data directories should be owned by that user.

Do not run `fibercoind` as root unless there is a specific administrative reason to do so.

## Configuration

A server installation can use:

    /etc/fibercoin/fibercoin.conf

with its blockchain and wallet data stored separately, for example:

    /var/lib/fibercoind

The exact paths depend on the service configuration being used.

Protect configuration and wallet data from other system users.

For example:

    chmod 600 /etc/fibercoin/fibercoin.conf

## RPC authentication

Fibercoin supports automatic RPC cookie authentication.

When `rpcpassword` is not configured, Fibercoin generates a random authentication cookie at startup.

The default cookie file is:

    .cookie

inside the Fibercoin data directory.

The cookie is removed when Fibercoin shuts down normally.

A custom cookie location can be selected with:

    -rpccookiefile=<file>

Explicit RPC credentials can also be configured when required:

    rpcuser=USERNAME
    rpcpassword=STRONG_RANDOM_PASSWORD

Do not expose the RPC interface directly to untrusted networks.

## systemd

The supplied systemd service template is:

    contrib/init/fibercoind.service.app

Review the service file before installation and adjust the executable, configuration, user, group, and data-directory paths for your system.

Review the template and create the final systemd unit as:

    /etc/systemd/system/fibercoind.service

Adjust any paths or placeholders in the template for your installation before enabling the service.

Then reload systemd:

    sudo systemctl daemon-reload

Start Fibercoin:

    sudo systemctl start fibercoind

Check its status:

    sudo systemctl status fibercoind

Enable automatic startup at boot:

    sudo systemctl enable fibercoind

Stop the service:

    sudo systemctl stop fibercoind

Restart the service:

    sudo systemctl restart fibercoind

View recent service logs with:

    journalctl -u fibercoind

## OpenRC and legacy service scripts

Additional service scripts may be available under:

    contrib/init/

These can include OpenRC, Upstart, or SysV-style configurations.

Use them only on systems that still use the corresponding service manager.

For modern Linux distributions using systemd, the systemd service unit is the preferred starting point.

## Masternodes

A Fibercoin masternode can also run under the system service.

Typical masternode configuration includes:

    masternode=1
    masternodeprivkey=MASTERNODE_PRIVATE_KEY

The default Fibercoin mainnet P2P port is:

    30114

A public masternode must be reachable by other Fibercoin nodes on its configured P2P address and port.

Do not place the wallet private key controlling the 10,000 FBC collateral on the remote masternode server.

## Checking the node

After starting the service, check the node with:

    fibercoin-cli getinfo
    fibercoin-cli getconnectioncount
    fibercoin-cli getblockcount

For a masternode:

    fibercoin-cli getmasternodestatus

Check synchronization before relying on node or masternode services.

## Security

For server installations:

- run Fibercoin under a dedicated non-root user
- restrict permissions on wallet and configuration files
- keep RPC access restricted to trusted systems
- use firewall rules appropriate for the node
- expose the P2P port only when inbound connectivity is required
- back up wallet data before upgrades
- shut down Fibercoin cleanly before replacing binaries
