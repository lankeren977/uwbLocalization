#ifndef OPENDEV_H
#define OPENDEV_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define UART_DEVICE  "/dev/tty.SLAB_USBtoUART"
#define SPEED 115200
#define MAX_BUFF_SIZE 128

int OpenDev(void);
void set_speed(int fd, int speed);
int set_Parity(int fd,int databits,int stopbits,int parity);
int uart_deinit(int dev);
void print_hex(unsigned char* chr,int num);

#endif
