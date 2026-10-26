# Vulture

A flight controller project written in C for an ESP32-S3 and MPU-9250 IMU.
Built incrementally to learn sensor communication, attitude estimation, and
quadcopter control.

## Design

Sensor adapters will convert hardware-specific readings into a shared
`ImuSample`. The goal is to let compatible sensors supply the same measurement
format to the flight software.

Planned flow:

```text
I2C register reads -> MPU-9250 adapter -> ImuSample
    -> attitude estimation -> control corrections -> motor mixing
```

## Current files

```text
src/
├── models/
│   ├── vector3.h           # XYZ floating-point vector
│   └── imu_sample.h        # Acceleration, angular velocity, timestamp
└── adapters/
    ├── imu_adapter.h       # Status codes and initial sample-reader interface
    ├── mpu9250_adapter.h   # Register-reader callback, device state, identity-check declaration
    └── mpu9250_adapter.c   # WHO_AM_I register and expected identity constants
```

- `Vector3f` groups three floats: X, Y, and Z.
- `ImuSample` contains acceleration in m/s², angular velocity in degrees/s,
  and a 32-bit timestamp in microseconds. The timestamp wraps.
- `ImuStatus` distinguishes success, communication errors, timeouts, and invalid data.
- `ImuAdapter` currently holds a sample-reading function pointer. Its generic
  interface still needs per-device state support.
- `Mpu9250ReadRegister` defines a function-pointer type for reading one register byte.
- `Mpu9250` pairs a register-reading function with its connection-state pointer.

## Current milestone

The data types and initial interfaces are defined. The MPU-9250 identity-check
function is declared but has no implementation yet. Its intended behavior is:

1. Read one byte from `WHO_AM_I`, register `0x75`.
2. Return any communication error before examining the byte.
3. Compare a successfully received byte with the expected MPU-9250 identity, `0x71`.

No I2C transport, sensor initialization, motion sampling, estimator, or motor
control is implemented. There is no firmware build configuration yet, and no
hardware behavior has been verified. Axis directions and mounting conversion
still need to be defined.

## Next step

Implement the identity check using the device's register-reading callback.
Connecting that callback to actual I2C communication is a separate step.
