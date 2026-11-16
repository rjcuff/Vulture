#ifndef VULTURE_BOARD_H
#define VULTURE_BOARD_H

#include "driver/gpio.h"

/*
 * Hardware facts for this build: ESP32-S3-DevKitC-1 (N16R8) + HW-123 MPU-6050.
 * Rewiring means editing this file only. See README for the wiring table.
 */

/* Shared I2C bus. */
#define BOARD_I2C_SDA           GPIO_NUM_4
#define BOARD_I2C_SCL           GPIO_NUM_5

/* IMU on the I2C bus. AD0 floats low on the HW-123, so 0x68. */
#define BOARD_IMU_I2C_ADDR      0x68
#define BOARD_IMU_I2C_SPEED_HZ  (100 * 1000)

#endif /* VULTURE_BOARD_H */
