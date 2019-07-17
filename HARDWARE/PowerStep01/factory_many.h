#ifndef _STEP01_MANY_TEST_H
#define _STEP01_MANY_TEST_H

#include "step01.h"
#include "uart_command_control_protocol.h"

void BSP_Motor_Control_Init(void);

void FactoryMotorTestMode_many(void);

void  CollectSampleTask(void);

void InjectLiquidTask(void);

void RestInjectAllPosition(void);

void MixingLiquidTask(void);

void process_motor_command_receive(Command_Package_t command);
#endif
