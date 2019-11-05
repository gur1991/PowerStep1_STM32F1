#ifndef __LIQUID_SENSOR_H
#define __LIQUID_SENSOR_H
#include "sys.h"

#define LIQUID_ONE_VALUE        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)  
#define LIQUID_TWO_VALUE        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)  

void Liquid_Sensor_Init(void);

u8 Liquid_Sensor_Get(void);//一次读取两个液路传感器的值
//BIT0---L1  BIT1--L2

typedef union{ 
struct{
		uint8_t nor;
}request;
struct{
		uint8_t ret;
		uint8_t value;
}response;
}get_liquid_sensor_level_t;

#endif


