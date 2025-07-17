#ifndef _RS485_WIND_DIRECTION_H_
#define _RS485_WIND_DIRECTION_H_

#include <modbus.h>

typedef struct rs485_wind_direction
{
    modbus_t *ctx;
    uint16_t tab_reg[32];
    int rc;
} rs485_wind_direction_t;

int rs485_wind_direction_begin();
int rs485_wind_direction_init();
int rs485_wind_direction_read_register();
int rs485_wind_direction_close();


#endif