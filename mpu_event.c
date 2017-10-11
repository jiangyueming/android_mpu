#include "stdio.h"
#include "mpu.h"
#include "stdint.h"

float DecodeAccgor(const unsigned char * acc, GrorAccEvent * event)
{
    int16_t ACC = 0;
    const float factor = 16*9.8 /32768;
    //printf("DecodeAccgor factor %f\n", factor);

    //X
    ACC = (acc[0] << 8 )| acc[1];
    event-> ACC_X = ACC * factor;

    //Y
    ACC = (acc[2] << 8 )| acc[3];
    event-> ACC_Y = ACC * factor;

    //Z
    ACC = (acc[4] << 8 )| acc[5];
    event-> ACC_Z = ACC * factor;
}

float DecodeGror(const unsigned char * gror, GrorAccEvent * event)
{
    int16_t GROR = 0;
    const float factor = (float)2000/32768;
    //printf("DecodeGror factor %f\n", factor);
    //X
    GROR = (gror[0] << 8) | gror[1];
    event-> GYRO_X = GROR * factor;

    //Y
    GROR = (gror[2] << 8) | gror[3];
    event-> GYRO_Y = GROR * factor;

    //Z
    GROR = (gror[4] << 8) | gror[5];
    event-> GYRO_Z = GROR * factor;
}

int decodeGrorAccFrame(const unsigned char * frame, GrorAccEvent * event)
{
    unsigned char index = 0;
    unsigned char sum = 0;
    //Check 2B SYNC code
    /*if((frame[0] != 0xAA)||(frame[1] != 0xAA))
    {
        return MPU_SYNC_CODE_ERROR;
    }

    //Check event type
    if(frame[2] != GROR_ACCGROR_EVENT_VALUE)
    {
        return MPU_EVENT_TYPE_ERROR;
    }*/

    if(frame[0] != EVENT_DATA_LENGTH)
    {
        return MPU_EVENT_DATA_LENGTH_ERROR;
    }

    //check sum
    sum = 0;
    for(index = 0; index < 13; index ++)
    {
        sum += frame[index];
    }
    sum += 0xAA;
    sum += 0xAA;
    sum += 2;
    if(frame[19] != sum)
    {
        return MPU_CHECK_SUM_ERROR;
    }

    DecodeAccgor(&frame[1], event);
    DecodeGror(&frame[7], event);
    return 0;
}

void GrorAccEventLog(GrorAccEvent * event)
{
    printf("ACC_X %f, ACC_Y %f, ACC_Z %f,GYRO_X %f, GYRO_Y %f, GYRO_Z %f\n",
        event->ACC_X,event->ACC_Y,event->ACC_Z, event->GYRO_X, event->GYRO_Y, event->GYRO_Z);
}