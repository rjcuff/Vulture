#ifndef IMU_ADAPTER_H
#define IMU_ADAPTER_H

#include "../models/imu_sample.h"

/* Result of an IMU operation. */
typedef enum {
    IMU_STATUS_OK = 0,                  /* Success. */
    IMU_STATUS_COMMUNICATION_ERROR,     /* Sensor communication failed. */
    IMU_STATUS_TIMEOUT,                 /* Time limit exceeded. */
    IMU_STATUS_INVALID_DATA,            /* Data failed validation. */
    IMU_STATUS_WRONG_IDENTITY,         /* Sensor ID does not match the expected device. */
    IMU_STATUS_NOT_READY,              /* Required operation is not configured. */
    IMU_STATUS_INVALID_ARGUMENT        /* Caller supplied an invalid argument. */
} ImuStatus;

typedef struct {
    ImuStatus (*read)(ImuSample *out_sample);
} ImuAdapter;

/* Fill a non-NULL sample on success; leave it unchanged on failure. */
/* Currently assumes one global sensor. */
ImuStatus imu_read(ImuSample *out_sample);



#endif /* IMU_ADAPTER_H */
