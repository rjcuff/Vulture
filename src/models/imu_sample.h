#ifndef IMU_SAMPLE_H
#define IMU_SAMPLE_H

#include <stdint.h>
#include "vector3.h"

/* One IMU reading, shared by all sensor adapters. */
typedef struct {
    Vector3f acceleration_mps2;    /* XYZ acceleration in m/s^2. */
    Vector3f angular_velocity_dps; /* XYZ rotation rates in degrees/s. */
    uint32_t timestamp_us;        /* Sample time in microseconds; wraps. */
} ImuSample;

#endif /* IMU_SAMPLE_H */
