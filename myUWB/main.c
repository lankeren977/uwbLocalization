#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "openDev.h"
#include "nlink_linktrack_tagframe0.h"
#include "nlink_utils.h"

NLINK_PACKED(typedef struct
             {
               uint8_t a;
               uint8_t b;
               uint32_t c;
               double d;
               uint8_t e;
             })
pack_test_t;

void print_hex(unsigned char* chr,int num)
{
    for(int i =0;i<num;i++)
    {
        printf("%02x ",*chr);
        chr++;
    }
    printf("\n");
}


int main()
{
    uint32_t check = 1;
    if (*(uint8_t *)(&check) != 1)
    {
        printf("Error: this code must run in little endian.");
        return EXIT_FAILURE;
    }
    
    if (sizeof(pack_test_t) != 15)
    {
        printf("Error: Pack do not work, pack size:%d. Contact us for support", sizeof(pack_test_t));
        return EXIT_FAILURE;
    }
    
    uint8_t data[1024];
    size_t data_length;
    
    int dev;
    int nread;
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
            //data_length = NLink_StringToHex(string, data);
            if (g_nlt_tagframe0.UnpackData(buffer, sizeof(buffer)))
            {
                nlt_tagframe0_result_t *result = &g_nlt_tagframe0.result;
                printf("LinkTrack TagFrame0 data unpack successfully:\r\n");
                printf("id:%d, system_time:%d, valid_node_count:%d, x_value:%f, y_value:%f, z_value:%f\r\n", result->id, result->system_time, result->pos_3d[0], result->pos_3d[1], result->pos_3d[2]);
            }
        }
        
    }

    return 0;
}
