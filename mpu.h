#ifndef MPU_H
#define MPU_H

#define GROR_ACCGROR_EVENT_FRAME_LENGTH (20)//23 -3
#define GROR_ACCGROR_EVENT_VALUE (2)
#define ANGLES_EVENT_FRAME_LENGTH (16)
#define ANGLES_EVENT_VALUE (1)
#define EVENT_VALUE_OFFSET (2)
#define EVENT_DATA_LENGTH (0x12)

enum MPU_DECODE_ERROR
{
    MPU_DECODE_ERROR_BASE = 100,
    MPU_SYNC_CODE_ERROR,
    MPU_EVENT_TYPE_ERROR,
    MPU_EVENT_DATA_LENGTH_ERROR,
    MPU_CHECK_SUM_ERROR,
};

typedef struct _GrorAccEvent {
    float ACC_X;
    float ACC_Y;
    float ACC_Z;
    float GYRO_X;
    float GYRO_Y;
    float GYRO_Z;
}GrorAccEvent;

void GrorAccEventLog(GrorAccEvent * event);
int decodeGrorAccFrame(const unsigned char * frame, GrorAccEvent * event);
#endif