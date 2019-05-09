#ifndef _LIGHT_H
#define _LIGHT_H
#include "sys.h"

//����ķ�ʽ��ͨ��λ��������ʽ��ȡIO
//#define KEY0        PEin(4) 	//KEY0����PE4
//#define KEY1        PEin(3) 	//KEY1����PE3
//#define WK_UP       PAin(0) 	//WKUP����PA0


//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define LIGHT_VALUE0        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)  //PE5

void Light_Sensor_Init(void);
u8 Light_Sensor_Get(u8 number);

//�ο����� ��������STM32F101xCDE_DS_CH_V5.pdf��5V��ѹ��


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

#endif