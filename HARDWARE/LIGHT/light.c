#include "light.h"
#include "delay.h"
#include <string.h>
#include "config.h"
#include "drv8434.h"
#include "motor.h"
#include "factory_many.h"
#include "drv8434_gpio.h"

void Light_Sensor_Init(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	
#if 0//(defined USE_DRV8434_CAMEL)
	  Light_EXTI_Init();
#else	
    GPIO_InitTypeDef GPIO_Initure;
    
	  __HAL_RCC_GPIOG_CLK_ENABLE();           
		__HAL_RCC_GPIOE_CLK_ENABLE();           
	  __HAL_RCC_GPIOF_CLK_ENABLE();          
	  __HAL_RCC_GPIOC_CLK_ENABLE();         
	  __HAL_RCC_GPIOB_CLK_ENABLE();           
		//__HAL_RCC_AFIO_CLK_ENABLE();
	
	
		
	  GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;            
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_5;            
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
		GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;            
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);

		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;            
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
#endif
#endif


#if USE_CLEANING_DILUTION_BOARD

#if 0//(defined USE_DRV8434_PECKER) 
	 Light_EXTI_Init();
#else

   GPIO_InitTypeDef GPIO_Initure;
    
		__HAL_RCC_GPIOE_CLK_ENABLE();           
	  __HAL_RCC_GPIOB_CLK_ENABLE();           

		
	  GPIO_Initure.Pin=GPIO_PIN_10;            
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;            
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
#endif		
	
#endif


}




//////////////////////////////DELAY_STOP/////////////



void EXTI0_IRQHandler(void)
{
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
}

void EXTI1_IRQHandler(void)
{
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}








void Light_EXTI_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;

