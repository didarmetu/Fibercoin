# Fibercoin DNS Seed Operator Policy

DNS seeds help new Fibercoin nodes discover peers when they do not yet have enough addresses in their local peer database.

Fibercoin minimizes its dependence on DNS seeds by also using previously learned peers and compiled fixed seeds. However, DNS seeds remain part of the initial peer-discovery infrastructure and should therefore be operated carefully.

## Current Fibercoin DNS seed

The Fibercoin mainnet DNS seed configured in the client is:

    seed.fibercoin.info

The default Fibercoin mainnet P2P port is:

    30114

Testnet currently does not define a DNS seed in the client and instead uses compiled fixed seeds.

## Client behavior

DNS peer discovery is enabled by default when Fibercoin needs additional peer addresses.

It can be disabled with:

    -dnsseed=0

or in `fibercoin.conf`:

    dnsseed=0

DNS seeding can be forced with:

    -forcednsseed=1

When explicit `-connect` peers are configured, Fibercoin disables normal DNS seeding.

Fibercoin may skip querying DNS seeds when it already has enough usable peer addresses.

## Operator requirements

A Fibercoin DNS seed operator should follow these requirements.

### 1. Secure operation

The operator must maintain control of the DNS seed infrastructure and follow appropriate host, DNS, and network security practices.

Control of a Fibercoin DNS seed should not be transferred to an untrusted party.

Any hosting or DNS provider used by the operator should be secured appropriately.

### 2. Return functioning Fibercoin nodes

DNS seed responses should contain publicly reachable and functioning Fibercoin nodes to the best of the operator's ability.

Nodes should be selected fairly from the observed public Fibercoin network.

The seed must not intentionally direct users toward malicious, controlled, or unrelated hosts.

### 3. Do not target individual users

DNS responses may be randomized.

However, the seed should not intentionally provide different peer sets to selected users, networks, geographic regions, or organizations unless there is an urgent technical reason.

Any intentional filtering or unusual response behavior that materially affects peer discovery should be publicly disclosed.

### 4. DNS TTL

DNS seed responses should not use a TTL shorter than:

    60 seconds

Operators may use a longer TTL when appropriate.

### 5. Minimize DNS query logging

DNS query logs should be collected only when necessary for operating, securing, or diagnosing the seed service.

Query data should not be retained longer than necessary.

DNS query information should not be sold, shared, or used to profile Fibercoin users.

### 6. Node crawler data

A DNS seed normally requires a crawler or similar system to discover reachable Fibercoin nodes.

Information gathered directly from public Fibercoin nodes may be retained or published for network-health purposes.

The crawler should not manipulate its network connections in a way that biases the DNS results toward selected nodes.

### 7. Public contact

The DNS seed operator should provide a reachable contact address for:

- security reports
- incorrect seed results
- network problems
- abuse reports
- operational inquiries

### 8. Operational transparency

Operators are encouraged to document:

- crawler behavior
- node eligibility requirements
- refresh intervals
- DNS TTL policy
- filtering rules
- monitoring practices

This is not required for normal operation, but transparency makes the seed easier for the Fibercoin community to evaluate.

## Availability and resilience

DNS seeds are discovery helpers, not authoritative Fibercoin network servers.

Fibercoin nodes should remain capable of discovering and maintaining peers through other mechanisms, including:

- `peers.dat`
- fixed seeds
- addresses learned from connected peers
- manually configured peers

Operators should avoid creating unnecessary dependencies on a single DNS service.

## Security incidents

If a DNS seed is compromised or begins returning incorrect or malicious results, the operator should disable the affected service until control and correct operation are restored.

If an operator can no longer satisfy this policy, the seed should be discontinued and the Fibercoin maintainers should be notified so the client configuration can be updated.

## Reference implementation

The Bitcoin project's `bitcoin-seeder` software is an upstream reference implementation of a cryptocurrency DNS seed crawler:

    https://github.com/sipa/bitcoin-seeder

Fibercoin DNS seed deployments may use different software.

Any implementation used for Fibercoin should enforce the operational and security expectations described above.
