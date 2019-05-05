#ifndef __MASTER_UART_CONTROL_INTERFACE_H
#define __MASTER_UART_CONTROL_INTERFACE_H
#include "uart_command_control_protocol.h"
#include "rs485.h"
uint8_t master_powerStep01_move_command(move_type_t data);

#endif