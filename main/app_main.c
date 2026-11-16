#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "platform/board.h"
#include "platform/i2c_bus.h"
#include "adapters/mpu6050_adapter.h"

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

    /* Hand the sensor adapter: which device, and how to read and write its registers. */
    Mpu6050 imu = {
        .context = s_imu,
        .read_register = i2c_bus_read_register,
        .write_register = i2c_bus_write_register,
        .read_registers = i2c_bus_read_registers
    };

    /* Raw read of WHO_AM_I so we can see the actual byte. */
    uint8_t who_am_i = 0;
    ImuStatus status = imu.read_register(imu.context, MPU6050_REG_WHO_AM_I, &who_am_i);
    printf("WHO_AM_I read: status %d, value 0x%02X\n", status, who_am_i);

    /* Your identity check from vulture_core. */
    status = mpu6050_check_identity(&imu);
    printf("Identity check: status %d\n", status);
    if (status != IMU_STATUS_OK) {
        return;
    }

    /* First register WRITE: PWR_MGMT_1 <- 0x01 (awake, gyro clock), then read back. */
    ImuStatus wake_status = mpu6050_wake(&imu);
    printf("Wake: status %d\n", wake_status);
    if (wake_status != IMU_STATUS_OK) {
        return;
    }

    /* Read and print raw motion data every 100 ms. Tilt the board and watch it change. */
    Mpu6050RawSample sample;
    while (1) {
        status = mpu6050_read_raw(&imu, &sample);
        if (status == IMU_STATUS_OK) {
            printf("accel %6d %6d %6d | gyro %6d %6d %6d\n",
                   sample.accel[0], sample.accel[1], sample.accel[2],
                   sample.gyro[0], sample.gyro[1], sample.gyro[2]);
        } else {
            printf("read_raw failed: status %d\n", status);
        }

        /* Sleep this task for 100 ms; FreeRTOS runs other work meanwhile. */
        vTaskDelay(pdMS_TO_TICKS(100));
    }

}
