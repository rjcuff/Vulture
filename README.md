# Vulture

A flight controller written in C for the ESP32-S3, built from scratch to learn
sensor communication, attitude estimation, and quadcopter control.
Build log: [ryancuff.com](https://ryancuff.com)

## Hardware

| Part | Notes |
|---|---|
| ESP32-S3-DevKitC-1 (N16R8) | 16 MB flash, 8 MB octal PSRAM (GPIO35–37 reserved) |
| HW-123 / GY-521 (MPU-6050) | 3-axis accel + gyro, I2C, on-board regulator and pull-ups |

### Wiring

| ESP32-S3 | Wire | MPU-6050 |
|---|---|---|
| 3V3 | red | VCC |
| GND | teal | GND |
| GPIO4 | yellow | SDA |
| GPIO5 | orange | SCL |
| — | — | AD0 floating (pulled low on board → address `0x68`) |

Pin assignments live in one place: `components/vulture_platform/include/platform/board.h`.

## Layout

```text
main/
└── app_main.c                  # Startup sequence only
components/
├── vulture_core/               # Platform-independent C. No ESP-IDF includes.
│   ├── models/                 #   Vector3f, ImuSample
│   └── adapters/               #   ImuStatus, sensor adapters (MPU identity check)
└── vulture_platform/           # ESP-IDF glue. Everything that touches the chip.
    ├── include/platform/
    │   ├── board.h             #   Pin map and device addresses
    │   └── i2c_bus.h           #   Shared I2C bus API
    └── i2c_bus.c
```

**Rule:** `vulture_core` never includes ESP-IDF headers, so it can be compiled and
unit-tested on a laptop. Sensor adapters reach hardware only through function
pointers (for example `Mpu9250ReadRegister`) that `vulture_platform` supplies.

## Build

Requires ESP-IDF v6.1.

```bash
get_idf                         # in every new terminal
idf.py set-target esp32s3       # once
idf.py build
idf.py -p /dev/cu.usbmodem<port> flash monitor   # Ctrl+] to exit
```

## Progress

- [x] ESP-IDF project skeleton, first flash
- [x] I2C bus on GPIO4/5, MPU added at `0x68`
- [x] MPU ACKs a probe
- [ ] Register read over I2C (`i2c_bus_read_register`)
- [ ] Read WHO_AM_I (`0x75`) and verify the chip
- [ ] Read raw accel/gyro (`0x3B`–`0x48`)
- [ ] Convert to `ImuSample` units
- [ ] Attitude estimation
- [ ] Control + motor mixing
