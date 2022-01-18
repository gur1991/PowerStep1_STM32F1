#ifndef __DRV8434_H
#define __DRV8434_H
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

#define nSLEEP_11_OUT PBout(12)
#define ENABLE_11_OUT PBout(13)
#define DIR_11_OUT PBout(14)

#define nSLEEP_10_OUT PBout(15)
#define ENABLE_10_OUT PDout(8)
#define DIR_10_OUT PDout(9)

#define nSLEEP_9_OUT PGout(2)
#define ENABLE_9_OUT PGout(3)
#define DIR_9_OUT PGout(4)

#define nSLEEP_8_OUT PGout(5)
#define ENABLE_8_OUT PGout(6)
#define DIR_8_OUT PGout(7)



#define nSLEEP_7_OUT PEout(12)
#define ENABLE_7_OUT PEout(13)
#define DIR_7_OUT PEout(14)

#define nSLEEP_6_OUT PEout(15)
#define ENABLE_6_OUT PBout(10)
#define DIR_6_OUT PBout(11)

#define nSLEEP_5_OUT PBout(13)
#define ENABLE_5_OUT PBout(14)
#define DIR_5_OUT PBout(15)

#define nSLEEP_4_OUT PDout(8)
#define ENABLE_4_OUT PDout(9)
#define DIR_4_OUT PDout(10)

#define nSLEEP_3_OUT PDout(11)
#define ENABLE_3_OUT PDout(12)//****************TIM4-CH1
#define DIR_3_OUT PGout(2)

#define nSLEEP_2_OUT PGout(3)
#define ENABLE_2_OUT PGout(4)
#define DIR_2_OUT PGout(5)

#define nSLEEP_1_OUT PGout(6)
#define ENABLE_1_OUT PGout(7)
#define DIR_1_OUT PGout(8)

















void DRV8434_MOTOR_Config_Init(void);//总初始化接口

void DRV8434_GPIO_Init(void);//初始化

void DRV8434_PWM3_Init(u16 arr,u16 psc);

void DRV8434_PWM4_Init(u16 arr,u16 psc);


void DRV8434_Motor_Run_Control_Timing(uint8_t deviceId,motorDir_t direction);

void DRV8434_Motor_Stop_Control_Timing(uint8_t deviceId);

int __DRV8434_Motor_Speed_To_PWM_Config__(double speed);

int __DRV8434_Motor_Caculate_Next_Acc_Speed__(uint8_t deviceId);

int __DRV8434_Motor_Caculate_Next_Dec_Speed__(uint8_t deviceId);

double __DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(double max, double start, double acc);

void __DRV8434_Motor_Get_Speed_BK_Config__(uint8_t deviceId);


void DRV8434_Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount);

void DRV8434_Motor_Move(uint8_t deviceId, motorDir_t direction);

void DRV8434_Motor_HardStop_And_Goto_Sleep(uint8_t deviceId);

void DRV8434_Motor_Select_Speed(uint8_t deviceId, MOTOR_SPEED_type_t speed_type);


typedef struct{
	double start;//启动速度
	double acc;//加速度
	double dec;//减速度
	double max;//最高速度
	double accSpeed;
}Drv_Motor_Speed_Run_Config_t;


typedef struct{
	Drv_Motor_Speed_Run_Config_t slow; //电机慢速
	Drv_Motor_Speed_Run_Config_t normal;//电机中速
	Drv_Motor_Speed_Run_Config_t high;//电机高速
	Drv_Motor_Speed_Run_Config_t use;
	Drv_Motor_Speed_Run_Config_t bk;
	
	//int currentSpeed;//当前电机速度
}Drv_Motor_Speed_Config_t;


typedef struct{
	
	Drv_Motor_Speed_Config_t M1;
	Drv_Motor_Speed_Config_t M2;
	Drv_Motor_Speed_Config_t M3;
	Drv_Motor_Speed_Config_t M4;
	
	Drv_Motor_Speed_Config_t M5;
	Drv_Motor_Speed_Config_t M6;
	Drv_Motor_Speed_Config_t M7;
	
	Drv_Motor_Speed_Config_t M8;
	Drv_Motor_Speed_Config_t M9;
	Drv_Motor_Speed_Config_t M10;
	Drv_Motor_Speed_Config_t M11;
}Drv_Motor_Speed_Config_Group_t;

#endif


