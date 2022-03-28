#ifndef _STEP01_MANY_TEST_H
#define _STEP01_MANY_TEST_H

#include "step01.h"
#include "uart_command_control_protocol.h"
#include "config.h"
#include "drv8434.h"


//Pecker
//Camel

typedef uint32_t FACTORY_TYPE;
#define set_bit(x,b) (x) |= (1U<<(b))

enum{
LIGHT_NONE_STATE=0, 	
LIGHT_HAVE_STATE		 
};

enum {
  ERROR_QT_BASE = 50,
	ERROR_CAMEL_BOARD_COM,
	ERROR_PECKER_BOARD_COM,
	ERROR_HUMINGBIRD_BOARD_COM,
	ERROR_BABY18_COM,
	ERROR_PUMP_COM,
	ERROR_C55_COM,
	ERROR_C52_COM,
	ERROR_M6E_COM,
	ERROR_PRINTER_COM,
	ERROR_THERMOMETER_ALL_COM,

	ERROR_NEEDLE_LIFT,
	ERROR_MALLOC_SPACE,

	ERROR_M1_LIGHT,
	ERROR_M3_LIGHT,
	ERROR_M4_LIGHT,
	ERROR_M7_LIGHT,

	ERROR_NORMAL_CHECK_DRAIN_LIGHT,
	ERROR_NORMAL_LIGHT_NEXT,
	ERROR_NORMAL_CHECK_MIX_LIGHT,

	ERROR_BLANK_LIGHT,
	ERROR_LEFT_LIGHT,
	ERROR_WAIT_LIGHT,
	ERROR_NEXT_LIGHT,

	ERROR_M1_LIGHT_WORK,
	ERROR_HIGH_CHECK_LIGHT,

	ERROR_M8_LIGHT,
	ERROR_M9_LIGHT,
	ERROR_M10_LIGHT,
	ERROR_M11_LIGHT,
	ERROR_PUMP_PRESSURE,
	ERROR_TEMPERATURE,
	ERROR_BABY18_FORMART,
	//may be not use
	ERROR_THERMOMETER_BOARD_COM,
	ERROR_WEIGHT_ONE_COM,
	ERROR_WEIGHT_TWO_COM,
	ERROR_WEIGHT_THREE_COM,
	ERROR_WEIGHT_FOUR_COM,
	ERROR_SCAN_CONNECT,

	WARRNING_QT_BASE = 150,
	WARRNING_LOST_SAMPLE,
	WARRNING_REGENT_A,
	WARRNING_REGENT_B,
	WARRNING_REGENT_C,
	WARRNING_REGENT_H,
	WARRNING_THERMOMETER_ONE_COM,
	WARRNING_THERMOMETER_TWO_COM,
	WARRNING_PRINTER_WRITE,
	WARRNING_WEIGHT_A_MAX,
	WARRNING_WEIGHT_B_MAX,
	WARRNING_WEIGHT_C_MAX,
	WARRNING_WEIGHT_H_MAX,
};


void BSP_MotorControl_HardStop_Select(int deviceId,MOTOR_SPEED_type_t speed_type);

uint8_t RestSelectMotorOrginSelect(int deviceId,int lightNum, motorDir_t motorDir,uint32_t steps,MOTOR_SPEED_type_t speed_type);


void BSP_MotorControl_Move_Select(uint8_t deviceId, motorDir_t direction, uint32_t stepCount,MOTOR_SPEED_type_t speed_type);


FACTORY_TYPE process_motor_command_receive(Command_Package_t command,uint32_t* value);

void Motor_Move_And_Wait_Select(uint8_t deviceId, motorDir_t direction, uint32_t stepCount,MOTOR_SPEED_type_t speed_type);

void Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount);

uint8_t ClearAndCheckBlankPosition(void);

uint8_t ClearAndCheckWaitPosition(void);

uint8_t ReadyAndCheckNextPosition(void);

uint8_t ReadyAndCheckLeftPosition(void);

uint8_t LeftMoveTowardWaitPosition(void);

FACTORY_TYPE Rest_Drain_And_Wash_Motor_Orgin(void);

FACTORY_TYPE Rest_Transporter_Belt(void);

FACTORY_TYPE Rest_high_wheel(void);

extern u8 C55_connect_check(void);

extern u8 C52_connect_check(void);

FACTORY_TYPE C55_C52_connect_check(void);

void  Rest_Injection_Module_Motor(uint32_t up_Steps,uint32_t big_Steps,int time);

void set_DelayMs_Normal_Positon(uint16_t delayMs,uint16_t delayMsLastTwo);
/****************new start*******************************/


//MAX steps 11*10000
#define M8_LIGHT 17
#define M8_BIG_IN_OUT 8

#if USE_NEW_DESIGN
#define M8_BIG_IN BACKWARD
#define M8_BIG_OUT FORWARD
#else
#define M8_BIG_IN FORWARD//?
#define M8_BIG_OUT BACKWARD//?
#endif
//MAX steps 12*10000
#define M9_LIGHT 18
#define M9_IN_OUT 9

#if USE_NEW_DESIGN
#define M9_IN BACKWARD
#define M9_OUT FORWARD
#else
#define M9_IN FORWARD
#define M9_OUT BACKWARD
#endif


#define M11_LIGHT 19
#define M11_FAR_NEAR 11
#define M11_FAR FORWARD//?
#define M11_NEAR BACKWARD//?


#define M10_LIGHT 20
#define M10_UP_DOWM 10
#define M10_UP  BACKWARD//?
#define M10_DOWM FORWARD//?


#define M1_LIGHT_WORK 16
#define M1_LIGHT 15
#define M1_MIX_V 1
#define M1_MIX_V_UP  FORWARD
#define M1_MIX_V_DOWN BACKWARD


#define M2_MIX 2

#define M2_MIX_LEFT BACKWARD
#define M2_MIX_RIGHT FORWARD

/*************************************************************/
/*
blank  ------check-------next
-							-
-							-
-							-
left---------------------wait
*/
#define M4_LIGHT 2
#define M4_BLANK_NEXT 4
#define M4_BLANK_TO_NEXT  FORWARD
#define M4_NEXT_TO_BLANK BACKWARD


#define M6_BLANK_LEFT 6
#define M6_BLANK_TO_LEFT BACKWARD
#define M6_LEFT_TO_BLANK FORWARD

#define M5_WAIT_NEXT 5

#define M5_WAIT_TO_NEXT BACKWARD
#define M5_NEXT_TO_WAIT FORWARD


#define M3_LIGHT 1
#define M3_LEFT_WAIT 3
#define M3_LEFT_TO_WAIT  BACKWARD
#define M3_WAIT_TO_LEFT  FORWARD

#define BLANK_LIGHT 5
#define LEFT_LIGHT 6
#define WAIT_LIGHT 3
#define NEXT_LIGHT 4

#define HIGH_CHECK_LIGHT 10
#define M7_LIGHT 7
#define M7_HIGH_TURN 7
#define M7_FRONT_TURN   BACKWARD
#define M7_BACK_TURN  FORWARD

#define NORMAL_NEXT_LIGHT 8
#define NORMAL_CHECK_MIX_LIGHT 11
#define NORMAL_CHECK_DRAIN_LIGHT 9






/*******************new end**********************************/

/*****************************************************************/



#endif
