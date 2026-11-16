# Platform-independent flight code. No ESP-IDF dependencies on purpose:
# this component should compile with plain gcc on a laptop for unit tests.
idf_component_register(
    SRCS "adapters/mpu9250_adapter.c"
    INCLUDE_DIRS "."
)
