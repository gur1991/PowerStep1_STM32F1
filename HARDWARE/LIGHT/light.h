#ifndef _LIGHT_H
#define _LIGHT_H
#include "sys.h"
#include "config.h"
#include "real_time_polling.h"

//下面的方式是通过直接操作HAL库函数方式读取IO
#define LIGHT_VALUE1        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_15)  //PF4
#define LIGHT_VALUE2        HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_0)
#define LIGHT_VALUE3        HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_1)
#define LIGHT_VALUE4        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7)
#define LIGHT_VALUE5        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_8)
#define LIGHT_VALUE6        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9)

#define LIGHT_VALUE7        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_10)
#define LIGHT_VALUE8        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11)
#define LIGHT_VALUE9        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)
#define LIGHT_VALUE10        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define LIGHT_VALUE11        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define LIGHT_VALUE12        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)

#define LIGHT_VALUE13        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_11)
#define LIGHT_VALUE14        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_12)
#define LIGHT_VALUE15        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_13)
#define LIGHT_VALUE16        HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_14)


#define LIGHT_VALUE17        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7)
#define LIGHT_VALUE18        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_8)
#define LIGHT_VALUE19        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11)
#define LIGHT_VALUE20        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12)
#define LIGHT_VALUE21        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13)
#define LIGHT_VALUE22        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14)
#define LIGHT_VALUE23        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15)
#define LIGHT_VALUE24        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)


extern u8 gStatusLight[3];

void Light_Sensor_Init(void);
u8 Light_Sensor_Get(u8 number);

void Light_Sensor_Get_All(void);

//参考资料 中文资料STM32F101xCDE_DS_CH_V5.pdf找5V耐压管


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

typedef union{ 
struct{
		uint8_t nor;//0,1,2...
}request;
struct{
		uint8_t ret;
		uint8_t value[3];
}response;
}get_all_light_sensor_level_t;


#endif
