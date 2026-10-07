# RS485 Wind Direction Transmitter

A lightweight C library for reading wind direction data from an RS485 Modbus RTU transmitter using a USB-to-RS485 adapter on Linux.

This project exposes a small API for initializing the Modbus connection, reading a wind direction register, and closing the device cleanly.

## Features

- Modbus RTU communication over RS485
- Linux-friendly device path support (for example `/dev/ttyACM0`)
- Wind direction reading from the transmitter register
- Built-in correction formula to account for sensor calibration offset
- Simple C API for embedding in other projects

## Hardware assumptions

The library is written for a transmitter configured as:

- Baud rate: 9600
- Parity: None
- Data bits: 8
- Stop bits: 1
- Modbus slave ID: 2
- Device path: `/dev/ttyACM0` (adjust as needed for your USB adapter)

## Repository layout

- `RS485_wind_direction.h` — public API definitions
- `RS485_wind_direction.c` — Modbus connection and sensor-reading logic
- `LICENSE` — MIT license

## Installation

This project depends on `libmodbus` and the math library.

On Debian/Ubuntu-based systems:

```bash
sudo apt-get install libmodbus-dev
```

If needed, install the build-essential tools as well:

```bash
sudo apt-get install build-essential
```

## Quick start

Example usage:

```c
#include <stdio.h>
#include "RS485_wind_direction.h"

int main(void) {
    rs485_wind_direction_t sensor = {0};

    if (rs485_wind_direction_begin(&sensor) != 0) {
        fprintf(stderr, "Failed to initialize the wind direction sensor\n");
        return 1;
    }

    if (rs485_wind_direction_read_register(&sensor) != 0) {
        fprintf(stderr, "Failed to read wind direction\n");
        rs485_wind_direction_close(&sensor);
        return 1;
    }

    printf("Wind direction: %.1f degrees\n", sensor.wind_direction);

    rs485_wind_direction_close(&sensor);
    return 0;
}
```

Compile it with:

```bash
gcc -Wall -Wextra -std=c11 main.c RS485_wind_direction.c -lmodbus -lm -o wind_direction
```

## API

```c
typedef struct rs485_wind_direction
{
    modbus_t *ctx;
    double wind_direction;
} rs485_wind_direction_t;
```

Functions:

```c
int rs485_wind_direction_begin(rs485_wind_direction_t *dev);
int rs485_wind_direction_init(rs485_wind_direction_t *dev);
int rs485_wind_direction_read_register(rs485_wind_direction_t *dev);
int rs485_wind_direction_close(rs485_wind_direction_t *dev);
```

### Function behavior

- `rs485_wind_direction_begin()` initializes the device and prepares the Modbus RTU interface.
- `rs485_wind_direction_init()` creates the Modbus context and connects to the serial device.
- `rs485_wind_direction_read_register()` reads the raw register value and converts it to a wind direction in degrees.
- `rs485_wind_direction_close()` closes the Modbus connection and frees resources.

## Reading logic

The device is read from Modbus register `0x0000`.

The raw register value is converted into a wind direction using a calibration curve defined in the source code. This compensates for a known offset in the transmitter output and normalizes the result to a 0–360° wind direction value.

## Notes

- Adjust the serial port path if your USB adapter appears under a different device name such as `/dev/ttyUSB0`.
- If your sensor uses a different Modbus slave address or serial configuration, update the values in `rs485_wind_direction_init()`.
- The library is deliberately minimal and intended to be easy to integrate into embedded or Linux-based monitoring systems.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
