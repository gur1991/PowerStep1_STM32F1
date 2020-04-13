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
current_value get_value :set_value =1:12 ��ֵ���̶�
*/



//����0���� ��[1]---[14]
#define SIZE_MOTOR_ARRAY 15


extern powerstep01_Init_u_t motor_config_array[SIZE_MOTOR_ARRAY];

typedef struct
{
  float acceleration;
  float deceleration;
  float maxSpeed;//-----����ٶȵ���
  float minSpeed;//���idle �ٶ�
	
}motor_commonSpeed_type_t;	

//��ѹ��ռ�ձ�
//������ma
typedef union{
struct{
	float current_value;//CURRENT_VALUE;
}current;	
struct{
	float duty_cycle;
	
}voltage;	
}motor_config_type_t;

typedef struct {
		//Current or voltage mode selection
		powerstep01_CmVm_t ModeSelection;
		motor_commonSpeed_type_t motor_commonSpeed;
		motor_config_type_t motor_config;	
}init_motor_type_t;

typedef union{ 
struct{
		uint8_t devices;//0,1,2
		init_motor_type_t init_motor;
}request;
struct{
		uint8_t ret;
		uint8_t nor[27];
}response;
}init_motor_speed_tension_type_t;

extern init_motor_speed_tension_type_t TempMotor;

int init_motor_device(init_motor_speed_tension_type_t data);

typedef enum MOTOR_SPEED_type
{
		LOW_SPEED=0,
		NORMAL_SPEED,	
		HIGH_SPEED,	
}MOTOR_SPEED_type_t;

//����motor����
int ConfigMotorAllDevice(int chip,MOTOR_SPEED_type_t speed_type);

void Set_Single_Motor_Config(init_motor_speed_tension_type_t data);

void Choose_Single_Motor_Speed_Config( int motor_chip, MOTOR_SPEED_type_t speed_type);


typedef union{ 
struct{
		uint8_t motorNum;
		uint8_t lightNum;
		uint8_t motorDir;
		uint32_t steps;
		uint8_t flag_wait;
}request;
struct{
		uint8_t ret;
	  uint8_t nor[19];
}response;
}rest_select_motor_orgin_type_t;

//motor ���
//light ���
//��λʱ����
//��λʱ�ĵ������

void RestSelectMotorOrgin(int motorNum,int lightNum, motorDir_t motorDir,uint32_t steps);
	
void StopALLMotorMotion(void);

void BSP_Motor_Control_Init(void);

/*********************************************************************/
#define ALLOW_MANY_MOTOR 2//�������6�����ͬʱ����
typedef struct{
			uint8_t array;//�ĸ����
			uint32_t stepCount;	//��С
			motorDir_t direction;//����
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
		move_motor_data_type_t move_motor_data[ALLOW_MANY_MOTOR];
}request;
struct{
		uint8_t ret;
		uint8_t nor[23];
}response;
}move_many_motor_type_t;


//�ȴ�������ִ�����
typedef struct{
			uint8_t array;//�ĸ����
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
#endif
