#ifndef __MASTER_UART_CONTROL_INTERFACE_H
#define __MASTER_UART_CONTROL_INTERFACE_H
#include "uart_command_control_protocol.h"
#include "uart2.h"
uint8_t master_powerStep01_move_command(move_type_t data);

uint8_t master_get_light_sensor_level(get_light_sensor_level_t data,uint8_t*value);
#endif

