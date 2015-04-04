
#ifndef SERIAL_H
#define SERIAL_H


int InitComm(void);
int CleanupComm(void);
int SaveComm(void);
int RestoreComm(void);
int ConfigComm(int baud);
int SendData(const unsigned char *buf, int length);
int FlushDev(void);


#endif SERIAL_H
