#include <stddef.h>
#include "mpu6050_adapter.h"

ImuStatus mpu6050_check_identity(Mpu6050 *device) {
    if (device == NULL) {
        return IMU_STATUS_INVALID_ARGUMENT;
    }
    if (device->read_register == NULL) {
        return IMU_STATUS_NOT_READY;
    }

    uint8_t id;
    ImuStatus status = device->read_register(device->context, MPU6050_REG_WHO_AM_I, &id);

    /* Only inspect the byte after a successful read. */
    if (status != IMU_STATUS_OK) {
        return status;
    }

    if (id != MPU6050_WHO_AM_I_VALUE) {
        return IMU_STATUS_WRONG_IDENTITY;
    }

    return IMU_STATUS_OK;
}
