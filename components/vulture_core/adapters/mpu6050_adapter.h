#ifndef MPU6050_ADAPTER_H
#define MPU6050_ADAPTER_H

#include <stdint.h>
#include "imu_adapter.h"

/* MPU-6050 register map (datasheet: "MPU-6000/6050 Register Map"). */
#define MPU6050_REG_WHO_AM_I    0x75u   /* Identity register. */

/* Expected WHO_AM_I contents. This is the chip's ID, not its I2C address,
 * even though both happen to be 0x68 on the 6050. */
#define MPU6050_WHO_AM_I_VALUE  0x68u

/* A type for pointers to register-reading functions, not a function body. */
/* The selected function returns ImuStatus: success or an error. */
typedef ImuStatus (*Mpu6050ReadRegister)(
    void *context,             /* Connection state; the reader decides whether NULL is valid. */
    uint8_t register_address,  /* Which register to read, e.g. MPU6050_REG_WHO_AM_I. */
    uint8_t *out_value         /* Where the function stores the received byte. */
);

/* One sensor's connection state and register-reading function. */
typedef struct {
    void *context;                     /* Passed to the reader as its connection state. */
    Mpu6050ReadRegister read_register; /* Function to call when reading a register. */
} Mpu6050;

/* Read WHO_AM_I and confirm the chip is an MPU-6050. */
ImuStatus mpu6050_check_identity(Mpu6050 *device);

#endif /* MPU6050_ADAPTER_H */
