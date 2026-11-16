#include <stddef.h>
#include "mpu6050_adapter.h"

/* Combine two bytes, high first ("big-endian"), into one signed 16-bit value.
 *   high << 8  moves the high byte into the top half
 *   | low      fills in the bottom half
 *   (int16_t)  reads the result as signed, so 0xFF00-style values go negative */
static int16_t be16_to_int16(const uint8_t *bytes) {
    return (int16_t)((bytes[0] << 8) | bytes[1]);
}

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

ImuStatus mpu6050_read_raw(Mpu6050 *device, Mpu6050RawSample *out) {
    if (device == NULL || out == NULL) {
        return IMU_STATUS_INVALID_ARGUMENT;
    }
    if (device->read_registers == NULL) {
        return IMU_STATUS_NOT_READY;
    }

    /* One burst so all seven values come from the same sample. */
    uint8_t buf[MPU6050_MOTION_DATA_BYTES];
    ImuStatus status = device->read_registers(device->context,
                                              MPU6050_REG_ACCEL_XOUT_H,
                                              buf,
                                              MPU6050_MOTION_DATA_BYTES);
    if (status != IMU_STATUS_OK) {
        return status;
    }

    /* Each value is [high byte, low byte]. See the map in mpu6050_adapter.h. */
    out->accel[0]    = be16_to_int16(&buf[0]);    /* 0x3B/3C ACCEL_X */
    out->accel[1]    = be16_to_int16(&buf[2]);    /* 0x3D/3E ACCEL_Y */
    out->accel[2]    = be16_to_int16(&buf[4]);    /* 0x3F/40 ACCEL_Z */
    out->temperature = be16_to_int16(&buf[6]);    /* 0x41/42 TEMP    */
    out->gyro[0]     = be16_to_int16(&buf[8]);    /* 0x43/44 GYRO_X  */
    out->gyro[1]     = be16_to_int16(&buf[10]);   /* 0x45/46 GYRO_Y  */
    out->gyro[2]     = be16_to_int16(&buf[12]);   /* 0x47/48 GYRO_Z  */

    return IMU_STATUS_OK;
}
