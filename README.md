Sysinfo
=========

**A tool to display theme and system information to be used in screenshots.**

This little application will print information about your system and your theme, along with some nice ASCII art when run.

**NOTE:** This application is currently written for Arch Linux; the ASCII art is the Arch Linux logo and the package count only works with Pacman. I will accept pull requests that enable supports for other distributions. Sysinfo is made for lightweight desktops; it only checks `~/.config/gtk-3.0/settings.ini` and `~/.gtkrc-2.0` for its theme retrieval. Sorry KDE users!

Currently, it prints the following:
* Shell;
* Username;
* Hostname;
* Kernel version and OS type (64 or 32 bit);
* Amount of installed packages[1];
* Window manager;
* GTK theme;
* Icon set (GTK);
* Font (GTK);
* Terminal colors[2].

[1]: This is an optional feature and only works with Pacman.

[2]: This is an optional feature and requires the path to a script that prints colors. An example script can be found below:

	#!/bin/bash

	colors=($(xrdb -query | sed -n 's/.*color\([0-9]\)/\1/p' | sort -nu | cut -f2))

	echo -e "\e[1;37m
	 Black      Red        Green      Yellow     Blue       Magenta    Cyan       White
	───────────────────────────────────────────────────────────────────────────────────────\e[0m"
	for i in {0..7}; do echo -en "\e[$((30+$i))m █ ${colors[i]} \e[0m"; done
	echo
	for i in {8..15}; do echo -en "\e[1;$((22+$i))m █ ${colors[i]} \e[0m"; done
	echo -e "\n"

Supported Window Managers and Desktop Environments
--------------------------------------------------

Window Managers are retrieved by first trying to read `_NET_WM_NAME` on the root window. If this fails, a fallback is run which uses pgrep.

The following Window Managers are supported:

* Fluxbox;
* Openbox;
* Blackbox;
* IceWM;
* PekWN;
* FVWM;
* DWM;
* AwesomeWM;
* WindowMaker;
* StumpWM;
* XMonad;
* Musca;
* i3;
* Ratpoison;
* ScrotWM;
* SpectrWM;
* WMFS²;
* WMii;
* Subtle;
* E16;
* WMFS;
* Compiz;
* Muffin;
* Enlightenment;
* Sawfish;
* MonsterWM;
* DminiWM;
* HerbstluftWM;
* XFWM4;
* Mutter;
* Metacity;
* Kwin;
* Beryl;
* Emerald.

Installation
------------

Two dependencies are to be installed, namely `pacman` and `iniparser`.
To build and install, run:

	$ make
	# sudo make install

Bugs
----

Should you find any, please open an [issue][bug].

[bug]: https://github.com/Unia/sysinfo

License
-------
This project is licensed under the GNU General Public License and should be treated as such. See `COPYING` for more information.

**Copyright © 2013-2014** Jente (jthidskes at outlook dot com)
