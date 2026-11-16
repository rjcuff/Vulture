#include <stdio.h>
#include "platform/board.h"
#include "platform/i2c_bus.h"
#include "adapters/mpu9250_adapter.h"

/* Startup sequence. Each step stops the boot if it fails. */

static i2c_master_dev_handle_t s_imu;

void app_main(void)
{
    printf("Vulture booting\n");

    esp_err_t err = i2c_bus_init();
    printf("I2C bus init: %s\n", esp_err_to_name(err));
    if (err != ESP_OK) {
        return;
    }

    err = i2c_bus_add_device(BOARD_IMU_I2C_ADDR, BOARD_IMU_I2C_SPEED_HZ, &s_imu);
    printf("IMU add device: %s\n", esp_err_to_name(err));
    if (err != ESP_OK) {
        return;
    }

    err = i2c_bus_probe(BOARD_IMU_I2C_ADDR);
    printf("IMU probe: %s\n", esp_err_to_name(err));
    if (err != ESP_OK) {
        return;
    }

    /* TODO(Ryan): step 4, build an Mpu9250 with s_imu + i2c_bus_read_register,
     * read WHO_AM_I, and print the raw byte. */
}
