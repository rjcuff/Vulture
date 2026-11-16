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

ImuStatus mpu6050_wake(Mpu6050 *device) {
    if (device == NULL) {
        return IMU_STATUS_INVALID_ARGUMENT;
    }
    if (device->write_register == NULL || device->read_register == NULL) {
        return IMU_STATUS_NOT_READY;
    }

    ImuStatus write_status = device->write_register(device->context,
                                                    MPU6050_REG_PWR_MGMT_1,
                                                    MPU6050_PWR_MGMT_1_WAKE);
    if (write_status != IMU_STATUS_OK) {
        return write_status;
    }

    /* Read back to confirm the write landed. */
    uint8_t readback;
    ImuStatus read_status = device->read_register(device->context,
                                                  MPU6050_REG_PWR_MGMT_1,
                                                  &readback);
    if (read_status != IMU_STATUS_OK) {
        return read_status;
    }

    if (readback != MPU6050_PWR_MGMT_1_WAKE) {
        return IMU_STATUS_INVALID_DATA;
    }

    return IMU_STATUS_OK;
}
