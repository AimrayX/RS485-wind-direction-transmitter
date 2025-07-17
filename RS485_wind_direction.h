#ifndef RS485_WIND_DIRECTION_H
#define RS485_WIND_DIRECTION_H

#include <modbus/modbus.h>

typedef struct rs485_wind_direction
{
    modbus_t *ctx;
    double wind_direction;
} rs485_wind_direction_t;

int rs485_wind_direction_begin(rs485_wind_direction_t *dev);
int rs485_wind_direction_init(rs485_wind_direction_t *dev);
int rs485_wind_direction_read_register(rs485_wind_direction_t *dev);
int rs485_wind_direction_close(rs485_wind_direction_t *dev);


#endif