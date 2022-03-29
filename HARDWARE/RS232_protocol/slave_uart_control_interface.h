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
#include "bl180.h"
#include "S1125.h"
#include "m6e_apply.h"
#include "rs232_change_protocol.h"
#include "config.h"
#include "check_rs232.h"
#include "export_liquid.h"

uint32_t Rest_C55_C52_Position(void);



uint8_t Gradient_control_buffer(int Work_Flow_Speed,int A_timeS,int B_timeS,int C_timeS,int A2_timeS);

void Gravity_Sensor_Setting(int weightA,int weightB,int weightC,int weightD);

void protocol_handle_uart_powerstep01_plain_slave_cmd(void);



uint8_t Run_S100_Pump(void);

uint8_t Stop_S100_Pump(void);

uint8_t Set_Pumps100_FlowSpeed(int speed);

uint8_t Set_Pumps100_Press(int MinPress,int MaxPress);




uint8_t Balance_Chromatographic_Column(int IdleFlowSpeed);

u8 test_actuator(Command_Cheminert_type_t type);



typedef struct
{
	void (*init)(void);
  uint8_t (*run)(void);
	uint8_t (*stop)(void);
	uint8_t (*setPress)(int max,int min);
	uint8_t (*setFlowSpeed)(int);
	int (*readPress)(void);
	uint8_t (*setMaxPress)(int max);
	uint8_t (*setMinPress)(int min);
	uint8_t (*connect)(void);
}Pump_t;

typedef enum{
	S100=0,
	S1125,
}PUMP_type;

extern Pump_t* PumpHandle;

void PumpChooseHandle(PUMP_type id);



enum {
	RUN_PUMP=0,
	STOP_PUMP,
	READ_PRESS,
	WRITE_MAX_PRESS,
	WRITE_MIN_PRESS,
	WRITE_FLOW_SPEED,
	WRITE_PRESS,
	CONNECT_TEST,
};




int pump_process_cmd( pump_type_t pump);

#endif
