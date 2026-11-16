#include "platform/i2c_bus.h"
#include "platform/board.h"

/* Upper bound on any single transaction. A one-byte read at 100 kHz takes ~0.3 ms. */
#define I2C_BUS_TIMEOUT_MS  (5 * 100)

/* The one bus instance. Private: other files go through the functions below. */
static i2c_master_bus_handle_t s_bus;

esp_err_t i2c_bus_init(void)
{
    if (s_bus != NULL) {
        return ESP_ERR_INVALID_STATE;   /* Already initialized. */
    }

    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = BOARD_I2C_SDA,
        .scl_io_num = BOARD_I2C_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        /* HW-123 has its own pull-ups; the internal ~45k ones just add margin. */
        .flags.enable_internal_pullup = true,
    };

    return i2c_new_master_bus(&bus_cfg, &s_bus);
}

esp_err_t i2c_bus_add_device(uint16_t address, uint32_t scl_speed_hz,
                             i2c_master_dev_handle_t *out_dev)
{
    if (s_bus == NULL) {
        return ESP_ERR_INVALID_STATE;   /* i2c_bus_init() not called. */
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = scl_speed_hz,
        /* ACK checking stays on so a missing device returns an error. */
    };

    return i2c_master_bus_add_device(s_bus, &dev_cfg, out_dev);
}

esp_err_t i2c_bus_probe(uint16_t address)
{
    if (s_bus == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_probe(s_bus, address, I2C_BUS_TIMEOUT_MS);
}

ImuStatus i2c_bus_read_register(void *context, uint8_t register_address,
                                uint8_t *out_value)
{
    /*
     * TODO(Ryan): step 3, this one is yours.
     *
     * 1. Cast context back to an i2c_master_dev_handle_t.
     * 2. Call i2c_master_transmit_receive(dev,
     *        <pointer to the register byte>, <how many bytes to write>,
     *        <where to read into>,           <how many bytes to read>,
     *        I2C_BUS_TIMEOUT_MS);
     * 3. Map esp_err_t to ImuStatus:
     *        ESP_OK          -> IMU_STATUS_OK
     *        ESP_ERR_TIMEOUT -> IMU_STATUS_TIMEOUT
     *        anything else   -> IMU_STATUS_COMMUNICATION_ERROR
     *
     * Until then it reports "not ready" so the project still builds.
     */
    (void)context;
    (void)register_address;
    (void)out_value;
    return IMU_STATUS_NOT_READY;
}
