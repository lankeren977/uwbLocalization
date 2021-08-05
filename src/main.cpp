#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <pthread.h>
#include <iomanip>
#include "openDev.h"
#include "location.h"
#include "load_config.h"

vec2d uwb_result;
pthread_t ntid_u;
int dev;

void *getUWBData(void *data)
{
    ssize_t nread;
    unsigned char buffer[20];

    while (1)
    {
        nread = read(dev, buffer, MAX_BUFF_SIZE);
        if (nread != 0 && 'm' == buffer[0])
        {
            int offset = 0;
            int *radius = new int[ANCHOR_NUM];
            switch (buffer[1])
            {
            case 'r': //原始测距数据
                print_hex(buffer, 16);
                for (int i = 0; i < ANCHOR_NUM; i++)
                {
                    unsigned int dis = buffer[ANCHOR_DIS_START + offset + 1] << 8;
                    dis = dis ^ buffer[ANCHOR_DIS_START + offset];
                    radius[i] = dis;
                    offset = offset + 2;
                }
                //选取最近的四个
                uwb_result = trilateration(radius);
                break;
            case 'c': //校正后测距数据
                break;
            case 'a': //基站间测距数据
                break;
            default:
                break;
            }
            delete radius;
        }
    }
    delete buffer;
}

int main()
{
    ofstream outFile;
    outFile.open("/home/node/Desktop/zhangqr-linux/uwbLocalize/output/data.csv", ios::out); // 打开模式可省略

    dev = OpenDev(); //打开串口
    if (dev > 0)
    {
        set_speed(dev, SPEED); //设置波特率
    }
    else
    {
        perror("Can't Open Serial Port");
        //return 0;
    }

    if (set_Parity(dev, 8, 1, 'N') == -1)
    {
        printf("Set Parity Error\n");
        exit(1);
    }

    tcflush(dev, TCIFLUSH);
    vector<vec2d> result;

    loadConfig();

    //取配置参数
    loadUWBParams();

    ssize_t nread;
    unsigned char buffer[20];

    while (1)
    {
        nread = read(dev, buffer, MAX_BUFF_SIZE);
        if (nread != 0 && 'm' == buffer[0])
        {
            int offset = 0;
            int *radius = new int[ANCHOR_NUM];
            switch (buffer[1])
            {
            case 'r': //原始测距数据
                print_hex(buffer, 16);
                for (int i = 0; i < ANCHOR_NUM; i++)
                {
                    unsigned int dis = buffer[ANCHOR_DIS_START + offset + 1] << 8;
                    dis = dis ^ buffer[ANCHOR_DIS_START + offset];
                    radius[i] = dis;
                    offset = offset + 2;
                }
                //选取最近的四个
                uwb_result = trilateration(radius);
                break;
            case 'c': //校正后测距数据
                break;
            case 'a': //基站间测距数据
                break;
            default:
                break;
            }
            delete radius;
        }
    }
    delete buffer;
/*
    int err1;

    err1 = pthread_create(&ntid_u, NULL, getUWBData, NULL);
    if (err1 != 0)
    {
        cout << "visual thread creates fail" << endl;
    }
    while (1)
    {
        outFile << setiosflags(ios::fixed) << setprecision(2) << uwb_result.x << "," << uwb_result.y << endl;
        cout<< uwb_result.x <<"," << uwb_result.y<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    pthread_cancel(ntid_u);
    //关闭串口
    close(dev);
    */
    return 0;
}
