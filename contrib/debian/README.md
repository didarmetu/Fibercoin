
Debian
====================
This directory contains files used to package fibercoind/fibercoin-qt
for Debian-based Linux systems. If you compile fibercoind/fibercoin-qt yourself, there are some useful files here.

## fibercoin: URI support ##


fibercoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install fibercoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your fibercoinqt binary to `/usr/bin`
and the `../../share/pixmaps/fibercoin128.png` to `/usr/share/pixmaps`

fibercoin-qt.protocol (KDE)

