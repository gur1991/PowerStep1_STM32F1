#ifndef _LIGHT_H
#define _LIGHT_H
#include "sys.h"

//下面的方式是通过位带操作方式读取IO
//#define KEY0        PEin(4) 	//KEY0按键PE4
//#define KEY1        PEin(3) 	//KEY1按键PE3
//#define WK_UP       PAin(0) 	//WKUP按键PA0


//下面的方式是通过直接操作HAL库函数方式读取IO
#define LIGHT_VALUE0        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5)  //PE5

void Light_Sensor_Init(void);
u8 Light_Sensor_Get(u8 number);

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

#endif