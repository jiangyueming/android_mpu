#include "uart.h"
#include <memory.h>
#include <pthread.h>
#include "stdio.h"
#include "mpu.h"
#include "stdint.h"
#include <stdbool.h>

char anglesInit[] = {0xFF, 0xAA, 0x52};
char anglesOpen[] = {0xFF, 0xAA, 0x53};
char anglesStop[] = {0xFF, 0xAA, 0x54};
char accgrorOpen[] = {0xFF, 0xAA, 0x55};
char accgrorStop[] = {0xFF, 0xAA, 0x56};
char anglesAccgrorOpen[] = {0xFF, 0xAA, 0x57};
char anglesAccgrorStop[] = {0xFF, 0xAA, 0x58};
char MpuCal[] = {0xFF, 0xAA, 0x59};


void * read_uart(void *arg)
{
    unsigned char data[GROR_ACCGROR_EVENT_FRAME_LENGTH];
    uart *tty = (uart*)arg;
    int length;
    //the idea is that we only need 1/10 sample, ignore the others
    char i = 0, j = 0, k = 0;
    unsigned char cUart, preChar = 0;
    bool b_Match = false;
    bool b_ArorAcc = false;
    while (1)
    {
        length = read(tty->uart_fd, &cUart, 1);
        if (length > 0)
        {
            //printf("0x%x ", cUart);
            if (b_ArorAcc)
            {
                data[j] = cUart;
                j++;
                if (j  == GROR_ACCGROR_EVENT_FRAME_LENGTH)
                {
                    //Got an event frame
                    j = 0;
                    b_ArorAcc = false;
                    i ++;
                    //printf("\n");
                    if (!(i % 10))
                    {
                        //1 out of 10 events need to be processed
                        GrorAccEvent result;
                        char ret;
                        memset(&result, 0, sizeof(GrorAccEvent));
                        ret = decodeGrorAccFrame(data, &result);
                        printf("test_decode ret %d\n", ret);
                    #if 1
                        for (k = 0; k < GROR_ACCGROR_EVENT_FRAME_LENGTH; k++)
                        {
                            printf("0x%x ", data[k]);
                        }
                        printf("\n");
                    #endif
                        if (ret == 0)
                            GrorAccEventLog(&result);
                        i = 0;
                    }
                }

            }
            else if (b_Match)
            {
                if (cUart == GROR_ACCGROR_EVENT_VALUE)
                {
                    b_ArorAcc = true;
                    //printf("***Right_Value***\n");
                }
                b_Match = false;
            }
            else if ((preChar == 0XAA) && (cUart == 0XAA))
            {
                b_Match = true;
                preChar = 0;
                //printf("***Match***\n");
            }
            else
            {
                preChar = cUart;
            }
        }
    }
}

void * enable_mpu(void *arg)
{
    uart *tty = (uart*)arg;
    int result = 0;

    result = uart_send(tty, accgrorOpen, sizeof(accgrorOpen));
    printf("write_uart %d \n", result);
}

int main( int argc, char ** argv)
{
    int err;
    pthread_t tid1;
    uart tty;

    tty.uart_path = "/dev/ttyUSB0";
    tty.baud_rate = 115200;
    tty.parity_check = 1;
    tty.two_stop_bit = 1;
    tty.blocking_mode = 1;

    err = uart_open(&tty);
    printf("uart_open err %d\n", err);
    if (err != 0)
    {
        return -1;
    }
    enable_mpu((void *)&tty);
    err = pthread_create(&tid1, NULL, read_uart, (void *)&tty);

    pthread_join(tid1, NULL);
    return 0;
}

int test_decode()
{
    const char event[] = {0xAA, 0xAA, 0x02, 0x12, 0xFF, 0x01, 0x00, 0x03, 0x1B, 0xC2, 0xFF, 0xED, 0xFF, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30};

    GrorAccEvent result;
    memset(&result, 0, sizeof(GrorAccEvent));
    char ret = decodeGrorAccFrame(event, &result);
    printf("test_decode ret %d\n", ret);
    GrorAccEventLog(&result);
    return 0;
}