# ESP-IDF glue: everything that touches the chip lives here.
# vulture_core stays platform-independent; this component adapts it to the ESP32-S3.
idf_component_register(
    SRCS "i2c_bus.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_i2c esp_driver_gpio vulture_core
)
