#include "light.h"
#include "delay.h"

//按键初始化函数
void Light_Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟

    GPIO_Initure.Pin=GPIO_PIN_5;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
       
}

//光感处理函数
//返回信号值
//或者使用宏值
u8 Light_Sensor_Get(u8 number)
{	
	u8 status=2;
	switch(number)
	{
		case 0:
				status=LIGHT_VALUE0;
				break;
		default:
				break;
	}	
    return status; 
}

u8 gStatusLight[3];
void Light_Sensor_Get_All(void)
{	
	memset(gStatusLight,0,sizeof(gStatusLight));

	gStatusLight[0]|=LIGHT_VALUE0<<0;
	/*	
	gStatusLight[0]|=LIGHT_VALUE1<<1;
	gStatusLight[0]|=LIGHT_VALUE2<<2;
	gStatusLight[0]|=LIGHT_VALUE3<<3;
	gStatusLight[0]|=LIGHT_VALUE4<<4;
	gStatusLight[0]|=LIGHT_VALUE5<<5;
	gStatusLight[0]|=LIGHT_VALUE6<<6;
	gStatusLight[0]|=LIGHT_VALUE7<<7;
	
	gStatusLight[1]|=LIGHT_VALUE8<<0;
	gStatusLight[1]|=LIGHT_VALUE9<<1;
	gStatusLight[1]|=LIGHT_VALUE10<<2;
	gStatusLight[1]|=LIGHT_VALUE11<<3;
	gStatusLight[1]|=LIGHT_VALUE12<<4;
	gStatusLight[1]|=LIGHT_VALUE13<<5;
	gStatusLight[1]|=LIGHT_VALUE14<<6;
	gStatusLight[1]|=LIGHT_VALUE15<<7;
	*/
}



