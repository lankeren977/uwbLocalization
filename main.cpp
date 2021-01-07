#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "openDev.h"
#include "location.h"

#define ANCHOR_NUM 3
#define ANCHOR_DIS_START 6
vec2d anchorArray[ANCHOR_NUM] = {{0, 0}, {100, 0}, {57, 53}};

int main()
{
    int dev;
    ssize_t nread;
    unsigned char buffer[MAX_BUFF_SIZE];

    dev = OpenDev(); //打开串口
    if (dev > 0)
    {
        set_speed(dev, SPEED); //设置波特率
    }
    else
    {
        perror("Can't Open Serial Port");
        return 0;
    }

    if (set_Parity(dev, 8, 1, 'N') == -1)
    {
        printf("Set Parity Error\n");
        exit(1);
    }

    tcflush(dev, TCIFLUSH);

    vec2d result;
    while (1)
    {
        nread = read(dev, buffer, MAX_BUFF_SIZE);
        if (nread != 0 && 'm' == buffer[0])
        {
            int offset = 0;
            int radius[ANCHOR_NUM];
            switch (buffer[1])
            {
            case 'r': //原始测距数据
                //print_hex(buffer, 16);
                for (int i = 0; i < ANCHOR_NUM; i++)
                {
                    unsigned int dis = buffer[ANCHOR_DIS_START + offset + 1] << 8;
                    dis = dis ^ buffer[ANCHOR_DIS_START + offset];
                    offset = offset + 2;
                    radius[i] = dis;
                }

                result = trilateration(anchorArray, radius, ANCHOR_NUM);
                cout << result.x << "," << result.y << endl;
                break;
            case 'c': //校正后测距数据
                break;
            case 'a': //基站间测距数据
                break;
            default:
                break;
            }
        }
    }
    return 0;
}