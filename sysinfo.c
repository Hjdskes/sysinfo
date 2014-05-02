/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * sysinfo.c
 * Copyright (C) 2013-2014 Jente Hidskes (hjdskes@gmail.com)
 *
 * Sysinfo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sysinfo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <alpm.h>
#include <iniparser.h>
#include <getopt.h>
#include <sys/utsname.h>

#define TEXT      "\033[0;37m"
#define LABEL     "\033[1;34m"
#define BOLDTEXT  "\033[1;37m"
#define LENGTH(X) (sizeof X / sizeof X[0]) 

void
printhelp(int exval) {
	fprintf(stdout, "sysinfo - %sshow system/theme information in screenshots\n\
	sysinfo [-h] [-p] [-c /path/to/script]\n\n\
	-h                    print this help and exit\n\
	-c /path/to/script    use this script to display your terminal colors\n\
	-p                    display amount of installed packages\n", BOLDTEXT);
 	exit(exval);
}

void
parsegtkrc() {
	dictionary *d;
	char *home, *gtkrc, *theme, *icons, *font;
	size_t len;

	home = getenv("HOME");
	/* first try gtk+-3.0 */
	len = strlen(home) + strlen("/.config/gtk-3.0/settings.ini") + 1; /* null-byte */
	gtkrc = (char *)malloc(len);
	snprintf(gtkrc, len, "%s/.config/gtk-3.0/settings.ini", home);
	if(access(gtkrc, F_OK) == 0) {
		d = iniparser_load(gtkrc);
		theme = iniparser_getstring(d, "settings:gtk-theme-name", "Unable to retrieve");
		icons = iniparser_getstring(d, "settings:gtk-icon-theme-name", "Unable to retrieve");
		font = iniparser_getstring(d, "settings:gtk-font-name", "Unable to retrieve");
	} else { /* if it can't be found, try gtk+-2.0 */
		free(gtkrc);
		len = strlen(home) + strlen("/.gtkrc-2.0") + 1; /* null-byte */
		gtkrc = (char *)malloc(len);
		snprintf(gtkrc, len, "%s/.gtkrc-2.0", home);
		
		/* now it doesn't matter if gtk+-2.0 is found or not,
		 * as we set a default anyway */
		d = iniparser_load(gtkrc);
		theme = iniparser_getstring(d, ":gtk-theme-name", "Unable to retrieve");
		icons = iniparser_getstring(d, ":gtk-icon-theme-name", "Unable to retrieve");
		font = iniparser_getstring(d, ":gtk-font-name", "Unable to retrieve");
	}

	free(gtkrc);
	fprintf(stdout, "    %sTheme:   %s%s\n    %sIcons:   %s%s\n    %sFont:    %s%s\n", LABEL, TEXT, theme, LABEL, TEXT, icons, LABEL, TEXT, font);
	iniparser_freedict(d);
}

char *
detectwm(const char *username) {
	Display *dpy;
	Atom name, utf8, t;
	int f;
	unsigned long n, a;
	FILE *pid;
	unsigned int i;
	size_t len;
	char *pgrep, *wm, *wmname, testwm[100];
	unsigned char *wm_xlib;
	/* Window managers that do not set _NET_WM_NAME. If you know of any that do
	 * not, that are not yet in here, please tell me.
	 * Likewise, if there are some in here that do set it, please tell me. */
	char *wmnames[] = { "dwm", "xmonad", "wmii", "sawfish", "monsterwm", \
			"dminiwm", "mutter", NULL };

	/* faster and cleaner */
	if((dpy = XOpenDisplay(NULL))) {
		name = XInternAtom(dpy, "_NET_WM_NAME", False);
		utf8 = XInternAtom(dpy, "UTF8_STRING", False);

		if(XGetWindowProperty(dpy, DefaultRootWindow(dpy), name, 0L, sizeof name,
				False, utf8, &t, &f, &n, &a, &wm_xlib) == Success && wm_xlib) {
			wm = (char *)wm_xlib;
			XFree(wm_xlib);
		}
		XCloseDisplay(dpy);
		return wm;
	}

	/* Ugly, but workaround for window managers that do not set
	 * _NET_WM_NAME. Only executed if the above fails */
	for(i = 0; i < LENGTH(wmnames); i++) {
		wmname = wmnames[i];
		len = strlen(username) + strlen(wmname) + strlen("pgrep -U %s -x %s");
		pgrep = (char *)malloc(len);
		snprintf(pgrep, len , "pgrep -U %s -x %s", username, wmname);
		pid = popen(pgrep, "r");
		if(pid != NULL) {
			if(fgets(testwm, sizeof(testwm), pid)) {
				wm = wmname;
				pclose(pid);
				break;
			} else
				pclose(pid);
		}
		free(pgrep);
	}
	if(!wm)
		wm = "Unable to retrieve";
	return wm;
}

int
listpkgs(void) {
	alpm_list_t *i;
	alpm_db_t *db_path;
	alpm_handle_t *handle = NULL;
	int package = 0;

	handle = alpm_initialize("/", "/var/lib/pacman", NULL);
	db_path = alpm_get_localdb(handle);
	for(i = alpm_db_get_pkgcache(db_path); i; i = i->next)
		package++;
	alpm_list_free(i);
	alpm_release(handle);
	return package;
}

int
main(int argc, char *argv[]) {
	struct utsname my_uname;
	const char *username, *shell;
	char *wm, *colorpath = NULL;
	int opt, packages, showpkgs = 0, showcolors = 0;

	while((opt = getopt(argc, argv, "hpc:o:")) != -1) {
		switch(opt) {
			case 'h':
				printhelp(EXIT_SUCCESS);
			case 'c':
				showcolors = 1;
				colorpath = optarg; /* probably unsafe */
				break;
			case 'p':
				showpkgs = 1;
				break;
			case '?':
				printhelp(EXIT_FAILURE);
		}
	}

	fprintf(stdout, "%s                      _     _ _                   \n\
        __ _ _ __ ___| |__ | (_)_ __  _   ___  __\n\
       / _` | '__/ __| '_ \\| | | '_ \\| | | \\ \\/ /\n\
      | (_| | | | (__| | | | | | | | | |_| |>  <\n\
       \\__,_|_|  \\___|_| |_|_|_|_| |_|\\__,_/_/\\_\\\n\n\
       %sA simple, lightweight Linux distribution.\t\n\n", LABEL, TEXT);

	username = getenv("USER");
	shell = getenv("SHELL");
	if(!username)
		username = "Unable to retrieve";
	if(!shell)
		shell = "Unable to retrieve";
	fprintf(stdout, "    %sShell:   %s%s\n    %sUser:    %s%s\n",
			LABEL, TEXT, shell, LABEL, TEXT, username);
	if(uname(&my_uname) >= 0) {
		fprintf(stdout, "    %sHost:    %s%s\n    %sKernel:  %s%s %s\n",
				LABEL, TEXT, my_uname.nodename, LABEL, TEXT, my_uname.release, my_uname.machine);
	}
	if(showpkgs) {
		packages = listpkgs();
		fprintf(stdout, "    %sPacman:  %s%d installed packages\n", LABEL, TEXT, packages);
	}
	wm = detectwm(username);
	fprintf(stdout, "    %sWM:      %s%s\n", LABEL, TEXT, wm);
	parsegtkrc();
	if(showcolors) {
		fprintf(stdout, "\n    %sTerminal colors:\n", LABEL);
		system(colorpath); /* probably unsafe */
	}
	exit(EXIT_SUCCESS);
}
