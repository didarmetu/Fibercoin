[Unit]
Description=Fibercoin's distributed currency daemon
After=network.target

[Service]
User=fibercoin
Group=fibercoin

Type=forking
PIDFile=/var/lib/fibercoind/fibercoind.pid

ExecStart=/usr/bin/fibercoind -daemon -pid=/var/lib/fibercoind/fibercoind.pid \
          -conf=/etc/fibercoin/fibercoin.conf -datadir=/var/lib/fibercoind

ExecStop=-/usr/bin/fibercoin-cli -conf=/etc/fibercoin/fibercoin.conf \
         -datadir=/var/lib/fibercoind stop

Restart=always
PrivateTmp=true
TimeoutStopSec=60s
TimeoutStartSec=2s
StartLimitInterval=120s
StartLimitBurst=5

[Install]
WantedBy=multi-user.target
