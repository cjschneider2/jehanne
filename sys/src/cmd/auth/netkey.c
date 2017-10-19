#include <u.h>
#include <lib9.h>
#include <authsrv.h>
#include <bio.h>
#include "authcmdlib.h"


void
usage(void)
{
	fprint(2, "usage: netkey\n");
	exits("usage");
}

void
main(int argc, char *argv[])
{
	char buf[32], pass[32], key[DESKEYLEN];
	char *s;
	int n;

	ARGBEGIN{
	default:
		usage();
	}ARGEND
	if(argc)
		usage();

	s = getenv(ENV_SERVICE);
	if(s && strcmp(s, "cpu") == 0){
		fprint(2, "netkey must not be run on the cpu server\n");
		exits("boofhead");
	}

	readln("Password: ", pass, sizeof pass, 1);
	passtodeskey(key, pass);

	for(;;){
		print("challenge: ");
		n = read(0, buf, sizeof buf - 1);
		if(n <= 0)
			exits(0);
		buf[n] = '\0';
		n = strtol(buf, 0, 10);
		sprint(buf, "%d", n);
		netcrypt(key, buf);
		print("response: %s\n", buf);
	}
}
