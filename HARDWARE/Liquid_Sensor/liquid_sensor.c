#include "liquid_sensor.h"

void Liquid_Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
	  __HAL_RCC_GPIOC_CLK_ENABLE();         

	  GPIO_Initure.Pin=GPIO_PIN_3;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
}	

u8 Liquid_Sensor_Get(void)
{		
	return LIQUID_VALUE;
}
