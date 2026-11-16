#ifndef MPU9250_ADAPTER_H
#define MPU9250_ADAPTER_H

#include "imu_adapter.h"

/* A type for pointers to register-reading functions, not a function body. */
/* The selected function returns ImuStatus: success or an error. */
typedef ImuStatus (*Mpu9250ReadRegister)(
    void *context,             /* Connection state; the reader decides whether NULL is valid. */
    uint8_t register_address,  /* Which register to read, e.g. 0x75. */
    uint8_t *out_value         /* Where the function stores the received byte. */
);

/* One sensor's connection state and register-reading function. */
typedef struct {
    void *context;                     /* Passed to the reader as its connection state. */
    Mpu9250ReadRegister read_register;  /* Function to call when reading a register. */
} Mpu9250;

ImuStatus mpu9250_check_identity(Mpu9250 *device);

#endif