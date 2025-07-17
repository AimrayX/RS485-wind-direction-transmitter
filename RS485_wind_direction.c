#include "RS485_wind_direction.h"


#include <stdio.h>
#include <stdlib.h>

int rs485_wind_direction_begin(rs485_wind_direction_t *dev) {
    rs485_wind_direction_init(dev);
    return 0;
}

int rs485_wind_direction_init(rs485_wind_direction_t *dev) {
    dev->ctx = modbus_new_rtu("/dev/ttyACM0", 9600, 'N', 8, 1);
    if (dev->ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    // Set the Modbus slave ID
    modbus_set_slave(dev->ctx, 2);

    // Open the serial port
    if (modbus_connect(dev->ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(dev->ctx);
        return -1;
    }
}

int rs485_wind_direction_read_register(rs485_wind_direction_t *dev) {
     dev->rc = modbus_read_registers(dev->ctx, 0x0000, 1, tab_reg);
    if (dev->rc == -1) {
        fprintf(stderr, "Failed to read: %s\n", modbus_strerror(errno));
        modbus_close(dev->ctx);
        modbus_free(dev->ctx);
        return -1;
    }
}

int rs485_wind_direction_close(rs485_wind_direction_t *dev) {
    modbus_close(dev->ctx);
    modbus_free(dev->ctx);
    return 0;
}