#ifndef OPENDEV_H
#define OPENDEV_H

#include <math.h>

#define TRUE 1
#define FALSE 0
#define UART_DEVICE  "/dev/tty.SLAB_USBtoUART"
#define SPEED 115200
#define MAX_BUFF_SIZE 128

int OpenDev();
void set_speed(int fd, int speed);
int set_Parity(int fd,int databits,int stopbits,int parity);
int uart_deinit();

#endif
