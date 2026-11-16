# Vulture

A quadcopter flight controller written from scratch in C for the ESP32-S3.
Built to learn sensor communication, attitude estimation, and control.
Build log: [ryancuff.com](https://ryancuff.com)

## Hardware

| Part | Notes |
|---|---|
| ESP32-S3-DevKitC-1 (N16R8) | 16 MB flash, 8 MB octal PSRAM (GPIO35–37 reserved) |
| HW-123 / GY-521 (MPU-6050) | 3-axis accelerometer + gyroscope over I2C, on-board regulator and pull-ups |

### Wiring

| ESP32-S3 | MPU-6050 |
|---|---|
| 3V3 | VCC |
| GND | GND |
| GPIO4 | SDA |
| GPIO5 | SCL |
| — | AD0 floating (pulled low on board → I2C address `0x68`) |

All pin assignments and device addresses live in
`components/vulture_platform/include/platform/board.h`.

## Architecture

```text
main/
└── app_main.c                  # Startup sequence and main loop
components/
├── vulture_core/               # Platform-independent C. No ESP-IDF includes.
│   ├── models/                 #   Vector3f, ImuSample
│   └── adapters/               #   ImuStatus, MPU-6050 driver
└── vulture_platform/           # ESP-IDF glue. Everything that touches the chip.
    ├── include/platform/
    │   ├── board.h             #   Pin map and device addresses
    │   └── i2c_bus.h           #   Shared I2C bus API
    └── i2c_bus.c
```

`vulture_core` never includes ESP-IDF headers, so it can be compiled and unit-tested
on a laptop. Sensor drivers reach hardware only through function pointers
(`read_register`, `write_register`, `read_registers`) that `main` wires to
`vulture_platform` at startup.

### I2C layer (`vulture_platform`)

| Function | On the wire |
|---|---|
| `i2c_bus_read_register` | `START, addr+W, reg, RESTART, addr+R, 1 byte, STOP` |
| `i2c_bus_read_registers` | Same, reading *N* consecutive bytes in one transaction |
| `i2c_bus_write_register` | `START, addr+W, reg, value, STOP` |

### MPU-6050 driver (`vulture_core`)

| Function | What it does |
|---|---|
| `mpu6050_check_identity` | Reads `WHO_AM_I` (`0x75`), expects `0x68` |
| `mpu6050_wake` | Writes `PWR_MGMT_1` (`0x6B`) = `0x01` (awake, gyro clock), reads back to verify |
| `mpu6050_read_raw` | Burst-reads `0x3B`–`0x48` (14 bytes) into signed accel/temp/gyro counts |

## Boot sequence

```text
I2C bus init → add IMU → probe 0x68 → WHO_AM_I check → wake → read loop (10 Hz)
```

Each step stops the boot on failure and logs its result.

## Sensor notes

Measured on this board at the default ranges (±2 g = 16384 counts/g,
±250 °/s = 131 counts/(°/s)):

- **Accel Z offset ≈ +4000 counts (~0.24 g).** Flat reads ~+20500, upside down ~−12500.
  The midpoint gives the offset; half the span (~16500) matches the datasheet scale.
- **Gyro bias** at rest ≈ −285 / −65 / +45 counts (X / Y / Z), about −2.2 °/s on X.
- **Noise** ≈ ±150 counts on accel Z.

Calibration and unit conversion are next.

## Build

Requires ESP-IDF v6.1.

```bash
get_idf                                          # in every new terminal
idf.py set-target esp32s3                        # once
idf.py build
idf.py -p /dev/cu.usbmodem<port> flash monitor   # Ctrl+] to exit
```
