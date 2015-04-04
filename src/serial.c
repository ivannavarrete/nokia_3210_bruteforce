
/*
 * serial.c
 */


#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"


struct termios org_mode;
int dev;								/* interface handler */
const char *iface = "/dev/ttyS1";		/* default interface */
int baud = 1200;						/* default initial interface speed */


int InitComm(void) {
	dev = open(iface, O_RDWR | O_SYNC);
	if (dev == -1)
		goto fail;
	if (SaveComm())
		goto fail;
	if (ConfigComm(baud))
		goto fail;

	return dev;
  fail:
	perror("InitComm()");
	return -1;
}


int CleanupComm(void) {
	RestoreComm();
	close(dev);
	return 0;
}


int SaveComm(void) {
	return tcgetattr(dev, &org_mode);
}


int RestoreComm(void) {
	return tcsetattr(dev, TCSANOW, &org_mode);
}


/* Currently this only changes the baud rate. */
int ConfigComm(int baud) {
	struct termios ser;
	speed_t speed;

	switch (baud) {
		case 50: speed = B50; break;
		case 75: speed = B75; break;
		case 110: speed = B110; break;
		case 134: speed = B134; break;
		case 150: speed = B150; break;
		case 200: speed = B200; break;
		case 300: speed = B300; break;
		case 600: speed = B600; break;
		case 1200: speed = B1200; break;
		case 1800: speed = B1800; break;
		case 2400: speed = B2400; break;
		case 4800: speed = B4800; break;
		case 9600: speed = B9600; break;
		default: speed = B1200; break;
	}

	/* change input and output baud rate */
	if (tcgetattr(dev, &ser))
		goto fail;
	if (cfsetospeed(&ser, speed))
		goto fail;
	if (cfsetispeed(&ser, speed))
		goto fail;
	if (tcsetattr(dev, TCSADRAIN, &ser))
		goto fail;
	
	/* 1 start bit, 8 data bits, 1 stop bit, no parity */
	/* TODO: Fix this ugly cfmakeraw() hack ASAP!! */
	cfmakeraw(&ser);
	if (tcsetattr(dev, TCSADRAIN, &ser))
		goto fail;

	return 0;
  fail:
	perror("ConfigComm()");
	return -1;
}


int SendData(const unsigned char *buf, int length) {
	int res;
	int i;

	for (i=res=0; i<length; i+=res) {
		res = write(dev, buf+i, length-i);
		if (res == -1) {
			perror(": SendData(): ");
			return -1;
		}
	}

	return 0;
}


/* Find a better way to flush the device */
/* maybe tcflush() ? */
int FlushDev(void) {
	int res;
	char c;
	
	fcntl(dev, F_SETFL, O_NONBLOCK);
	do {
		res = read(dev, &c, 1);
	} while (res != 0 && res != -1);
	fcntl(dev, F_SETFL, ~O_NONBLOCK);

	return res;
}
