#ifndef __DRV8434_GPIO_H
#define __DRV8434_GPIO_H

#include <stdio.h>
#include "config.h"
#include "sys.h"
//#include <stdbool.h>
#include "stdint.h"	 
#include "motor.h"
#include "x_nucleo_ihmxx.h"	 
#include "stdio.h"	 
#include "sys.h"
#include "powerstep01_target_config.h"
#include "x_nucleo_ihm03a1_stm32f1xx.h"
#include "powerstep01.h" 
#include "light.h"
#include "config.h"
#include "step01.h"
#include "factory_many.h"
#include <math.h>


void GP_DRV8434_MOTOR_GPIO_Config_Init(void);
void GP_DRV8434_Motor_Stop_Control_Timing(uint8_t deviceId);
void GP_DRV8434_Motor_Run_Control_Timing(uint8_t deviceId,motorDir_t direction);
void GP_Init_Drv8434_Speed_Config(void);
void GP_DRV8434_Motor_Move_Steps_Single(uint8_t deviceId, motorDir_t direction, uint32_t stepCount);
void GP_DRV8434_Motor_Move_Steps_Dual(uint8_t deviceId_1, motorDir_t direction_1,
													uint8_t deviceId_2, motorDir_t direction_2, uint32_t stepCount);
void GP_DRV8434_Motor_Move_Steps_Single_Soft_Stop(uint8_t deviceId, motorDir_t direction, uint32_t stepCount);
uint8_t GP_DRV8434_Motor_Move_Steps_Single_Check_Light(uint8_t deviceId, motorDir_t direction, uint32_t stepCount,u8 light);
int GP_DRV8434_From_DelayMS_Get_Steps_Count(int deviceId, int delayMs);


#define STEP_11_OUT PDout(12)
#define STEP_10_OUT PDout(13)
#define STEP_9_OUT PCout(8)
#define STEP_8_OUT PCout(9)
#define STEP_7_OUT PDout(13)
#define STEP_6_OUT PDout(14)
#define STEP_5_OUT PCout(8)//PDout(15) 
#define STEP_4_OUT PCout(6)
#define STEP_3_OUT PCout(7)
#define STEP_2_OUT PDout(15)//PCout(8) 
#define STEP_1_OUT PCout(9)



typedef struct
{
	int s;
	int e;
}GP_Drv_Speed_Config_Type;

typedef struct{
	
	GP_Drv_Speed_Config_Type M1;
	GP_Drv_Speed_Config_Type M2;
	GP_Drv_Speed_Config_Type M3;
	GP_Drv_Speed_Config_Type M4;
	
	GP_Drv_Speed_Config_Type M5;
	GP_Drv_Speed_Config_Type M6;
	GP_Drv_Speed_Config_Type M7;
	
	GP_Drv_Speed_Config_Type M8;
	GP_Drv_Speed_Config_Type M9;
	GP_Drv_Speed_Config_Type M10;
	GP_Drv_Speed_Config_Type M11;
}GP_Drv_Motor_Speed_Config_Group_t;

#endif


