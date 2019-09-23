#ifndef __LIQUID_SENSOR_H
#define __LIQUID_SENSOR_H
#include "sys.h"

#define LIQUID_VALUE        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)  

void Liquid_Sensor_Init(void);

u8 Liquid_Sensor_Get(void);

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


