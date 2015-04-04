
/*
 * main.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <linux/serial.h>

#include "install.h"
#include "serial.h"
#include "sfile.h"


int Init(void);
void Cleanup(void);
void Usage(void);
void Error(char *err);
void Perror(char *err);
void SigDef(int signum);


int main(int argc, char **argv) {
	if (Init() == -1)
		Error("Init()");

	return 0;
}


/* Init() initializes all the different parts of the program. If anything goes
 * wrong it cleans up and returns. */
int Init(void) {
	/* install signal handlers */
	signal(SIGHUP, SigDef);
	signal(SIGINT, SigDef);
	signal(SIGKILL, SigDef);

	/* setup communications */
	if (InitComm() == -1)
		goto Comm_fail;

	if (InstallProgram() == -1)
		goto Install_fail;

	return 0;

  Install_fail:
	CleanupComm();
  Comm_fail:
	return -1;
}


/* Cleanup() resets everything so that we can exit cleanly. */
void Cleanup(void) {
	/* shut down communications */
	CleanupComm();
}


void SigDef(int signum) {
	printf("signal %i\n", signum);
	Cleanup();
	exit(-2);
}


void Usage(void) {
	printf("Usage: Not implemented\n");
	fflush(stdout);
	exit(1);
}


/* Error() displays errno and exits. Currently used only in main if Init() fails
 * since it is the only routine that doesn't need to do a Cleanup(). */
void Error(char *err) {
	perror(err);
	exit(2);
}


/* Perror displays errno, does a Cleanup() and exits. (pretty obvious!!) */
void Perror(char *err) {
	perror(err);
	Cleanup();
	exit(3);
}
