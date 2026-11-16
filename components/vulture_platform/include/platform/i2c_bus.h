#ifndef VULTURE_I2C_BUS_H
#define VULTURE_I2C_BUS_H

#include <stdint.h>
#include "esp_err.h"
#include "driver/i2c_master.h"
#include "adapters/imu_adapter.h"

/* Bring up the shared I2C bus on the board's SDA/SCL pins. Call once at startup. */
esp_err_t i2c_bus_init(void);

/* Register a 7-bit device on the bus and return its handle through out_dev. */
esp_err_t i2c_bus_add_device(uint16_t address, uint32_t scl_speed_hz,
                             i2c_master_dev_handle_t *out_dev);

/* Check whether a device ACKs its address. No registers are read or written. */
esp_err_t i2c_bus_probe(uint16_t address);

/*
 * Read one register from a device: write the register address, repeated START,
 * read one byte. Matches Mpu6050ReadRegister so it can be plugged into an adapter.
 * context must be an i2c_master_dev_handle_t from i2c_bus_add_device.
 */
ImuStatus i2c_bus_read_register(void *context, uint8_t register_address,
                                uint8_t *out_value);

/*
 * Write one register on a device: START, [addr+W], [register_address], [value], STOP.
 * context must be an i2c_master_dev_handle_t from i2c_bus_add_device.
 */
ImuStatus i2c_bus_write_register(void *context, uint8_t register_address,
                                uint8_t value);

#endif /* VULTURE_I2C_BUS_H */
