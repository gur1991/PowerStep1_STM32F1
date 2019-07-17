#ifndef __SLAVE_UART_CONTROL_INTERFACE_H
#define __SLAVE_UART_CONTROL_INTERFACE_H
#include "uart_command_control_protocol.h"
#include <stdbool.h>
#include "uart4.h"
#include "pump_s100.h"
#include "weight.h"
void protocol_handle_uart_powerstep01_plain_slave_cmd(void);

#endif
