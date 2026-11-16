#ifndef MPU6050_ADAPTER_H
#define MPU6050_ADAPTER_H

#include <stddef.h>
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

/*
 * Motion data block: 14 consecutive registers, 0x3B-0x48 (registers 59-72).
 * Seven big-endian int16 values, each as [high byte, low byte]:
 *   0x3B/3C ACCEL_X   0x3D/3E ACCEL_Y   0x3F/40 ACCEL_Z
 *   0x41/42 TEMP
 *   0x43/44 GYRO_X    0x45/46 GYRO_Y    0x47/48 GYRO_Z
 * Read all 14 in one burst so every value comes from the same sample.
 */
#define MPU6050_REG_ACCEL_XOUT_H    0x3Bu   /* Start of the motion data block. */
#define MPU6050_MOTION_DATA_BYTES   14u

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

/* A type for pointers to multi-register (burst) read functions. */
typedef ImuStatus (*Mpu6050ReadRegisters)(
    void *context,             /* Connection state, same as the reader's. */
    uint8_t start_register,    /* First register to read, e.g. MPU6050_REG_ACCEL_XOUT_H. */
    uint8_t *out_buffer,       /* Where the function stores the received bytes. */
    size_t length              /* How many consecutive registers to read. */
);

/* One sensor's connection state and its register read/write functions. */
typedef struct {
    void *context;                     /* Passed to the reader as its connection state. */
    Mpu6050ReadRegister     read_register; /* Function to call when reading a register. */
    Mpu6050WriteRegister    write_register; /* Function to call when writing a register. */
    Mpu6050ReadRegisters    read_registers; /* Function to call for burst reads. */
} Mpu6050;

/* One raw reading, straight from the registers. Signed: tilting one way goes negative. */
typedef struct {
    int16_t accel[3];      /* X, Y, Z in raw counts. */
    int16_t temperature;   /* Raw counts. */
    int16_t gyro[3];       /* X, Y, Z in raw counts. */
} Mpu6050RawSample;

/* Read WHO_AM_I and confirm the chip is an MPU-6050. */
ImuStatus mpu6050_check_identity(Mpu6050 *device);

/* Take the chip out of sleep and select the gyro clock, then read back to verify. */
ImuStatus mpu6050_wake(Mpu6050 *device);

ImuStatus mpu6050_read_raw(Mpu6050 *device, Mpu6050RawSample *out);

#endif /* MPU6050_ADAPTER_H */
