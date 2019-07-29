#ifndef __SLAVE_UART_CONTROL_INTERFACE_H
#define __SLAVE_UART_CONTROL_INTERFACE_H
#include "uart_command_control_protocol.h"
#include <stdbool.h>
#include "uart4.h"
#include "pump_s100.h"
#include "weight.h"
#include "factory_many.h"
#include "keep_temperature.h"
#include "temperature.h"
#include "uart_choose.h"
#include "Cheminert_c52.h"

void protocol_handle_uart_powerstep01_plain_slave_cmd(void);

u8 test_actuator(Command_Cheminert_type_t type);

void test_pump_s100(void);
#endif
