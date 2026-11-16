#include <stdio.h>
#include "driver/i2c_master.h"
#include "adapters/mpu9250_adapter.h"

#define VULTURE_I2C_SDA         GPIO_NUM_4
#define VULTURE_I2C_SCL         GPIO_NUM_5
#define MPU_I2C_ADDR            0x68
#define MPU_I2C_SCL_SPEED_HZ    (100 * 1000)
#define MPU_I2C_TIMEOUT_MS      (5 * 100)

/* File-scope so other functions in this file can use the bus later. */
static i2c_master_bus_handle_t s_bus;
static i2c_master_dev_handle_t s_mpu;

void app_main(void)
{
    printf("Vulture booting\n");

    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,                      /* which I2C controller (the S3 has 0 and 1) */
        .sda_io_num = VULTURE_I2C_SDA,                    /* your SDA pin */
        .scl_io_num = VULTURE_I2C_SCL,                    /* your SCL pin */
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,  /* true or false? */
    };

    i2c_device_config_t mpu_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = MPU_I2C_ADDR,
        .scl_speed_hz = MPU_I2C_SCL_SPEED_HZ,
        .scl_wait_us = 0,
        .flags.disable_ack_check = false
    };

    esp_err_t err = i2c_new_master_bus(&bus_cfg, &s_bus);
    printf("I2C bus init: %s\n", esp_err_to_name(err));

    if (err != ESP_OK) {
        return;
    }

    esp_err_t mpu_err = i2c_master_bus_add_device(s_bus,&mpu_config,&s_mpu);
    printf("MPU add device: %s\n", esp_err_to_name(mpu_err));

    if (mpu_err != ESP_OK) {
        return;
    }

    esp_err_t probe_err = i2c_master_probe(s_bus, MPU_I2C_ADDR, MPU_I2C_TIMEOUT_MS);
    printf("MPU probe: %s\n", esp_err_to_name(probe_err));

    if (probe_err != ESP_OK) {
        return;
    }

}