#include "electromagnetic.h"
#include "config.h"
/*
*stm32f1xx_hal_gpio_ex.h 文件里面有关于管教复用很多资源。如果遇到资源不好使或者冲突，应该考虑到这个情况。
*/

void Electromagnetic_init(void)
{
#if USE_GRADIENT_CONTROL_BOARD	
	GPIO_InitTypeDef GPIO_InitStruct;

	
	__HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_AFIO_REMAP_SWJ_NOJTAG();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

	
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
#endif

#if USE_CLEANING_DILUTION_BOARD	
		GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
#endif	
	
}

/*example*/
//FET0=1;
//FET0=0;
void electromagnetic_control(uint8_t devices, uint8_t status)
{
		switch(devices)
		{
#if USE_GRADIENT_CONTROL_BOARD				
			//風扇
			case 1:	FET1=status;break;
			case 2:	FET2=status;break;
		  case 3:	FET3=status;break;
		  case 4:	FET4=status;break;
		  case 5:	FET5=status;break;
		  case 6:	FET6=status;break;
			//A B C
			case 7:	  VALUE_A=status;break;
			case 8:	  VALUE_B=status;break;
			case 9:	  VALUE_C=status;break;
			case 10:	VALUE_0=status;break;
#endif
			
#if	USE_CLEANING_DILUTION_BOARD				
			case 11:	VALUE_D=status;break;
			case 12:	VALUE_E=status;break;
			case 13:	VALUE_F=status;break;
			case 14:	VALUE_G=status;break;
			case 15:	VALUE_1=status;break;
			case 16:	VALUE_2=status;break;
#endif
			
			default:break;
		}
}

void electromagnetic_control_package(electromagnetic_package_type_t data)
{
#if (USE_GRADIENT_CONTROL_BOARD||USE_CLEANING_DILUTION_BOARD)		
		electromagnetic_control(data.request.device_1, data.request.status_1);
		electromagnetic_control(data.request.device_2, data.request.status_2);
	  electromagnetic_control(data.request.device_3, data.request.status_3);	
#endif
}	


