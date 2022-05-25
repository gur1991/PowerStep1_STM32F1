#ifndef _STEP01_INTERFACE_H
#define _STEP01_INTERFACE_H

#include "stdint.h"	 
#include "motor.h"
#include "x_nucleo_ihmxx.h"	 
#include "stdio.h"	 
#include "sys.h"
#include "powerstep01_target_config.h"
#include "x_nucleo_ihm03a1_stm32f1xx.h"
#include "powerstep01.h" 
#include "light.h"


/*
voltate
speed      get_value :set_value =1:16
duty_cycle get_value :set_value =2.56:1

current
speed      get_value :set_value =1:16
current_value get_value :set_value =1:12 此值不固定
*/



//数组0废弃 用[1]---[14]
#define SIZE_MOTOR_ARRAY 15


extern powerstep01_Init_u_t motor_config_array[SIZE_MOTOR_ARRAY];

typedef struct
{
  float acceleration;
  float deceleration;
  float maxSpeed;
  float minSpeed;	
}motor_commonSpeed_type_t;	


typedef struct{
	float acc;
	float dec;
	float hold;
	float run;
}motor_config_type_t;

typedef struct {
		uint8_t ModeSelection;
		motor_commonSpeed_type_t Speed;
		motor_config_type_t config;	
}init_motor_type_t;

typedef union{ 
struct{
		uint8_t devices;//0,1,2
		init_motor_type_t init_motor;
}request;
struct{
		uint8_t ret;
		uint8_t nor[39];
}response;
}init_motor_speed_tension_type_t;


int init_motor_device(init_motor_speed_tension_type_t data);

typedef enum MOTOR_SPEED_type
{
		LOW_SPEED=0,
		NORMAL_SPEED,	
		HIGH_SPEED,	
	  SLEEP_SPEED
}MOTOR_SPEED_type_t;

//配置motor参数
int ConfigMotorAllDevice(int chip,MOTOR_SPEED_type_t speed_type);

void Set_Single_Motor_Config(init_motor_speed_tension_type_t data);

void Choose_Single_Motor_Speed_Config( int motor_chip, MOTOR_SPEED_type_t speed_type);


uint8_t RestSelectMotorOrgin(int motorNum,int lightNum, motorDir_t motorDir,uint32_t steps);

uint8_t RestSelectMotorOrginDelay(int motorNum,int lightNum, motorDir_t motorDir,uint32_t steps,uint16_t delayMs);

uint8_t __Normal_Pitch_Move_Next__(int motorNum,int lightNum, motorDir_t motorDir,uint32_t steps, uint16_t delayMs);

typedef union{ 
struct{
		uint8_t motorNum;
		uint8_t lightNum;
		uint8_t motorDir;
		uint32_t steps;
		uint8_t flag_wait;
}request;
struct{
		uint8_t light;
		uint8_t ret;
	  uint8_t nor[18];
}response;
}rest_select_motor_orgin_type_t;


typedef union{ 
struct{
		uint8_t motorNum;
		uint8_t lightNum;
		uint8_t motorDir;
		uint32_t steps;
		uint8_t flag_wait;
	  uint8_t speed;
}request;
struct{
		uint8_t light;
		uint8_t ret;
	  uint8_t nor[18];
}response;
}rest_select_motor_orgin_select_type_t;


//motor 编号
//light 编号
//复位时步伐
//复位时的电机方向


	
void StopALLMotorMotion(void);

void BSP_Motor_Control_Init(void);

/*********************************************************************/
#define ALLOW_MANY_MOTOR 2//最多允许6个电机同时控制
typedef struct{
			uint8_t array;//哪烐电机
			uint32_t stepCount;	//庴小
			uint8_t direction;//motorDir_t direction;//窚向
}move_motor_data_type_t;


typedef union{ 
struct{
		move_motor_data_type_t data;
}request;
struct{
		uint8_t ret;
		uint8_t nor[23];
}response;
}move_wait_motor_type_t;

typedef union{
struct{
		uint8_t speed;
		move_motor_data_type_t data;
}request;
struct{
		uint8_t ret;
		uint8_t nor[23];
}response;
}move_wait_motor_select_type_t;

typedef union{ 
struct{
		move_motor_data_type_t move_motor_data[ALLOW_MANY_MOTOR];
}request;
struct{
		uint8_t ret;
		uint8_t nor[23];
}response;
}move_many_motor_type_t;


//等待多个电机执行完毕
typedef struct{
			uint8_t array;//哪个电机
}wait_motor_data_type_t;

typedef union{ 
struct{
		wait_motor_data_type_t wait_motor_data[ALLOW_MANY_MOTOR];
}request;
struct{
		uint8_t ret;
		uint8_t nor[23];
}response;
}wait_many_motor_type_t;


typedef union{ 
	struct{
		uint8_t up_down_steps;
		uint8_t far_near_steps;
	}request;
	struct{
		uint8_t ret;
		uint8_t nor[3];
	}response;	
}mix_and_reach_type_t;


typedef struct mix_motor_config_type
{
		uint16_t max_slow;
		uint16_t acc_slow;
		uint16_t dec_slow;
		
		uint16_t max_normal;
		uint16_t acc_normal;
		uint16_t dec_normal;
}mix_motor_config_type_t;

void init_mix_motor_config(void);
void set_mix_motor_config(mix_motor_config_type_t p);

#endif
