
/*
 * install.c
 */


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#include "install.h"
#include "sfile.h"
#include "serial.h"


/* Install the program at HC11 */
int InstallProgram(void) {
	struct sfile *sf;
	char accept_rate = 0xFF;
	int r1, r2;
	
	/* parse the s-file, before uploading it */
	sf = CreateSFile("./hc11/nokia.s19", SFILE_UNCOMPRESSED);
	if (sf == NULL || sf->dsize != 256)
		return -1;

	/* upload the code */
	r1 = SendData(&accept_rate, 1);
	r2 = SendData(sf->data, sf->dsize);
	DestroySFile(sf);
	if (r1 == -1 || r2 == -1)
		return -1;

	/* set a higher baud rate */
	ConfigComm(9600);

	FlushDev();
	return 0;
}
