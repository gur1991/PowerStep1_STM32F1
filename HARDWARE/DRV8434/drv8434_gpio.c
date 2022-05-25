#include "drv8434_gpio.h"
#include "drv8434.h"
#include "light.h"

void GP_DRV8434_MOTOR_GPIO_Config_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;

	LOGD("start \r\n");
#ifdef USE_DRV8434_PECKER

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();


  /*Configure GPIO pins : PB14 PB15 PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /*Configure GPIO pins : PB14 PB15 PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD9 PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


  /*Configure GPIO pins : PG7 PG6 PG5 PG4 
                           PG3 PG2 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_3|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_3|GPIO_PIN_2, GPIO_PIN_RESET);
#endif



#ifdef USE_DRV8434_CAMEL

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
	
  /*Configure GPIO pins : PB14 PB15 PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /*Configure GPIO pins : PB14 PB15 PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD9 PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  /*Configure GPIO pins : PG7 PG6 PG5 PG4 
                           PG3 PG2 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_3|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_3|GPIO_PIN_2, GPIO_PIN_RESET);

#endif
	GP_Init_Drv8434_Speed_Config();
	LOGD("end \r\n");
}

GP_Drv_Motor_Speed_Config_Group_t sDRV8434Config;


void GP_Init_Drv8434_Speed_Config(void)
{
	memset(&sDRV8434Config,0,sizeof(sDRV8434Config));
	
	sDRV8434Config.M1.s=100;
	sDRV8434Config.M1.e=20;
	
	sDRV8434Config.M2.s=100;
	sDRV8434Config.M2.e=5;
	
	sDRV8434Config.M3.s=100;
  sDRV8434Config.M3.e=20;
	
	sDRV8434Config.M4.s=100;
  sDRV8434Config.M4.e=40;
	
	sDRV8434Config.M5.s=200;
  sDRV8434Config.M5.e=100;
	
	sDRV8434Config.M6.s=100;
  sDRV8434Config.M6.e=30;
	
	sDRV8434Config.M7.s=400;
  sDRV8434Config.M7.e=300;
	
	sDRV8434Config.M8.s=100;
  sDRV8434Config.M8.e=20;
	
	sDRV8434Config.M9.s=100;
  sDRV8434Config.M9.e=20;
	
	sDRV8434Config.M10.s=100;
  sDRV8434Config.M10.e=50;
	
	sDRV8434Config.M11.s=100;
  sDRV8434Config.M11.e=40;
	
}	

int GP_DRV8434_From_DelayMS_Get_Steps_Count(int deviceId, int delayMs)
{
	int count =0;
		switch(deviceId)
	{
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
			count=(delayMs*1000)/(2*sDRV8434Config.M1.e);
			break;
		case M2_MIX:
			count=(delayMs*1000)/(2*sDRV8434Config.M2.e);
			break;
		case M3_LEFT_WAIT:
			count=(delayMs*1000)/(2*sDRV8434Config.M3.e);
			break;
		case M4_BLANK_NEXT:
			count=(delayMs*1000)/(2*sDRV8434Config.M4.e);
			break;
		case M5_WAIT_NEXT:
			count=(delayMs*1000)/(2*sDRV8434Config.M5.e);
			break;
		case M6_BLANK_LEFT:
			count=(delayMs*1000)/(2*sDRV8434Config.M6.e);
			break;
		case M7_HIGH_TURN:
			count=(delayMs*1000)/(2*sDRV8434Config.M7.e);
			break;
#endif		
		
		
#ifdef USE_DRV8434_PECKER		
		case M8_BIG_IN_OUT:
			count=(delayMs*1000)/(2*sDRV8434Config.M8.e);
			break;
		case M9_IN_OUT:
			count=(delayMs*1000)/(2*sDRV8434Config.M9.e);
			break;
		case M10_UP_DOWM:
			count=(delayMs*1000)/(2*sDRV8434Config.M10.e);
			break;
		case M11_FAR_NEAR:	
			count=(delayMs*1000)/(2*sDRV8434Config.M11.e);
			break;			
#endif

		default:
			;
	}	
	return count;
}








void GP_DRV8434_Motor_Move_Steps_Dual(uint8_t deviceId_1, motorDir_t direction_1,
																			uint8_t deviceId_2, motorDir_t direction_2, uint32_t stepCount)
{
	uint32_t i=0,c6=0,c5=0;
	//LOGD("start \r\n");
	GP_DRV8434_Motor_Run_Control_Timing(deviceId_1, direction_1);
	GP_DRV8434_Motor_Run_Control_Timing(deviceId_2, direction_2);
	
	STEP_5_OUT=1;
	STEP_6_OUT=1;
	for(i=0; i<stepCount*sDRV8434Config.M5.e*2; i++)
	{
		
			delay_us(1);
			if(0 == i%sDRV8434Config.M6.e)
			{
				STEP_6_OUT=~STEP_6_OUT;
				c6++;
				if(c6>=stepCount)break;
			}
			
			if(0 == i%sDRV8434Config.M5.e)
			{
				STEP_5_OUT=~STEP_5_OUT;
				c5++;
				if(c5>=stepCount)break;
			}
	}
	
	
	GP_DRV8434_Motor_Stop_Control_Timing(deviceId_1);
	GP_DRV8434_Motor_Stop_Control_Timing(deviceId_2);
	//LOGD("end \r\n");


}


void GP_DRV8434_Motor_Move_Steps_Single_Soft_Stop(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
	uint32_t DelayUs=100;
	DelayUs=sDRV8434Config.M2.s;
	int i=0;
	GP_DRV8434_Motor_Run_Control_Timing(deviceId, direction);
	for(i=0; i<stepCount; i++)
	{
		
		STEP_2_OUT=1;
		delay_us(DelayUs);
		STEP_2_OUT=0;
		delay_us(DelayUs);
		
		//if(DelayUs<2*sDRV8434Config.M2.s)DelayUs++;

	}
	GP_DRV8434_Motor_Stop_Control_Timing(deviceId);

}









void GP_DRV8434_Motor_Move_Steps_Single_Check_Light(uint8_t deviceId, motorDir_t direction, uint32_t stepCount,u8 light)
{
	uint32_t DelayUs=100;
	uint32_t i=0;
	//LOGD("start \r\n");
	GP_DRV8434_Motor_Run_Control_Timing(deviceId, direction);
	switch(deviceId)
	{	
		case M1_MIX_V:
			
		DelayUs=sDRV8434Config.M1.s;
		for(i=0; i<stepCount; i++)
		{
			if(0==__Light_Sensor_Get__(light))break;
			
			STEP_1_OUT=1;
			delay_us(DelayUs);
			STEP_1_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M1.e)DelayUs--;
		}	
			break;		
		case M2_MIX:
		DelayUs=sDRV8434Config.M2.s;
		
		for(i=0; i<stepCount; i++)
		{
			if(0==__Light_Sensor_Get__(light))break;
			
			STEP_2_OUT=1;
			delay_us(DelayUs);
			STEP_2_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M2.e)DelayUs--;
		}
			break;		
		case M3_LEFT_WAIT:
			DelayUs=sDRV8434Config.M3.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_3_OUT=1;
			delay_us(DelayUs);
			STEP_3_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M3.e)DelayUs--;

		}	
			break;	
		case M4_BLANK_NEXT:
			DelayUs=sDRV8434Config.M4.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_4_OUT=1;
			delay_us(DelayUs);
			STEP_4_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M4.e)DelayUs--;
		}	
			break;	
		case M5_WAIT_NEXT:
			DelayUs=sDRV8434Config.M5.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_5_OUT=1;
			delay_us(DelayUs);
			STEP_5_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M5.e)DelayUs--;
		}	
			break;
		case M6_BLANK_LEFT:
			DelayUs=sDRV8434Config.M6.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_6_OUT=1;
			delay_us(DelayUs);
			STEP_6_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M6.e)DelayUs--;
		}	
			break;
		case M7_HIGH_TURN:
			DelayUs=sDRV8434Config.M7.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_7_OUT=1;
			delay_us(DelayUs);
			STEP_7_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M7.e)DelayUs--;
		}	
			break;					
		case M8_BIG_IN_OUT:
			DelayUs=sDRV8434Config.M8.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_8_OUT=1;
			delay_us(DelayUs);
			STEP_8_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M8.e)DelayUs--;
		}	
			break;
		case M9_IN_OUT:
			DelayUs=sDRV8434Config.M9.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_9_OUT=1;
			delay_us(DelayUs);
			STEP_9_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M9.e)DelayUs--;
		}	
			break;
		case M10_UP_DOWM:
			DelayUs=sDRV8434Config.M10.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_10_OUT=1;
			delay_us(DelayUs);
			STEP_10_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M10.e)DelayUs--;
		}	
			break;
		case M11_FAR_NEAR:
			DelayUs=sDRV8434Config.M11.s;
		for(i=0; i<stepCount; i++)
		{	
			if(0==__Light_Sensor_Get__(light))break;
			STEP_11_OUT=1;
			delay_us(DelayUs);
			STEP_11_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M11.e)DelayUs--;
		}	
			break;
		
		default:
			;
	}
	GP_DRV8434_Motor_Stop_Control_Timing(deviceId);
	//LOGD("end \r\n");
}	















void GP_DRV8434_Motor_Move_Steps_Single(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
	uint32_t DelayUs=100;
	uint32_t i=0;
	//LOGD("start \r\n");
	GP_DRV8434_Motor_Run_Control_Timing(deviceId, direction);
	switch(deviceId)
	{	
		case M1_MIX_V:
			
		DelayUs=sDRV8434Config.M1.s;
		for(i=0; i<stepCount; i++)
		{
			STEP_1_OUT=1;
			delay_us(DelayUs);
			STEP_1_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M1.e)DelayUs--;
		}	
			break;		
		case M2_MIX:
		DelayUs=sDRV8434Config.M2.s;
		
		for(i=0; i<stepCount; i++)
		{
			
			STEP_2_OUT=1;
			delay_us(DelayUs);
			STEP_2_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M2.e)DelayUs--;
		}
			break;		
		case M3_LEFT_WAIT:
			DelayUs=sDRV8434Config.M3.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_3_OUT=1;
			delay_us(DelayUs);
			STEP_3_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M3.e)DelayUs--;

		}	
			break;	
		case M4_BLANK_NEXT:
			DelayUs=sDRV8434Config.M4.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_4_OUT=1;
			delay_us(DelayUs);
			STEP_4_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M4.e)DelayUs--;
		}	
			break;	
		case M5_WAIT_NEXT:
			DelayUs=sDRV8434Config.M5.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_5_OUT=1;
			delay_us(DelayUs);
			STEP_5_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M5.e)DelayUs--;
		}	
			break;
		case M6_BLANK_LEFT:
			DelayUs=sDRV8434Config.M6.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_6_OUT=1;
			delay_us(DelayUs);
			STEP_6_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M6.e)DelayUs--;
		}	
			break;
		case M7_HIGH_TURN:
			DelayUs=sDRV8434Config.M7.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_7_OUT=1;
			delay_us(DelayUs);
			STEP_7_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M7.e)DelayUs--;
		}	
			break;					
		case M8_BIG_IN_OUT:
			DelayUs=sDRV8434Config.M8.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_8_OUT=1;
			delay_us(DelayUs);
			STEP_8_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M8.e)DelayUs--;
		}	
			break;
		case M9_IN_OUT:
			DelayUs=sDRV8434Config.M9.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_9_OUT=1;
			delay_us(DelayUs);
			STEP_9_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M9.e)DelayUs--;
		}	
			break;
		case M10_UP_DOWM:
			DelayUs=sDRV8434Config.M10.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_10_OUT=1;
			delay_us(DelayUs);
			STEP_10_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M10.e)DelayUs--;
		}	
			break;
		case M11_FAR_NEAR:
			DelayUs=sDRV8434Config.M11.s;
		for(i=0; i<stepCount; i++)
		{	
			
			STEP_11_OUT=1;
			delay_us(DelayUs);
			STEP_11_OUT=0;
			delay_us(DelayUs);
			if(DelayUs>sDRV8434Config.M11.e)DelayUs--;
		}	
			break;
		
		default:
			;
	}
	GP_DRV8434_Motor_Stop_Control_Timing(deviceId);
	//LOGD("end \r\n");
}	


void GP_DRV8434_Motor_Run_Control_Timing(uint8_t deviceId,motorDir_t direction)
{
	
	switch(deviceId)
	{
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
			nSLEEP_1_OUT=1;
			ENABLE_1_OUT=1;
			DIR_1_OUT=direction;
			break;
		case M2_MIX:
			nSLEEP_2_OUT=1;
			ENABLE_2_OUT=1;
			DIR_2_OUT=direction;
			break;
		case M3_LEFT_WAIT:
		  nSLEEP_3_OUT=1;
			ENABLE_3_OUT=1;
			DIR_3_OUT=direction;
			break;
		case M4_BLANK_NEXT:
			nSLEEP_4_OUT=1;
			ENABLE_4_OUT=1;
			DIR_4_OUT=direction;
			break;
		case M5_WAIT_NEXT:
			nSLEEP_5_OUT=1;
			ENABLE_5_OUT=1;
			DIR_5_OUT=direction;
			break;
		case M6_BLANK_LEFT:
			nSLEEP_6_OUT=1;
			ENABLE_6_OUT=1;
			DIR_6_OUT=direction;
			break;
		case M7_HIGH_TURN:
			nSLEEP_7_OUT=1;
			ENABLE_7_OUT=1;
			DIR_7_OUT=direction;
			break;
#endif		
		
		
#ifdef USE_DRV8434_PECKER		
		case M8_BIG_IN_OUT:
			nSLEEP_8_OUT=1;
			ENABLE_8_OUT=1;
			DIR_8_OUT=direction;
			break;
		case M9_IN_OUT:
			nSLEEP_9_OUT=1;
			ENABLE_9_OUT=1;
			DIR_9_OUT=direction;
		
			break;
		case M10_UP_DOWM:
			nSLEEP_10_OUT=1;
			ENABLE_10_OUT=1;
			DIR_10_OUT=direction;
		
			break;
		case M11_FAR_NEAR:	
			nSLEEP_11_OUT=1;
			ENABLE_11_OUT=1;
			DIR_11_OUT=direction;
		
			break;			
#endif



		
		default:
			;
	}	
	delay_us(1);
}	


void GP_DRV8434_Motor_Stop_Control_Timing(uint8_t deviceId)
{
		delay_us(1);//延时起timing
	switch(deviceId)
	{
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
			nSLEEP_1_OUT=0;
			ENABLE_1_OUT=1;
			break;
		
		case M2_MIX:
			nSLEEP_2_OUT=0;
			ENABLE_2_OUT=1;
			break;
		
		case M3_LEFT_WAIT:
			nSLEEP_3_OUT=0;
			ENABLE_3_OUT=1;
			break;
		case M4_BLANK_NEXT:
			nSLEEP_4_OUT=0;
			ENABLE_4_OUT=1;
			break;
		case M5_WAIT_NEXT:
			nSLEEP_5_OUT=0;
			ENABLE_5_OUT=1;
			break;
		case M6_BLANK_LEFT:
			nSLEEP_6_OUT=0;
			ENABLE_6_OUT=1;
			break;
		
		case M7_HIGH_TURN:
			nSLEEP_7_OUT=0;
			ENABLE_7_OUT=1;
			break;
#endif				
		
		
		
#ifdef USE_DRV8434_PECKER				
		case M8_BIG_IN_OUT:
			nSLEEP_8_OUT=0;
			ENABLE_8_OUT=1;
		
			break;
		case M9_IN_OUT:
			nSLEEP_9_OUT=0;
			ENABLE_9_OUT=1;
		
			break;
		case M10_UP_DOWM://****************************电路修改降低空闲电流获得发热少
			nSLEEP_10_OUT=0;
			ENABLE_10_OUT=1;
		
			break;
		case M11_FAR_NEAR:	
			nSLEEP_11_OUT=0;
			ENABLE_11_OUT=1;
		
			break;			
#endif

		default:
			;
	}	
}





