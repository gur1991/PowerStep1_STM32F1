#include "liquid_sensor.h"

void Liquid_Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
	  __HAL_RCC_GPIOE_CLK_ENABLE();         

	  GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_2;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}	

//BIT0 -- L1  BIT1--L2
u8 Liquid_Sensor_Get(void)
{	
	u8 value=0;	
	
	value=LIQUID_ONE_VALUE;
	value|=LIQUID_TWO_VALUE<<1;
	
	return value;
}
