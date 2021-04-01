#include "export_liquid.h"

void Export_Liquid_Control_Init(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOE_CLK_ENABLE();
	
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
#endif
}	

void export_liquid_control(uint8_t status)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
		if(status!=0 && status!=1)
	EXPORT_LIQUID=status;
#endif
}	
