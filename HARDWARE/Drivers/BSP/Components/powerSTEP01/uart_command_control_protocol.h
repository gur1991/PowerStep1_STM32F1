#ifndef _UART_COMMAND_CONTROL_H_INCLUDED
#define _UART_COMMAND_CONTROL_H_INCLUDED

#ifdef __cplusplus
 extern "C" {
#endif 
#include "stdint.h"	 
#include "powerstep01.h" 
#include "motor.h"
#include "x_nucleo_ihmxx.h"	 
#include "stdio.h"	 
#include "sys.h"
uint8_t motorInit(void);	 

#define OVER_UART_VALUE0 0x0d
#define OVER_UART_VALUE1 0x0a
	 
typedef enum Command_type{ 
		MOVE_TYPE=1,
		POWER_TYPE,
		REST_TYPE,
		SEND_COMMAND_AND_WAIT_NOTBUSY_TYPE,
		DELAY_TIME_TYPE,
	
		ONE_DEVICE_MOVE_TYPE,
		ONE_DEVICE_WAIT_TYPE,
		ONE_DEVICE_GET_POS_TYPE,
		ONE_DEVICE_SET_MARK_TYPE,
	
		GET_PARAM_TYPE,
		SET_PARAM_TYPE,
		SElECT_STEP_MODE_TYPE,
	
		GET_LIGHT_LEVEL_TYPE=21,//light sensor level
		CHEMINERT_C52_C55_TYPE,
}Command_type_t;


typedef enum Command_Cheminert_type{ 
		CHEMINERT_C52_CP=25,//actuator 
		CHEMINERT_C52_CCA,
		CHEMINERT_C52_CCB,
		CHEMINERT_C52_CWA,
		CHEMINERT_C52_CWB,
		CHEMINERT_C52_DT,
		CHEMINERT_C52_GOA,
		CHEMINERT_C52_GOB,
		CHEMINERT_C52_MD,
		CHEMINERT_C52_SB,
		CHEMINERT_C52_SN,
		CHEMINERT_C52_TO,
		CHEMINERT_C52_VR,
	
		CHEMINERT_C55_CP,
		CHEMINERT_C55_CC1,
		CHEMINERT_C55_CC2,
		CHEMINERT_C55_CC3,
		CHEMINERT_C55_CC4,
		CHEMINERT_C55_CC5,
		CHEMINERT_C55_CC6,
		CHEMINERT_C55_CW1,
		CHEMINERT_C55_CW2,
		CHEMINERT_C55_CW3,
		CHEMINERT_C55_CW4,
		CHEMINERT_C55_CW5,
		CHEMINERT_C55_CW6,
		CHEMINERT_C55_GH,
		CHEMINERT_C55_GO1,
		CHEMINERT_C55_GO2,
		CHEMINERT_C55_GO3,
		CHEMINERT_C55_GO4,
		CHEMINERT_C55_GO5,
		CHEMINERT_C55_GO6,
		CHEMINERT_C55_MD,
		CHEMINERT_C55_MN,
		CHEMINERT_C55_SB,
		CHEMINERT_C55_SD,
		CHEMINERT_C55_SDCC,
		CHEMINERT_C55_SDCW,
		CHEMINERT_C55_SN,
		CHEMINERT_C55_VR,
		
}Command_Cheminert_type_t;
typedef union{ 
	struct{
		uint8_t devices;//0,1,2
		powerstep01_Commands_t types;//gpowerstep01_Commands_t
		int32_t steps;//+forward -backward
	}request;
	struct{
		uint8_t ret;
	}response;
}move_type_t;

typedef union{ 
	struct{
		uint8_t devices;//0,1,2
		uint8_t power;//1--start 0--stop
	}request;
	struct{
		uint8_t ret;
	}response;
}power_type_t;

typedef union{ 
	struct{
		uint8_t devices;//0,1,2
	}request;
	struct{
		uint8_t ret;
	}response;
}rest_pos_type_t;

typedef union{ 
	struct{
		uint8_t Nor;//±£¡ÙŒª
	}request;
	struct{
		uint8_t ret;
	}response;
}	send_command_and_wait_no_busy_type_t;

typedef union{
struct{
		uint16_t time_ms;
}request;
struct{
		uint8_t ret;
}response;
}delay_time_type_t;

typedef union{ 
struct{
		uint8_t devices;//0,1,2
		int32_t steps;//+forward -backward
}request;
struct{
		uint8_t ret;
}response;
}one_device_move_type_t;

typedef union{
struct{
		uint8_t devices;//0,1,2
}request;
struct{
		uint8_t ret;
}response;
}one_device_wait_type_t;

typedef union{ 
	struct{
		uint8_t devices;//0,1,2
}request;
struct{
		int32_t pos;
		uint8_t ret;	
}response;
}one_device_get_pos_type_t;

typedef union{
struct{
		uint8_t devices;//0,1,2
		int32_t pos;
}request;
struct{
		uint8_t ret;
}response;
}one_device_set_mark_type_t;

typedef union{
struct{
		uint8_t devices;//0,1,2
		powerstep01_Registers_t para;
}request;
struct{
		powerstep01_Registers_t result_para;
		uint8_t ret;
}response;
}get_para_type_t;

typedef union{
struct{
		uint8_t devices;//0,1,2
		powerstep01_Registers_t para;
		uint32_t value;
}request;
struct{
		uint8_t ret;
}response;
}set_para_type_t;

typedef union{ 
struct{
		uint8_t devices;//0,1,2
		motorStepMode_t StepMode;
}request;
struct{
		uint8_t ret;
}response;
}select_step_mode_t;

//light sensor
typedef union{ 
struct{
		uint8_t number;//0,1,2...
}request;
struct{
		uint8_t ret;
		uint8_t value;
}response;
}get_light_sensor_level_t;

//actuator
typedef union{ 
struct{
		Command_Cheminert_type_t para;
	  uint16_t timeout;
}request;
struct{
		uint8_t ret;
		uint8_t size;
		uint8_t buf[64];
}response;
}cheminert_c52_c55_type_t;




typedef struct{
		Command_type_t type;
		union {
				move_type_t move;
				power_type_t power;
				rest_pos_type_t rest_post;
				send_command_and_wait_no_busy_type_t send_and_wait;
				delay_time_type_t delay;
				one_device_move_type_t one_device_move;
				one_device_wait_type_t one_device_wait;
				one_device_get_pos_type_t  one_device_get_pos;
				one_device_set_mark_type_t one_device_set_mark;
				get_para_type_t  get_para;
				set_para_type_t  set_para;
				select_step_mode_t  select_step_mode;
			
				get_light_sensor_level_t get_light_sensor_level;
				cheminert_c52_c55_type_t cheminert_c52_c55;
		}CommandPowerStep1;
		u8 OverReceiveFlag[2];
}Powerstep1_contorl_motor_command_t;



	 
#endif 