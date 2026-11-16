#ifndef MPU6050_ADAPTER_H
#define MPU6050_ADAPTER_H

#include <stdint.h>
#include "imu_adapter.h"

/* MPU-6050 register map (datasheet: "MPU-6000/6050 Register Map"). */
#define MPU6050_REG_WHO_AM_I    0x75u   /* Identity register. */

/* Expected WHO_AM_I contents. This is the chip's ID, not its I2C address,
 * even though both happen to be 0x68 on the 6050. */
#define MPU6050_WHO_AM_I_VALUE  0x68u

#define MPU6050_REG_PWR_MGMT_1  0x6Bu   /* Power management 1 (register 107). */

/* PWR_MGMT_1 value to wake the chip: SLEEP (bit 6) = 0, CLKSEL (bits 2-0) = 1
 * (PLL with X-axis gyro reference, more stable than the internal 8 MHz oscillator).
 * Power-on default is 0x40: SLEEP = 1. */
#define MPU6050_PWR_MGMT_1_WAKE 0x01u

/* A type for pointers to register-reading functions, not a function body. */
/* The selected function returns ImuStatus: success or an error. */
typedef ImuStatus (*Mpu6050ReadRegister)(
    void *context,             /* Connection state; the reader decides whether NULL is valid. */
    uint8_t register_address,  /* Which register to read, e.g. MPU6050_REG_WHO_AM_I. */
    uint8_t *out_value         /* Where the function stores the received byte. */
);

/* A type for pointers to register-writing functions. */
typedef ImuStatus (*Mpu6050WriteRegister)(
    void *context,             /* Connection state, same as the reader's. */
    uint8_t register_address,  /* Which register to write, e.g. MPU6050_REG_PWR_MGMT_1. */
    uint8_t value              /* The byte to store there. */
);

/* One sensor's connection state and its register read/write functions. */
typedef struct {
    void *context;                     /* Passed to the reader as its connection state. */
    Mpu6050ReadRegister     read_register; /* Function to call when reading a register. */
    Mpu6050WriteRegister    write_register; /* Function to call when writing a register. */
} Mpu6050;

/* Read WHO_AM_I and confirm the chip is an MPU-6050. */
ImuStatus mpu6050_check_identity(Mpu6050 *device);

/* Take the chip out of sleep and select the gyro clock, then read back to verify. */
ImuStatus mpu6050_wake(Mpu6050 *device);

#endif /* MPU6050_ADAPTER_H */
