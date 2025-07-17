#include "RS485_wind_direction.h"


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

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

    modbus_set_slave(dev->ctx, 2);

    if (modbus_connect(dev->ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(dev->ctx);
        return -1;
    }
    return 0;
}

int rs485_wind_direction_read_register(rs485_wind_direction_t *dev) {
    uint16_t tab_reg[32];
    int rc = modbus_read_registers(dev->ctx, 0x0000, 1, tab_reg);
    if (rc == -1) {
        fprintf(stderr, "Failed to read: %s\n", modbus_strerror(errno));
        return -1;
    }
    
    // calculate with correction formula since mine was oddly of by roughly this polinomial function
    double result = 10 * round((-0.00006 * pow((double)tab_reg[0], 2.0) + 1.20048 * (double)tab_reg[0] + 10.76594) / 100.0);
    if (result > 355.0) {
        dev->wind_direction = 0.0;
    } else {
        dev->wind_direction = result;
    }
    
    return 0;
}

int rs485_wind_direction_close(rs485_wind_direction_t *dev) {
    modbus_close(dev->ctx);
    modbus_free(dev->ctx);
    return 0;
}