#include <stddef.h>
#include "mpu9250_adapter.h"

/* Register containing the sensor identity. */
#define MPU9250_REG_WHO_AM_I    0x75u
/* Expected identity byte, not the configured I2C device address. */
#define MPU9250_WHO_AM_I_VALUE  0x71u

ImuStatus mpu9250_check_identity(Mpu9250 *device) {
    if (device == NULL) {
        return IMU_STATUS_INVALID_ARGUMENT;
    }
    if (device->read_register == NULL) {
        return IMU_STATUS_NOT_READY;
    }

    uint8_t id;
    ImuStatus status = device->read_register(device->context, MPU9250_REG_WHO_AM_I, &id);

    /* Only inspect the byte after a successful read. */
    if (status != IMU_STATUS_OK) {
        return status;
    }

    if (id != MPU9250_WHO_AM_I_VALUE) {
        return IMU_STATUS_WRONG_IDENTITY;
    }

    return IMU_STATUS_OK;
}