#if (defined USE_DRV8434_PECKER) 

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();

  /*Configure GPIO pins : PE7 PE8 PE11 PE12 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
  /* EXTI9_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	
	//LOGD("ok \r\n");
#endif
	
	

#if (defined USE_DRV8434_CAMEL) 

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	
	  /*Configure GPIO pins : PG0 PG1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	
	//OP8 NEXT--REPEAT
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH ;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11, 1);
	
  /*Configure GPIO pin : PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 1);


	
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, 1);




  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	
  HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
  HAL_NVIC_DisableIRQ(EXTI1_IRQn);
	
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
#endif	
	
	
}




void Light_Control_Int_Enable(int lightNum , int enableInt)
{
	return ;
	if(enableInt)
	{				
		switch(lightNum)
		{
	#if (defined USE_DRV8434_PECKER) 		
			case M8_LIGHT:HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);break;
			case M9_LIGHT:HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);break;
			case M10_LIGHT:HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);break;
			case M11_LIGHT:HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);break;
	#endif		
			
	#if (defined USE_DRV8434_CAMEL) 	
			case M1_LIGHT:HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);break;
			case M1_LIGHT_WORK:HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);break;
			
			case M4_LIGHT:HAL_NVIC_EnableIRQ(EXTI0_IRQn);break;
			case M3_LIGHT:HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);break;
			case WAIT_LIGHT:HAL_NVIC_EnableIRQ(EXTI1_IRQn);break;
			case BLANK_LIGHT:HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);break;
			case M7_LIGHT:HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);break;
			
			case NORMAL_NEXT_LIGHT:HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);break;
			case NORMAL_CHECK_DRAIN_LIGHT:HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);break;
			
	#endif			
			default:
		}
		
	}else{
		
		switch(lightNum)
		{
	#if (defined USE_DRV8434_PECKER) 		
			case M8_LIGHT:HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);break;
			case M9_LIGHT:HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);break;
			case M10_LIGHT:HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);break;
			case M11_LIGHT:HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);break;
	#endif		
			
	#if (defined USE_DRV8434_CAMEL) 	
			case M1_LIGHT:HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);break;
			case M1_LIGHT_WORK:HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);break;
			
			case M4_LIGHT:HAL_NVIC_DisableIRQ(EXTI0_IRQn);break;
			case M3_LIGHT:HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);break;
			case WAIT_LIGHT:HAL_NVIC_DisableIRQ(EXTI1_IRQn);break;
			case BLANK_LIGHT:HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);break;
			case M7_LIGHT:HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);break;
			
			case NORMAL_NEXT_LIGHT:HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);break;
			case NORMAL_CHECK_DRAIN_LIGHT:HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);break;
			
	#endif			
			default:
		}
	}
}





//光感处理函数
//返回信号值
//或者使用宏值
u8 __Light_Sensor_Get__(u8 number)
{	
	u8 status=2;
	switch(number)
	{
#if USE_AUTOMATIC_INJECTION_BOARD		
		case 1:status=LIGHT_VALUE1;break;
		case 2:status=LIGHT_VALUE2;break;
		case 3:status=LIGHT_VALUE3;break;
		case 4:status=LIGHT_VALUE4;break;
		case 5:status=LIGHT_VALUE5;break;
		case 6:status=LIGHT_VALUE6;break;
		
		case 7:status=LIGHT_VALUE7;break;
		case 8:status=LIGHT_VALUE8;break;
		case 9:status=LIGHT_VALUE9;break;
		case 10:status=LIGHT_VALUE10;break;
		case 11:status=LIGHT_VALUE11;break;
		case 12:status=LIGHT_VALUE12;break;
		
		case 13:status=LIGHT_VALUE13;break;
		case 14:status=LIGHT_VALUE14;break;
		case 15:status=LIGHT_VALUE15;break;
		case 16:status=LIGHT_VALUE16;break;
#endif	
#if USE_CLEANING_DILUTION_BOARD			
		case 17:status=LIGHT_VALUE17;break;
		case 18:status=LIGHT_VALUE18;break;
		
		case 19:status=LIGHT_VALUE19;break;
		case 20:status=LIGHT_VALUE20;break;
		case 21:status=LIGHT_VALUE21;break;
		case 22:status=LIGHT_VALUE22;break;
		case 23:status=LIGHT_VALUE23;break;
		case 24:status=LIGHT_VALUE24;break;
#endif		
		default:
				break;
	}	
    return status; 
}


u8 Light_Sensor_Get(u8 number)
{
	//return __Light_Sensor_Get__(number);
	
	u8 status;

	status=__Light_Sensor_Get__(number);
	if(!status)return status;
	delay_ms(1);
	status=__Light_Sensor_Get__(number);
	return status;
}







u8 gStatusLight[3];
void Light_Sensor_Get_All(void)
{	
	memset(gStatusLight,0,sizeof(gStatusLight));
#if USE_AUTOMATIC_INJECTION_BOARD
	gStatusLight[0]|=LIGHT_VALUE1<<0;
	gStatusLight[0]|=LIGHT_VALUE2<<1;
	gStatusLight[0]|=LIGHT_VALUE3<<2;
	gStatusLight[0]|=LIGHT_VALUE4<<3;
	gStatusLight[0]|=LIGHT_VALUE5<<4;
	gStatusLight[0]|=LIGHT_VALUE6<<5;
	gStatusLight[0]|=LIGHT_VALUE7<<6;
	gStatusLight[0]|=LIGHT_VALUE8<<7;

	gStatusLight[1]|=LIGHT_VALUE9<<0;
	gStatusLight[1]|=LIGHT_VALUE10<<1;
	gStatusLight[1]|=LIGHT_VALUE11<<2;
	gStatusLight[1]|=LIGHT_VALUE12<<3;
	gStatusLight[1]|=LIGHT_VALUE13<<4;
	gStatusLight[1]|=LIGHT_VALUE14<<5;
	gStatusLight[1]|=LIGHT_VALUE15<<6;
	gStatusLight[1]|=LIGHT_VALUE16<<7;
#endif
	
#if USE_CLEANING_DILUTION_BOARD	
	gStatusLight[2]|=LIGHT_VALUE17<<0;
	gStatusLight[2]|=LIGHT_VALUE18<<1;
	gStatusLight[2]|=LIGHT_VALUE19<<2;
	gStatusLight[2]|=LIGHT_VALUE20<<3;
	gStatusLight[2]|=LIGHT_VALUE21<<4;
	gStatusLight[2]|=LIGHT_VALUE22<<5;
	gStatusLight[2]|=LIGHT_VALUE23<<6;
	gStatusLight[2]|=LIGHT_VALUE24<<7;
#endif	
}



