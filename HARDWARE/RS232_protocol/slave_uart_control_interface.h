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
#include "config.h"
#include "bl180.h"

uint8_t Rest_C55_C52_Position(void);

uint8_t Set_Pumps100_Press(int MaxPress,int MinPress);

uint8_t Balance_Chromatographic_Column(int IdleFlowSpeed);

uint8_t Gradient_control_buffer(int Work_Flow_Speed,int A_timeS,int B_timeS,int C_timeS,int A2_timeS);

void Gravity_Sensor_Setting(int weightA,int weightB,int weightC,int weightD);

uint8_t Set_Pumps100_FlowSpeed(int speed);


void protocol_handle_uart_powerstep01_plain_slave_cmd(void);





u8 test_actuator(Command_Cheminert_type_t type);

//void test_pump_s100(void);


#endif
