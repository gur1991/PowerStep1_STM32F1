#ifndef _STEP01_MANY_TEST_H
#define _STEP01_MANY_TEST_H

#include "step01.h"
#include "uart_command_control_protocol.h"



uint8_t process_motor_command_receive(Command_Package_t command);

void Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount);

uint8_t ClearAndCheckBlankPosition(void);

uint8_t ClearAndCheckWaitPosition(void);

uint8_t ReadyAndCheckNextPosition(void);

uint8_t ReadyAndCheckLeftPosition(void);

uint8_t LeftMoveTowardWaitPosition(void);

void Rest_Drain_And_Wash_Motor_Orgin(void);

void Rest_Transporter_Belt(void);

void Rest_high_wheel(void);

void RestAllMotorOrgin(void);

void First_Open_Motor_AutoCheck(void);

/***********************************************/
/*
 *????18CM
 *1000s = 3cm
 */
#define M5_LIGHT 12
#define M5_FAR_NEAR 5
#define M5_FAR BACKWARD//?
#define M5_NEAR FORWARD//?

/*
 *????8CM
 *17w s = 7cm
 */
#define M6_LIGHT 11
#define M6_UP_DOWM 6
#define M6_UP BACKWARD//?
#define M6_DOWM FORWARD//?

/*
 *??3.5cm
 *1w =0.5
 */
#define M10_LIGHT 16
#define M10_BIG_IN_OUT 10
#define M10_BIG_IN FORWARD//?
#define M10_BIG_OUT BACKWARD//?
/*
 * ??3.5cm
 * 3w =1
 */
#define M9_LIGHT 15
#define M9_IN_OUT 9
#define M9_IN FORWARD//?
#define M9_OUT BACKWARD//?



/*
 * ??6cm
 * 1w =0.5
 */
#define M7_LIGHT 14
#define M7_LIGHT_WORK 13
#define M7_MIX_V 7
#define M7_MIX_V_UP  BACKWARD
#define M7_MIX_V_DOWN FORWARD

/*
 * 15w
 */
#define M8_MIX 8
#define M8_MIX_LEFT FORWARD
#define M8_MIX_RIGHT BACKWARD

/*************************************************************/
/*
blank  ------check-------next
-							-
-							-
-							-
left---------------------wait
*/
#define M1_LIGHT 6
#define M1_BLANK_NEXT 1
#define M1_BLANK_TO_NEXT BACKWARD
#define M1_NEXT_TO_BLANK FORWARD


#define M2_BLANK_LEFT 2
#define M2_BLANK_TO_LEFT BACKWARD
#define M2_LEFT_TO_BLANK FORWARD

#define M3_WAIT_NEXT 3
#define M3_WAIT_TO_NEXT BACKWARD
#define M3_NEXT_TO_WAIT FORWARD

#define M4_LIGHT 3
#define M4_LEFT_WAIT 4
#define M4_LEFT_TO_WAIT  FORWARD
#define M4_WAIT_TO_LEFT  BACKWARD

#define BLANK_LIGHT 1
#define LEFT_LIGHT 2
#define WAIT_LIGHT 4
#define NEXT_LIGHT 5

#define NORMAL_CHECK_LIGHT 7//??????????


#define HIGH_CHECK_LIGHT 9//??????????

#define M11_LIGHT 10
#define M11_HIGH_TURN 11
#define M11_LEFT_TURN  FORWARD
#define M11_RIGHT_TURN  BACKWARD

#define PITCH_VALUE 1330
#define HIGH_PITCH_VALUE 1000
/*****************************************************************/

#endif
