#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>
#include <sys/utsname.h>

#define TEXT "\e[0;37m"
#define LABEL "\e[1;34m"
#define BOLDTEXT "\e[1;37m"

char *wmnames[] = { "fluxbox", "openbox", "blackbox", "icewm", "pekwm", "fvwm", "dwm", "awesomewm", "windowmaker", \
	"stumpwm", "xmonad", "musca", "i3", "ratpoison", "scrotwm", "spectrwm",	"wmfs2", "wmii", "subtle", "e16", "wmfs", "compiz", "muffin", \
	"enlightenment", "sawfish", "monsterwm", "dminiwm", "herbstluftwm", "xfwm4", "mutter", "metacity", "kwin", "beryl", "emerald", NULL };
char *denames[] = { "gnome-session", "gnome-settings-daemon", "ksmserver", "plasma-desktop", "xfce-mcs-manage", "xfce4-session", "xfconfd", "lxsession", \
	"mate-session", "mate-settings-daemon", NULL };

void printhelp(int exval) {
	fprintf(stdout, "sysinfo - %sshow system/theme information in screenshots\n\
	sysinfo [-h] [-p] [-c /path/to/script]\n\n\
	-h                    print this help and exit\n", BOLDTEXT);
 	exit(exval);
}

void detectde(char *username) {
	FILE *pid;
	int i;
	char pgrep[100], *dename, *de = NULL, test[100];

	for(i = 0; i < 10; i++) {
		dename = denames[i];
		sprintf(pgrep, "pgrep -U %s -x %s", username, dename);
		pid = popen(pgrep, "r");
		if(pid != NULL) {
			if(fgets(test, sizeof(test), pid)) {
				de = dename;
				pclose(pid);
				break;
			} else
				pclose(pid);
		}
	}
	if(!de)
		de = "Unable to retrieve";
	else {
		if(strcmp(de, "gnome-session") == 0 || strcmp(de, "gnome-settings-daemon") == 0)
			de = "GNOME";
		else if(strcmp(de, "xfce-mcs-manage") == 0 || strcmp(de, "xfce4-session") || strcmp(de, "xfconfd") == 0)
			de = "XFCE4";
		else if(strcmp(de, "ksmserver") == 0 || strcmp(de, "plasma-desktop") == 0)
			de = "KDE";
		else if(strcmp(de, "lxsession") == 0)
			de = "LXDE";
		else if(strcmp(de, "mate-session") == 0 || strcmp(de, "mate-settings-daemon") == 0)
			de = "Mate";
	}
	fprintf(stdout, "    %sDE:      %s%s\n", LABEL, TEXT, de);
}

void detectwm(char *username) {
	FILE *pid;
	int i;
	char pgrep[100], *wm = NULL, *wmname, test[100];

	for(i = 0; i < 34; i++) {
		wmname = wmnames[i];
		sprintf(pgrep, "pgrep -U %s -x %s", username, wmname);
		pid = popen(pgrep, "r");
		if(pid != NULL) {
			if(fgets(test, sizeof(test), pid)) {
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

int main(int argc, char *argv[]) {
	struct utsname my_uname;
	char *username, *shell;
	int opt;

	while((opt = getopt(argc, argv, "h:o:")) != -1) {
		switch(opt) {
			case 'h':
				printhelp(0);
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
	detectde(username);
	detectwm(username);
	return 0;
}
