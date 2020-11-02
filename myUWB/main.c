#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "openDev.h"
#include "nlink_linktrack_tagframe0.h"
#include "nlink_utils.h"
float past_theta = 0;

int main()
{
    int dev;
    ssize_t nread;
    unsigned char buffer[MAX_BUFF_SIZE];

    dev = OpenDev(); //打开串口
    if(dev > 0){
        set_speed(dev,SPEED); //设置波特率
    }else{
        perror("Can't Open Serial Port");
        return 0;
    }

    if(set_Parity(dev,8,1,'N') == -1){
        printf("Set Parity Error\n");
        exit(1);
    }

    tcflush(dev,TCIFLUSH);
    
    while(1)
    {
        nread = read(dev, buffer, MAX_BUFF_SIZE);
        if(nread!=0 && 'U'==buffer[0]){
            //print_hex(buffer,128);
            if (g_nlt_tagframe0.UnpackData(buffer, sizeof(buffer)))
            {
                nlt_tagframe0_result_t *result = &g_nlt_tagframe0.result;
                double d_theta = result->angle_3d[2] - past_theta;
                past_theta = result->angle_3d[2];
                printf("id:%d, x:%f, y:%f, theta:%f, d_theta:%f\r\n", result->id, result->pos_3d[0], result->pos_3d[1], result->angle_3d[2], d_theta);
            }
        }
        
    }
    return 0;
}
