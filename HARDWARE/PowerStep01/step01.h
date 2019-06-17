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

typedef struct
{
  ///Acceleration 
  float acceleration;
  ///Deceleration
  float deceleration;
  ///Maximum speed
  float maxSpeed;//-----����ٶȵ���
  ///Minimum speed
  float minSpeed;//���idle �ٶ�
}motor_commonSpeed_type_t;	

//��ѹ��ռ�ձ�
//������ma
typedef union{
struct{
/*	
	float tvalHold;
  ///Torque regulation DAC reference voltage when motor is runnig at constant speed
  float tvalRun;
  ///Torque regulation DAC reference voltage during motor acceleration
  float tvalAcc;
  ///Torque regulation DAC reference voltage during motor deceleration
  float tvalDec;
*/	
	float current_value;//CURRENT_VALUE;
}current;	
struct{
/*
	float kvalHold;
  ///Voltage amplitude regulation when the motor is running at constant speed
  float kvalRun;
  ///Voltage amplitude regulation during motor acceleration
  float kvalAcc;
  ///Voltage amplitude regulation during motor deceleration
  float kvalDec;
*/
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
}response;
}init_motor_speed_tension_type_t;

int init_motor_device(init_motor_speed_tension_type_t data);


/*********************************************************************/

#define ALLOW_MANY_MOTOR 6//�������6�����ͬʱ����
typedef struct{
			uint8_t array;//�ĸ����
			uint32_t stepCount;	//��С
			motorDir_t direction;//����
}move_motor_data_type_t;

typedef union{ 
struct{
		move_motor_data_type_t move_motor_data[ALLOW_MANY_MOTOR];
}request;
struct{
		uint8_t ret;
}response;
}move_many_motor_type_t;
void move_many_motor(move_many_motor_type_t data);


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
}response;
}wait_many_motor_type_t;

void wait_many_motor(wait_many_motor_type_t data);

#endif