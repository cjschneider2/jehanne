/*
 * This file is part of Jehanne.
 *
 * Copyright (C) 2015-2018 Giacomo Tesio <giacomo@tesio.it>
 *
 * Jehanne is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Jehanne is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Jehanne.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <u.h>
#include <lib9.h>

#include "console.h"

int linecontrol;
int blind;
int crnl;

static void
usage(void)
{
	fprint(2, "usage: %s [-d dbgfile] [-l] [-b] [-P] program [args]\n", argv0);
	exits("usage");
}
void
main(int argc, char *argv[])
{
	int fd, devmnt;

	blind = 0;
	linecontrol = 1;
	crnl = 0;
	ARGBEGIN{
	case 'b':
		blind = 1;
		break;
	case 'd':
		enabledebug(EARGF(usage()));
		break;
	case 'l':
		linecontrol = 0;
		break;
	case 'P':
		posix = 1;
		break;
	default:
		usage();
	}ARGEND;

	if(argc == 0)
		usage();

	fd = servecons(passthrough, passthrough, &devmnt);

	debug("%s %d: mounting cons for %s\n", argv0, getpid(), argv[0]);
	if(mount(fd, -1, "/dev", MBEFORE, "", devmnt) == -1)
		sysfatal("mount (%s): %r", argv[0]);

	debug("%s (%d): all services started, ready to exec(%s)\n", argv0, getpid(), argv[0]);

	/* become the requested program */
	rfork(RFNOTEG|RFREND|RFCFDG);

	if(posix){
		fd = open("/dev/tty", OREAD);
		fd = open("/dev/tty", OWRITE);
	} else {
		fd = open("/dev/cons", OREAD);
		fd = open("/dev/cons", OWRITE);
	}
	if(dup(fd, 2) != 2)
		sysfatal("bad FDs: %r");
	jehanne_pexec(argv[0], argv);
	sysfatal("exec %s: %r", argv[0]);
}
