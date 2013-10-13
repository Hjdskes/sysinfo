/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * sysinfo.c
 * Copyright (C) 2013 Jente (jthidskes at outlook dot com)
 *
 * sysinfo.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sysinfo.c is distributed in the hope that it will be useful, but
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
#include <alpm.h>
#include <glib.h>
#include <getopt.h>
#include <sys/utsname.h>

#define TEXT "\e[0;37m"
#define LABEL "\e[1;34m"
#define BOLDTEXT "\e[1;37m"

static int showpkgs = 0;
static int showcolors = 0;

char *wmnames[] = { "fluxbox", "openbox", "blackbox", "icewm", "pekwm", "fvwm", "dwm", "awesomewm", "windowmaker", \
	"stumpwm", "xmonad", "musca", "i3", "ratpoison", "scrotwm", "spectrwm",	"wmfs2", "wmii", "subtle", "e16", "wmfs", "compiz", "muffin", \
	"enlightenment", "sawfish", "monsterwm", "dminiwm", "herbstluftwm", "xfwm4", "mutter", "metacity", "kwin", "beryl", "emerald", NULL };

void printhelp(int exval) {
	fprintf(stdout, "sysinfo - %sshow system/theme information in screenshots\n\
	sysinfo [-h] [-p] [-c /path/to/script]\n\n\
	-h                    print this help and exit\n\
	-c /path/to/script    use this script to display your terminal colors\n\
	-p                    display amount of installed packages\n", BOLDTEXT);
 	exit(exval);
}

void parsegtkrc(void) {
	char *home, gtk3rc[100], gtk2rc[50], gtk2rcmine[50], *gtktheme = NULL, *icontheme = NULL, *font = NULL;
	GKeyFile *settings;

	home = getenv("HOME");
	snprintf(gtk3rc, 100, "%s/.config/gtk-3.0/settings.ini", home);
	snprintf(gtk2rc, 50, "%s/.gtkrc-2.0", home);
	snprintf(gtk2rcmine, 50, "%s/.gtkrc-2.0.mine", home);
	settings = g_key_file_new();
	if(access(gtk3rc, F_OK) != -1 ) {
		if(g_key_file_load_from_file (settings, gtk3rc, G_KEY_FILE_NONE, NULL) == TRUE) {
			gtktheme = g_key_file_get_string(settings, "Settings", "gtk-theme-name", NULL);
			icontheme = g_key_file_get_string(settings, "Settings", "gtk-icon-theme-name", NULL);
			font = g_key_file_get_string(settings, "Settings", "gtk-font-name", NULL);
		}
	} else if(access(gtk2rc, F_OK) != -1 ) {
		if(g_key_file_load_from_file (settings, gtk2rc, G_KEY_FILE_NONE, NULL) == TRUE) {
			gtktheme = g_key_file_get_string(settings, "Settings", "gtk-theme-name", NULL);
			icontheme = g_key_file_get_string(settings, "Settings", "gtk-icon-theme-name", NULL);
			font = g_key_file_get_string(settings, "Settings", "gtk-font-name", NULL);
		}
	} else if(access(gtk2rcmine, F_OK) != -1 ) {
		if(g_key_file_load_from_file (settings, gtk2rcmine, G_KEY_FILE_NONE, NULL) == TRUE) {
			gtktheme = g_key_file_get_string(settings, "Settings", "gtk-theme-name", NULL);
			icontheme = g_key_file_get_string(settings, "Settings", "gtk-icon-theme-name", NULL);
			font = g_key_file_get_string(settings, "Settings", "gtk-font-name", NULL);
		}
	}
	if(!gtktheme && !icontheme && !font)
		gtktheme = icontheme = font = "Unable to retrieve";
	fprintf(stdout, "    %sTheme:   %s%s\n    %sIcons:   %s%s\n    %sFont:    %s%s\n", LABEL, TEXT, gtktheme, LABEL, TEXT, icontheme, LABEL, TEXT, font);
	free(gtktheme);
	free(icontheme);
	free(font);
}

void detectwm(char *username) {
	FILE *pid;
	int i;
	char pgrep[100], *wm = NULL, *wmname, testwm[100];

	for(i = 0; i < 34; i++) {
		wmname = wmnames[i];
		snprintf(pgrep, 100, "pgrep -U %s -x %s", username, wmname);
		pid = popen(pgrep, "r");
		if(pid != NULL) {
			if(fgets(testwm, sizeof(testwm), pid)) {
				wm = wmname;
				pclose(pid);
				break;
			} else
				pclose(pid);
		}
	}
	if(!wm)
		wm = "Unable to retrieve";
	fprintf(stdout, "    %sWM:      %s%s\n", LABEL, TEXT, wm);
}

void listpkgs(void) {
	alpm_list_t *i;
	alpm_db_t *db_path;
	alpm_handle_t *handle = NULL;
	int package = 0;

	handle = alpm_initialize ("/", "/var/lib/pacman", NULL);
	db_path = alpm_get_localdb(handle);
	for(i = alpm_db_get_pkgcache(db_path); i; i = alpm_list_next(i)) {
		package++;
	}
	alpm_release(handle);
	printf("    %sPacman:  %s%d installed packages\n", LABEL, TEXT, package);
}

int main(int argc, char *argv[]) {
	struct utsname my_uname;
	char *username, *shell, *colorpath = NULL;
	int opt;

	while((opt = getopt(argc, argv, "hpc:o:")) != -1) {
		switch(opt) {
			case 'h':
				printhelp(0);
			break;
			case 'c':
				showcolors = 1;
				colorpath = optarg;
			break;
			case 'p':
				showpkgs = 1;
			break;
			case ':':
				printhelp(1);
			break;
			case '?':
				printhelp(1);
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
	fprintf(stdout, "    %sShell:   %s%s\n    %sUser:    %s%s\n", LABEL, TEXT, shell, LABEL, TEXT, username);
	if(uname(&my_uname) != -1)
		fprintf(stdout, "    %sHost:    %s%s\n    %sKernel:  %s%s %s\n", LABEL, TEXT, my_uname.nodename, LABEL, TEXT, my_uname.release,my_uname.machine);
	if(showpkgs)
		listpkgs();
	detectwm(username);
	parsegtkrc();
	if(showcolors) {
		fprintf(stdout, "\n    %sTerminal colors:\n", LABEL);
		system(colorpath);
	}
	return 0;
}
