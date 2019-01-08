#ifndef __COMSERIAL_H_
#define __COMSERIAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>

#define BAUDRATE B115200
#define SERIAL_DEVICE "/dev/ttyAMA0"

int serial_recevice_flag;
pthread_t serial_rec_pid;

void* Serial_Recevice_pthread(void*arg);
int Serial_Send_Data(int serial_fd);


#endif

