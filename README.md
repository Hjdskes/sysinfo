Sysinfo
=========

**A tool to display theme and system information to be used in screenshots.**

This little application will print information about your system and your theme, along with some nice ASCII art when run.

**NOTE:** This application is specifically written for Arch Linux; the ASCII art is the Arch Linux logo and the package count only works with Pacman.

Currently, it prints the following:
* Shell;
* Username;
* Hostname;
* Kernel version and OS type (64 or 32 bit);
* Amount of installed packages[1];
* Window manager
* GTK theme;
* Icon set (GTK);
* Font (GTK);
* Terminal colors[2].

[1]: This is an optional feature and for now only works with Pacman.

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

The following Window Managers and Desktop Environments are supported.

**Window Managers:**
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

**Desktop Environments:**
* GNOME;
* Cinnamon;
* KDE;
* XFCE;
* LXDE;
* Mate.

Installation
------------

Two dependencies are to be installed, namely `pacman` and `glib2`.
To build and install, run:

	$ make
	# sudo make install

ToDo
----

* Remove our dependency on glib2; requiring the whole glib2 library is overkill for our use. Perhaps switch to iniparser, or come up with something ourselves?

Bugs
----

Should you find any, please open an [issue][bug].

[bug]: https://github.com/Unia/sysinfo

License
-------
**Sysinfo** is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

**Sysinfo** is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

**Copyright © 2013** Jente (jthidskes at outlook dot com)
