#include "drv8434.h"


TIM_HandleTypeDef 	DRV_TIM3_Handler;      	//定时器句柄 
TIM_HandleTypeDef 	DRV_TIM4_Handler;      	//定时器句柄 

TIM_OC_InitTypeDef 	DRV_CHHandler;

const int divClock=2;
//max 50k pwm

static Drv_Motor_Speed_Config_Group_t sDrvMotorSpeed;


void DRV8434_MOTOR_Config_Init(void)
{
	//PWM频率=72000000/900=80Khz
	DRV8434_GPIO_Init();
	
	//return;
	
	DRV8434_PWM3_Init(900-1,divClock-1);//>>1-1 为40K
	DRV8434_PWM4_Init(900-1,divClock-1);//10-10为8K  
	
#if (defined USE_DRV8434_CAMEL)||(defined USE_DRV8434_PECKER) 
	__HAL_TIM_ENABLE_IT(&DRV_TIM3_Handler, TIM_IT_UPDATE);
	HAL_NVIC_SetPriority(TIM3_IRQn,3,1);    //设置中断优先级，抢占优先级1，子优先级3
	HAL_NVIC_DisableIRQ(TIM3_IRQn);  

	__HAL_TIM_ENABLE_IT(&DRV_TIM4_Handler, TIM_IT_UPDATE);
	HAL_NVIC_SetPriority(TIM4_IRQn,3,2);    //设置中断优先级，抢占优先级1，子优先级3
	HAL_NVIC_DisableIRQ(TIM4_IRQn);  
#endif
	
	
	int i=0;
	for(i=1;i<12;i++)
	{
		DRV8434_Motor_Select_Speed(i, NORMAL_SPEED);
	}	
}	



//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz

void DRV8434_PWM3_Init(u16 arr,u16 psc)
{
    DRV_TIM3_Handler.Instance=TIM3;          	//定时器3
    DRV_TIM3_Handler.Init.Prescaler=psc;       //定时器分频
    DRV_TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    DRV_TIM3_Handler.Init.Period=arr;          //自动重装载值
    DRV_TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&DRV_TIM3_Handler);       //初始化PWM
    
	  DRV_CHHandler.OCMode=TIM_OCMODE_PWM2; //模式选择PWM1
    DRV_CHHandler.Pulse=arr>>1;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    DRV_CHHandler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	 	
#if (defined USE_DRV8434_CAMEL)||(defined USE_DRV8434_PECKER) 
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM3_Handler,&DRV_CHHandler,TIM_CHANNEL_3);//配置TIM3通道4	
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM3_Handler,&DRV_CHHandler,TIM_CHANNEL_4);
	
		HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_3);//开启通道
		HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_4);
#endif
	
#ifdef USE_DRV8434_CAMEL	
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM3_Handler,&DRV_CHHandler,TIM_CHANNEL_1);//配置TIM3通道4	
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM3_Handler,&DRV_CHHandler,TIM_CHANNEL_2);
	
		HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_1);//开启通道
		HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_2);
#endif


}

void DRV8434_PWM4_Init(u16 arr,u16 psc)
{
    DRV_TIM4_Handler.Instance=TIM4;          	//定时器3
    DRV_TIM4_Handler.Init.Prescaler=psc;       //定时器分频
    DRV_TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    DRV_TIM4_Handler.Init.Period=arr;          //自动重装载值
    DRV_TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&DRV_TIM4_Handler);       //初始化PWM
    
	  DRV_CHHandler.OCMode=TIM_OCMODE_PWM2; //模式选择PWM1
    DRV_CHHandler.Pulse=arr>>1;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    DRV_CHHandler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	 	
#ifdef USE_DRV8434_PECKER
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM4_Handler,&DRV_CHHandler,TIM_CHANNEL_1);//配置TIM3通道4
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM4_Handler,&DRV_CHHandler,TIM_CHANNEL_2);//配置TIM3通道4

		HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_1);
	  HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_2);
#endif

#ifdef USE_DRV8434_CAMEL
	
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM4_Handler,&DRV_CHHandler,TIM_CHANNEL_2);//配置TIM3通道4	
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM4_Handler,&DRV_CHHandler,TIM_CHANNEL_3);//配置TIM3通道4
		HAL_TIM_PWM_ConfigChannel(&DRV_TIM4_Handler,&DRV_CHHandler,TIM_CHANNEL_4);//配置TIM3通道4

		HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_3);
	  HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_4);
		
#endif
	
}	





void DRV8434_GPIO_Init(void)
{
#ifdef USE_DRV8434_PECKER
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pins : PG7 PG6 PG5 PG4 
                           PG3 PG2 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_3|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PD9 PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB15 PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_3|GPIO_PIN_2, GPIO_PIN_RESET);
													
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9|GPIO_PIN_8, GPIO_PIN_RESET);
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);
	
	
	
	
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器	
		__HAL_AFIO_REMAP_TIM3_ENABLE();   //TIM3通道引脚重映射使能
		__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_9|GPIO_PIN_8;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOC,&GPIO_InitStruct); 	
		

		__HAL_RCC_TIM4_CLK_ENABLE();
		__HAL_AFIO_REMAP_TIM4_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_12|GPIO_PIN_13;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOD,&GPIO_InitStruct); 	
	
#endif




#ifdef USE_DRV8434_CAMEL
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();


  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


  /*Configure GPIO pins : PG7 PG6 PG5 PG4 
                           PG3 PG2 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_3|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PD9 PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB15 PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_13|GPIO_PIN_11|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4 
                          |GPIO_PIN_3|GPIO_PIN_2, GPIO_PIN_RESET);
													
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8, GPIO_PIN_RESET);
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_13|GPIO_PIN_11|GPIO_PIN_10, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_13|GPIO_PIN_12, GPIO_PIN_RESET);
	
		
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		
		
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器	
	__HAL_AFIO_REMAP_TIM3_ENABLE();   //TIM3通道引脚重映射使能
	__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
	
	GPIO_InitStruct.Pin=GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6;  //PC9
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
	GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH ;     //高速
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct); 	
	
	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_AFIO_REMAP_TIM4_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOC时钟
	
	GPIO_InitStruct.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;  //PC9
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
	GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;     //高速
	HAL_GPIO_Init(GPIOD,&GPIO_InitStruct); 

#endif
}	


/*
#if (USE_AUTOMATIC_INJECTION_BOARD||USE_GRADIENT_CONTROL_BOARD)


#if (defined USE_DRV8434_PECKER)||(defined USE_DRV8434_CAMEL) 		
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
   if(htim->Instance==TIM3)
	{
		__HAL_TIM_ENABLE_IT(&DRV_TIM3_Handler, TIM_IT_UPDATE);
		HAL_NVIC_SetPriority(TIM3_IRQn,3,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);  
	}else if(htim->Instance==TIM4)
	{
		__HAL_TIM_ENABLE_IT(&DRV_TIM4_Handler, TIM_IT_UPDATE);
		HAL_NVIC_SetPriority(TIM4_IRQn,3,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);  
	}
}
#endif


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
#if (defined USE_DRV8434_CAMEL) 	
	
  if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器	
		__HAL_AFIO_REMAP_TIM3_ENABLE();   //TIM3通道引脚重映射使能
		__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOC,&GPIO_InitStruct); 	
		
	}else if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();
		__HAL_AFIO_REMAP_TIM4_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOD,&GPIO_InitStruct); 	
	
	}
#endif

#if (defined USE_DRV8434_PECKER) 	
	
  if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器	
		__HAL_AFIO_REMAP_TIM3_ENABLE();   //TIM3通道引脚重映射使能
		__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_9|GPIO_PIN_8;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOC,&GPIO_InitStruct); 	
		
	}else if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();
		__HAL_AFIO_REMAP_TIM4_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_12|GPIO_PIN_13;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOD,&GPIO_InitStruct); 	
	
	}
#endif	
	
	
	
	
}
#endif
*/




/********************************************************************/

static uint32_t sTIM_PWM_COUNT_INDEX_TIM3=0, sTIM_PWM_COUNT_MAX_TIM3=0xFFFFFFFE;
static uint8_t sMOTOR_DEVICE_TIM3=0;
static int sMOTOR_CONFIG_TIM3=0;
static bool DRV_MOTOR_WAIT_OK_TIM3=false;

static uint32_t sTIM_PWM_COUNT_INDEX_TIM4=0, sTIM_PWM_COUNT_MAX_TIM4=0xFFFFFFFE;
static uint8_t sMOTOR_DEVICE_TIM4=0;
static int sMOTOR_CONFIG_TIM4=0;
static bool DRV_MOTOR_WAIT_OK_TIM4=false;







//中断统计脉冲数量，清除中断标识
void TIM3_IRQHandler(void)
{ 	
	    
    if(__HAL_TIM_GET_IT_SOURCE(&DRV_TIM3_Handler,TIM_IT_UPDATE)==SET)//溢出中断
    {
				__HAL_TIM_CLEAR_IT(&DRV_TIM3_Handler, TIM_IT_UPDATE);//清除中断标志位		
				sTIM_PWM_COUNT_INDEX_TIM3++;
				if(sTIM_PWM_COUNT_INDEX_TIM3>=sTIM_PWM_COUNT_MAX_TIM3)
				{	
					switch(sMOTOR_DEVICE_TIM3)
					{
#ifdef USE_DRV8434_PECKER			
							case M8_BIG_IN_OUT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_4);break;
							case M9_IN_OUT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_3);break;		
#endif

#ifdef USE_DRV8434_CAMEL			
							case M1_MIX_V:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_4);break;
							case M5_WAIT_NEXT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_3);break;
							case M3_LEFT_WAIT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_2);break;
							case M4_BLANK_NEXT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_1);break;		
#endif
							default:
					}
					sTIM_PWM_COUNT_INDEX_TIM3=0;
					sTIM_PWM_COUNT_MAX_TIM3=0xFFFFFFFE;
					DRV_MOTOR_WAIT_OK_TIM3=true;
				}		
				else
				{
						sMOTOR_CONFIG_TIM3= __DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM3,sTIM_PWM_COUNT_INDEX_TIM3);
		
						if(0!=sMOTOR_CONFIG_TIM3)
						{	
							switch(sMOTOR_DEVICE_TIM3)
							{
							#ifdef USE_DRV8434_PECKER		
							case M8_BIG_IN_OUT:
							if(0!=sMOTOR_CONFIG_TIM3){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
								TIM3->CCR4=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1; 
							}
							break;

							case M9_IN_OUT:
							if(0!=sMOTOR_CONFIG_TIM3){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
								TIM3->CCR3=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1; 
							}
							break;
							#endif

							#ifdef USE_DRV8434_CAMEL		
							case M1_MIX_V:
							if(0!=sMOTOR_CONFIG_TIM3){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
								TIM3->CCR4=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1; 
							}
							break;
							case M5_WAIT_NEXT:
							if(0!=sMOTOR_CONFIG_TIM3){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
								TIM3->CCR3=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1; 
							}
							break;
							case M3_LEFT_WAIT:
							if(0!=sMOTOR_CONFIG_TIM3){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
								TIM3->CCR2=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1; 
							}
							break;
							case M4_BLANK_NEXT:
							if(0!=sMOTOR_CONFIG_TIM3){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
								TIM3->CCR1=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1; 
							}
							break;
							#endif
							default:
							}
					}
				}				
		}
}		


void TIM4_IRQHandler(void)
{ 		    		

    if(__HAL_TIM_GET_IT_SOURCE(&DRV_TIM4_Handler,TIM_IT_UPDATE)==SET)//溢出中断
    {				

				__HAL_TIM_CLEAR_IT(&DRV_TIM4_Handler, TIM_IT_UPDATE);//清除中断标志位
				//脉冲计数到设定值，PWM通道关闭, 脉冲计数恢复默认
				sTIM_PWM_COUNT_INDEX_TIM4++;
				
				if(sTIM_PWM_COUNT_INDEX_TIM4 >= sTIM_PWM_COUNT_MAX_TIM4)
				{
						switch(sMOTOR_DEVICE_TIM4){
				#ifdef USE_DRV8434_PECKER			
								case M10_UP_DOWM:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_2);break;
								case M11_FAR_NEAR:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_1);break;			
				#endif
				#ifdef USE_DRV8434_CAMEL			
								case M2_MIX:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_4);break;
								case M6_BLANK_LEFT:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_3);break;
								case M7_HIGH_TURN:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_2);break;
				#endif
								default:
						}
					
					sTIM_PWM_COUNT_INDEX_TIM4=0;
					sTIM_PWM_COUNT_MAX_TIM4=0xFFFFFFFE;
					DRV_MOTOR_WAIT_OK_TIM4=true;
				}else
				{
						sMOTOR_CONFIG_TIM4= __DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM4,sTIM_PWM_COUNT_INDEX_TIM4);
						if(0!=sMOTOR_CONFIG_TIM4)
						{	
								switch(sMOTOR_DEVICE_TIM4)
								{
								#ifdef USE_DRV8434_PECKER		
								case M10_UP_DOWM:
								if(0!=sMOTOR_CONFIG_TIM4){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
									TIM4->CCR2=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1; 
								}
								break;

								case M11_FAR_NEAR:
								if(0!=sMOTOR_CONFIG_TIM4){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
									TIM4->CCR1=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1; 
								}
								break;
								#endif

								#ifdef USE_DRV8434_CAMEL		
								case M2_MIX:
								if(0!=sMOTOR_CONFIG_TIM4){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
									TIM4->CCR4=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1; 
								}
								break;
								case M6_BLANK_LEFT:
								if(0!=sMOTOR_CONFIG_TIM4){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
									TIM4->CCR3=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1; 
								}
								break;
								case M7_HIGH_TURN:
								if(0!=sMOTOR_CONFIG_TIM4){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
									TIM4->CCR2=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1; 
								} 
								break;
								#endif
								default:
								}
					}		
				}
		}
 
}


void __DRV8434_Motor_Move_Steps_Enable_Disable_Acc__(uint8_t deviceId,int enable)
{
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
//step-1 初始化脉冲计数
		switch(deviceId){
				case M10_UP_DOWM:
				case M11_FAR_NEAR:		
				case M2_MIX:
				case M6_BLANK_LEFT:
				case M7_HIGH_TURN:
				if(enable)HAL_NVIC_EnableIRQ(TIM4_IRQn);
				else HAL_NVIC_DisableIRQ(TIM4_IRQn);
				break;
				
				case M1_MIX_V:
				case M5_WAIT_NEXT:		
				case M3_LEFT_WAIT:
				case M4_BLANK_NEXT:
				case M8_BIG_IN_OUT:
				case M9_IN_OUT:
				
				if(enable)HAL_NVIC_EnableIRQ(TIM3_IRQn);
				else HAL_NVIC_DisableIRQ(TIM3_IRQn);
				break;
				default:
		}
#endif
}	


void DRV8434_Motor_Move_Steps_Enable_Acc(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
		__DRV8434_Motor_Move_Steps_Enable_Disable_Acc__(deviceId, 1);
	  __DRV8434_Motor_Move_Steps__( deviceId, direction, stepCount);
#endif
}	
void DRV8434_Motor_Move_Steps_Disable_Acc(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
		__DRV8434_Motor_Move_Steps_Enable_Disable_Acc__(deviceId, 0);
	  __DRV8434_Motor_Move_Steps__( deviceId, direction, stepCount);
#endif
}



void __DRV8434_Motor_Move_Steps__(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
	
//step-1 初始化脉冲计数
		switch(deviceId){
				case M10_UP_DOWM:
				case M11_FAR_NEAR:		
				case M2_MIX:
				case M6_BLANK_LEFT:
				case M7_HIGH_TURN:

				sTIM_PWM_COUNT_INDEX_TIM4=0;
				sTIM_PWM_COUNT_MAX_TIM4=stepCount;
				DRV_MOTOR_WAIT_OK_TIM4=false;
				sMOTOR_DEVICE_TIM4=deviceId;
				//HAL_NVIC_DisableIRQ(TIM4_IRQn);
				break;
				
				case M1_MIX_V:
				case M5_WAIT_NEXT:		
				case M3_LEFT_WAIT:
				case M4_BLANK_NEXT:
				case M8_BIG_IN_OUT:
				case M9_IN_OUT:

				sTIM_PWM_COUNT_INDEX_TIM3=0;
				sTIM_PWM_COUNT_MAX_TIM3=stepCount;
				DRV_MOTOR_WAIT_OK_TIM3=false;
				sMOTOR_DEVICE_TIM3=deviceId;
				//HAL_NVIC_DisableIRQ(TIM3_IRQn);
				break;
				
				
				default:
		}
	
	
	//step-2 运动
	DRV8434_Motor_Move( deviceId,  direction);
	

#endif



}



void DRV8434_Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
	
	DRV8434_Motor_Move_Steps_Enable_Acc(deviceId,  direction,  stepCount);
	while(1)
	{
		
			if((M10_UP_DOWM==deviceId ||
				 M11_FAR_NEAR==deviceId ||
				 M2_MIX==deviceId ||
				 M6_BLANK_LEFT==deviceId || 
				 M7_HIGH_TURN==deviceId)&&DRV_MOTOR_WAIT_OK_TIM4)break;


			if((M1_MIX_V==deviceId ||
			M5_WAIT_NEXT==deviceId ||
			M3_LEFT_WAIT==deviceId ||
			M4_BLANK_NEXT==deviceId || 
			M8_BIG_IN_OUT==deviceId ||
			M9_IN_OUT==deviceId)&&DRV_MOTOR_WAIT_OK_TIM3)break;


			delay_ms(1);
	}
	//LOGD("***3 \r\n");
	//step-3:  DRV goto sleep
	DRV8434_Motor_HardStop_And_Goto_Sleep(deviceId);
#endif	
}





void DRV8434_Motor_Move(uint8_t deviceId, motorDir_t direction )
{	
			switch(deviceId){
				case M10_UP_DOWM:
				case M11_FAR_NEAR:		
				case M2_MIX:
				case M6_BLANK_LEFT:
				case M7_HIGH_TURN:

				sMOTOR_DEVICE_TIM4=deviceId;
				break;
				
				case M1_MIX_V:
				case M5_WAIT_NEXT:		
				case M3_LEFT_WAIT:
				case M4_BLANK_NEXT:
				case M8_BIG_IN_OUT:
				case M9_IN_OUT:

				sMOTOR_DEVICE_TIM3=deviceId;
				break;
	
				default:
		}
	
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)		

	//step-1: timing-ready
	DRV8434_Motor_Run_Control_Timing(deviceId, direction);
	
	//step-2: init config
	__DRV8434_Motor_Get_Speed_BK_Config__(deviceId);
	
  //step-3:  PWM runing 
	switch(deviceId)
	{
#ifdef USE_DRV8434_PECKER		
		case M8_BIG_IN_OUT:

			sMOTOR_CONFIG_TIM3=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM3,0);
			if(0!=sMOTOR_CONFIG_TIM3){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
			}
			TIM3->CCR4= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_4);
			break;
		
		case M9_IN_OUT:
			sMOTOR_CONFIG_TIM3=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM3,0);
			if(0!=sMOTOR_CONFIG_TIM3){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
			}
			TIM3->CCR3=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_3);
			break;
		
		case M10_UP_DOWM:
			sMOTOR_CONFIG_TIM4=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM4,0);
			if(0!=sMOTOR_CONFIG_TIM4){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
			}
			TIM4->CCR2= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_2);
			break;
		
		case M11_FAR_NEAR:
			sMOTOR_CONFIG_TIM4=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM4,0);
			if(0!=sMOTOR_CONFIG_TIM4){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
			}
			TIM4->CCR1= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_1);
			break;
#endif

#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
			sMOTOR_CONFIG_TIM3=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM3,0);
			if(0!=sMOTOR_CONFIG_TIM3){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler, sMOTOR_CONFIG_TIM3);
			}
			TIM3->CCR4= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_4);
			break;
		case M5_WAIT_NEXT:
			sMOTOR_CONFIG_TIM3=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM3,0);
			if(0!=sMOTOR_CONFIG_TIM3){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
			}
			TIM3->CCR3= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_3);
			break;
		case M3_LEFT_WAIT:
			sMOTOR_CONFIG_TIM3=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM3,0);
			if(0!=sMOTOR_CONFIG_TIM3){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
			}
			TIM3->CCR2= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_2);
			break;
		case M4_BLANK_NEXT:
			sMOTOR_CONFIG_TIM3=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM3,0);
			if(0!=sMOTOR_CONFIG_TIM3){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG_TIM3);
			}
			TIM3->CCR1= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_1);
			break;
		case M2_MIX:
			sMOTOR_CONFIG_TIM4=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM4,0);
			if(0!=sMOTOR_CONFIG_TIM4){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
			}
			TIM4->CCR4= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_4);
			break;
		case M6_BLANK_LEFT:
			sMOTOR_CONFIG_TIM4=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM4,0);
			if(0!=sMOTOR_CONFIG_TIM4){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
			}
			TIM4->CCR3= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_3);
			break;
		case M7_HIGH_TURN:
			sMOTOR_CONFIG_TIM4=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE_TIM4,0);
			if(0!=sMOTOR_CONFIG_TIM4){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG_TIM4);
			}
			TIM4->CCR2= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)>>1;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_2);
			break;
#endif
		
		
			
		default:
	}
	

#endif	
}	





void DRV8434_Motor_HardStop_And_Goto_Sleep(uint8_t deviceId)
{
  //step-1:  PWM CH close
	__DRV8434_Motor_Move_Steps_Enable_Disable_Acc__(deviceId, 0);
	
	switch(deviceId)
	{
#ifdef USE_DRV8434_PECKER			
		case M8_BIG_IN_OUT:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_4);
			break;
		
		case M9_IN_OUT:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_3);
			break;
		
		case M10_UP_DOWM:
			HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_2);
			break;
		
		case M11_FAR_NEAR:
			HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_1);
			break;			
#endif

#ifdef USE_DRV8434_CAMEL			
		case M1_MIX_V:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_4);
			break;
		case M5_WAIT_NEXT:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_3);
			break;
		case M3_LEFT_WAIT:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_2);
			break;
		case M4_BLANK_NEXT:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_1);
			break;
		case M2_MIX:
			HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_4);
			break;
		case M6_BLANK_LEFT:
			HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_3);
			break;
		case M7_HIGH_TURN:
			HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_2);
			break;
		
#endif
		
		default:
	}
	
	
	//step-2:  DRV timing change
	DRV8434_Motor_Stop_Control_Timing(deviceId);
}



//速度解析函数需要优化
int __DRV8434_Motor_Speed_To_PWM_Config__(double speed)
{
	return ceil(72000000/(divClock*speed));
}	

double __DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(double max, double start, double acc)
{
	double f = (max+start)*0.5;

	double perAcc= acc/f;	

	return perAcc;
}	




void __DRV8434_Motor_Speed_To_PWM_Config_Group_Ready__(uint8_t deviceId)
{
	Drv_Motor_Speed_Config_t Msc;
	memset(&Msc , 0, sizeof(Msc));
	
	switch(deviceId)
	{
		case M1_MIX_V:Msc=sDrvMotorSpeed.M1;break;		
		case M2_MIX:Msc=sDrvMotorSpeed.M2;break;
		case M3_LEFT_WAIT:Msc=sDrvMotorSpeed.M3;break;
		case M4_BLANK_NEXT:Msc=sDrvMotorSpeed.M4;break;
		case M5_WAIT_NEXT:Msc=sDrvMotorSpeed.M5;break;
		case M6_BLANK_LEFT:Msc=sDrvMotorSpeed.M6;break;
		case M7_HIGH_TURN:Msc=sDrvMotorSpeed.M7;break;				
		case M8_BIG_IN_OUT:Msc=sDrvMotorSpeed.M8;break;
		case M9_IN_OUT:Msc=sDrvMotorSpeed.M9;break;
		case M10_UP_DOWM:Msc=sDrvMotorSpeed.M10;break;
		case M11_FAR_NEAR:Msc=sDrvMotorSpeed.M11;break;		
		default:
	}
	
	
	int speed =0;
	int i=0;
	for(i=0; i<=Msc.slow.accTimes; i++)
	{
			speed = Msc.slow.start + i*Msc.slow.interval_acc;
			Msc.slow.config[i]=ceil(72000000/(divClock*speed));
	}

	for(i=0; i<=Msc.normal.accTimes; i++)
	{
			speed = Msc.normal.start + i*Msc.normal.interval_acc;
			Msc.normal.config[i]=ceil(72000000/(divClock*speed));
		  //LOGD("i:%d %d %d\r\n",i,Msc.normal.config[i],speed);
	}
	
	for(i=0; i<=Msc.high.accTimes; i++)
	{
			speed = Msc.high.start + i*Msc.high.interval_acc;
			Msc.high.config[i]=ceil(72000000/(divClock*speed));
	}
	
	
		switch(deviceId)
	{
		case M1_MIX_V:sDrvMotorSpeed.M1=Msc;break;		
		case M2_MIX:sDrvMotorSpeed.M2=Msc;break;
		case M3_LEFT_WAIT:sDrvMotorSpeed.M3=Msc;break;
		case M4_BLANK_NEXT:sDrvMotorSpeed.M4=Msc;break;
		case M5_WAIT_NEXT:sDrvMotorSpeed.M5=Msc;break;
		case M6_BLANK_LEFT:sDrvMotorSpeed.M6=Msc;break;
		case M7_HIGH_TURN:sDrvMotorSpeed.M7=Msc;break;				
		case M8_BIG_IN_OUT:sDrvMotorSpeed.M8=Msc;break;
		case M9_IN_OUT:sDrvMotorSpeed.M9=Msc;break;
		case M10_UP_DOWM:sDrvMotorSpeed.M10=Msc;break;
		case M11_FAR_NEAR:sDrvMotorSpeed.M11=Msc;break;		
		default:
	}
	
}	


//梯度加速，整个加速过程分为十次加上去，而不是每次都要计算一下
//accTimes为几次加上去
int __DRV8434_GET_ACC_PER_TIMES_ADD__(Drv_Motor_Speed_Run_Config_t Ms)
{
	int max=Ms.max;
	int start=Ms.start; 
	int acc=Ms.accSpeed;  
	int accTimes=Ms.accTimes;
	
	double f = (max+start)*0.5;//计算平均速率
	double perAcc= acc/f; //计算每个波形分摊的增速
	
	double interval_f=f/accTimes;//计算每次调速走了多少
	int interval_Acc=interval_f*perAcc;//计算每次累加ACC的值

	return interval_Acc;
}	

int __DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(Drv_Motor_Speed_Run_Config_t Ms)
{
	int max=Ms.max;
	int start=Ms.start; 
	int acc=Ms.accSpeed;  
	int accTimes=Ms.accTimes;
	
	double f = (max+start)*0.5;//计算平均速率
	double perAcc= acc/f; //计算每个波形分摊的增速
	
	int interval_f=f/accTimes;//计算每次调速走了多少

	return interval_f;
}	



void __DRV8434_Motor_Get_Speed_BK_Config__(uint8_t deviceId)
{
		
	switch(deviceId)
	{	
		case M1_MIX_V:memset(&sDrvMotorSpeed.M1.use, 0,sizeof(sDrvMotorSpeed.M1.use));sDrvMotorSpeed.M1.use=sDrvMotorSpeed.M1.bk;break;		
		case M2_MIX:memset(&sDrvMotorSpeed.M2.use, 0,sizeof(sDrvMotorSpeed.M2.use));sDrvMotorSpeed.M2.use=sDrvMotorSpeed.M2.bk;break;
		case M3_LEFT_WAIT:memset(&sDrvMotorSpeed.M3.use, 0,sizeof(sDrvMotorSpeed.M3.use));sDrvMotorSpeed.M3.use=sDrvMotorSpeed.M3.bk;break;
		case M4_BLANK_NEXT:memset(&sDrvMotorSpeed.M4.use, 0,sizeof(sDrvMotorSpeed.M4.use));sDrvMotorSpeed.M4.use=sDrvMotorSpeed.M4.bk;break;
		case M5_WAIT_NEXT:memset(&sDrvMotorSpeed.M5.use, 0,sizeof(sDrvMotorSpeed.M5.use));sDrvMotorSpeed.M5.use=sDrvMotorSpeed.M5.bk;break;
		case M6_BLANK_LEFT:memset(&sDrvMotorSpeed.M6.use, 0,sizeof(sDrvMotorSpeed.M6.use));sDrvMotorSpeed.M6.use=sDrvMotorSpeed.M6.bk;break;
		case M7_HIGH_TURN:memset(&sDrvMotorSpeed.M7.use, 0,sizeof(sDrvMotorSpeed.M7.use));sDrvMotorSpeed.M7.use=sDrvMotorSpeed.M7.bk;break;				
		case M8_BIG_IN_OUT:memset(&sDrvMotorSpeed.M8.use, 0,sizeof(sDrvMotorSpeed.M8.use));sDrvMotorSpeed.M8.use=sDrvMotorSpeed.M8.bk;break;
		case M9_IN_OUT:memset(&sDrvMotorSpeed.M9.use, 0,sizeof(sDrvMotorSpeed.M9.use));sDrvMotorSpeed.M9.use=sDrvMotorSpeed.M9.bk;break;
		case M10_UP_DOWM:memset(&sDrvMotorSpeed.M10.use, 0,sizeof(sDrvMotorSpeed.M10.use));sDrvMotorSpeed.M10.use=sDrvMotorSpeed.M10.bk;break;
		case M11_FAR_NEAR:memset(&sDrvMotorSpeed.M11.use, 0,sizeof(sDrvMotorSpeed.M11.use));sDrvMotorSpeed.M11.use=sDrvMotorSpeed.M11.bk;break;	
		default:
	}

}

void DRV8434_Motor_Select_Speed(uint8_t deviceId, MOTOR_SPEED_type_t speed_type)
{
	__DRV8434_Motor_Select_Speed__( deviceId,  speed_type);
	__DRV8434_Motor_Select_Speed__( deviceId,  speed_type);
}	
void __DRV8434_Motor_Select_Speed__(uint8_t deviceId, MOTOR_SPEED_type_t speed_type)
{
	int config=0;
	
	
	
	switch(deviceId)
	{
		
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
			sDrvMotorSpeed.M1.slow.start=1000;
			sDrvMotorSpeed.M1.slow.max=2000;	
			sDrvMotorSpeed.M1.slow.accSpeed=1000;
			sDrvMotorSpeed.M1.slow.dec=40;
			sDrvMotorSpeed.M1.slow.accTimes=10;
			sDrvMotorSpeed.M1.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M1.slow);
			sDrvMotorSpeed.M1.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M1.slow);

		
			sDrvMotorSpeed.M1.normal.start=10000;
			sDrvMotorSpeed.M1.normal.max=20000;	
			sDrvMotorSpeed.M1.normal.accSpeed=10000;
			sDrvMotorSpeed.M1.normal.dec=40;
			sDrvMotorSpeed.M1.normal.accTimes=10;
			sDrvMotorSpeed.M1.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M1.normal);
			sDrvMotorSpeed.M1.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M1.normal);
		
			sDrvMotorSpeed.M1.high.start=10000;
			sDrvMotorSpeed.M1.high.max=20000;	
			sDrvMotorSpeed.M1.high.accSpeed=10000;
			sDrvMotorSpeed.M1.high.dec=40;
			sDrvMotorSpeed.M1.high.accTimes=10;
			sDrvMotorSpeed.M1.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M1.high);
			sDrvMotorSpeed.M1.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M1.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M1.bk=sDrvMotorSpeed.M1.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M1.bk=sDrvMotorSpeed.M1.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M1.bk=sDrvMotorSpeed.M1.high;}
			break;
			
		case M2_MIX:
			sDrvMotorSpeed.M2.slow.start=10000;
			sDrvMotorSpeed.M2.slow.max=20000;	
			sDrvMotorSpeed.M2.slow.accSpeed=10000;
			sDrvMotorSpeed.M2.slow.dec=40;
			sDrvMotorSpeed.M2.slow.accTimes=10;
			sDrvMotorSpeed.M2.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M2.slow);
			sDrvMotorSpeed.M2.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M2.slow);
		
			sDrvMotorSpeed.M2.normal.start=10000;
			sDrvMotorSpeed.M2.normal.max=20000;	
			sDrvMotorSpeed.M2.normal.accSpeed=10000;
			sDrvMotorSpeed.M2.normal.dec=40;
			sDrvMotorSpeed.M2.normal.accTimes=10;
			sDrvMotorSpeed.M2.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M2.normal);
			sDrvMotorSpeed.M2.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M2.normal);
		
			sDrvMotorSpeed.M2.high.start=10000;
			sDrvMotorSpeed.M2.high.max=20000;	
			sDrvMotorSpeed.M2.high.accSpeed=10000;
			sDrvMotorSpeed.M2.high.dec=40;
			sDrvMotorSpeed.M2.high.accTimes=10;
			sDrvMotorSpeed.M2.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M2.high);
			sDrvMotorSpeed.M2.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M2.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M2.bk=sDrvMotorSpeed.M2.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M2.bk=sDrvMotorSpeed.M2.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M2.bk=sDrvMotorSpeed.M2.high;}
			break;
		case M3_LEFT_WAIT:
			sDrvMotorSpeed.M3.slow.start=10000;
			sDrvMotorSpeed.M3.slow.max=20000;	
			sDrvMotorSpeed.M3.slow.accSpeed=10000;
			sDrvMotorSpeed.M3.slow.dec=40;
			sDrvMotorSpeed.M3.slow.accTimes=10;
			sDrvMotorSpeed.M3.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M3.slow);
			sDrvMotorSpeed.M3.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M3.slow);
		
			sDrvMotorSpeed.M3.normal.start=20000;
			sDrvMotorSpeed.M3.normal.max=40000;	
			sDrvMotorSpeed.M3.normal.accSpeed=20000;
			sDrvMotorSpeed.M3.normal.dec=40;
			sDrvMotorSpeed.M3.normal.accTimes=10;
			sDrvMotorSpeed.M3.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M3.normal);
			sDrvMotorSpeed.M3.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M3.normal);
		
			sDrvMotorSpeed.M3.high.start=20000;
			sDrvMotorSpeed.M3.high.max=40000;	
			sDrvMotorSpeed.M3.high.accSpeed=20000;
			sDrvMotorSpeed.M3.high.dec=40;
			sDrvMotorSpeed.M3.high.accTimes=10;
			sDrvMotorSpeed.M3.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M3.high);
			sDrvMotorSpeed.M3.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M3.high);
		
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M3.bk=sDrvMotorSpeed.M3.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M3.bk=sDrvMotorSpeed.M3.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M3.bk=sDrvMotorSpeed.M3.high;}
			break;
		case M4_BLANK_NEXT:
			sDrvMotorSpeed.M4.slow.start=1000;
			sDrvMotorSpeed.M4.slow.max=2000;	
			sDrvMotorSpeed.M4.slow.accSpeed=1000;
			sDrvMotorSpeed.M4.slow.dec=40;
			sDrvMotorSpeed.M4.slow.accTimes=10;
			sDrvMotorSpeed.M4.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M4.slow);
			sDrvMotorSpeed.M4.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M4.slow);
		
			sDrvMotorSpeed.M4.normal.start=10000;
			sDrvMotorSpeed.M4.normal.max=20000;	
			sDrvMotorSpeed.M4.normal.accSpeed=10000;
			sDrvMotorSpeed.M4.normal.dec=40;
			sDrvMotorSpeed.M4.normal.accTimes=10;
			sDrvMotorSpeed.M4.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M4.normal);
			sDrvMotorSpeed.M4.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M4.normal);
		
			sDrvMotorSpeed.M4.high.start=10000;
			sDrvMotorSpeed.M4.high.max=20000;	
			sDrvMotorSpeed.M4.high.accSpeed=10000;
			sDrvMotorSpeed.M4.high.dec=40;
			sDrvMotorSpeed.M4.high.accTimes=10;
			sDrvMotorSpeed.M4.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M4.high);
			sDrvMotorSpeed.M4.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M4.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M4.bk=sDrvMotorSpeed.M4.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M4.bk=sDrvMotorSpeed.M4.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M4.bk=sDrvMotorSpeed.M4.high;}
			break;
		case M5_WAIT_NEXT:
			sDrvMotorSpeed.M5.slow.start=1000;
			sDrvMotorSpeed.M5.slow.max=2000;	
			sDrvMotorSpeed.M5.slow.accSpeed=1000;
			sDrvMotorSpeed.M5.slow.dec=40;
			sDrvMotorSpeed.M5.slow.accTimes=10;
			sDrvMotorSpeed.M5.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M5.slow);
			sDrvMotorSpeed.M5.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M5.slow);
		
			sDrvMotorSpeed.M5.normal.start=10000;
			sDrvMotorSpeed.M5.normal.max=20000;	
			sDrvMotorSpeed.M5.normal.accSpeed=10000;
			sDrvMotorSpeed.M5.normal.dec=40;
			sDrvMotorSpeed.M5.normal.accTimes=10;
			sDrvMotorSpeed.M5.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M5.normal);
			sDrvMotorSpeed.M5.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M5.normal);
		
			sDrvMotorSpeed.M5.high.start=10000;
			sDrvMotorSpeed.M5.high.max=20000;	
			sDrvMotorSpeed.M5.high.accSpeed=5000;
			sDrvMotorSpeed.M5.high.dec=40;
			sDrvMotorSpeed.M5.high.accTimes=10;
			sDrvMotorSpeed.M5.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M5.high);
			sDrvMotorSpeed.M5.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M5.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M5.bk=sDrvMotorSpeed.M5.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M5.bk=sDrvMotorSpeed.M5.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M5.bk=sDrvMotorSpeed.M5.high;}
			break;
		case M6_BLANK_LEFT:
			sDrvMotorSpeed.M6.slow.start=1000;
			sDrvMotorSpeed.M6.slow.max=2000;	
			sDrvMotorSpeed.M6.slow.accSpeed=1000;
			sDrvMotorSpeed.M6.slow.dec=40;
			sDrvMotorSpeed.M6.slow.accTimes=10;
			sDrvMotorSpeed.M6.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M6.slow);
			sDrvMotorSpeed.M6.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M6.slow);
		
			sDrvMotorSpeed.M6.normal.start=10000;
			sDrvMotorSpeed.M6.normal.max=20000;	
			sDrvMotorSpeed.M6.normal.accSpeed=10000;
			sDrvMotorSpeed.M6.normal.dec=40;
			sDrvMotorSpeed.M6.normal.accTimes=10;
			sDrvMotorSpeed.M6.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M6.normal);
			sDrvMotorSpeed.M6.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M6.normal);
		
			sDrvMotorSpeed.M6.high.start=10000;
			sDrvMotorSpeed.M6.high.max=20000;	
			sDrvMotorSpeed.M6.high.accSpeed=10000;
			sDrvMotorSpeed.M6.high.dec=40;
			sDrvMotorSpeed.M6.high.accTimes=10;
			sDrvMotorSpeed.M6.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M6.high);
			sDrvMotorSpeed.M6.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M6.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M6.bk=sDrvMotorSpeed.M6.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M6.bk=sDrvMotorSpeed.M6.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M6.bk=sDrvMotorSpeed.M6.high;}
			break;
		case M7_HIGH_TURN:
			sDrvMotorSpeed.M7.slow.start=1000;
			sDrvMotorSpeed.M7.slow.max=2000;	
			sDrvMotorSpeed.M7.slow.accSpeed=1000;
			sDrvMotorSpeed.M7.slow.dec=40;
			sDrvMotorSpeed.M7.slow.accTimes=10;
			sDrvMotorSpeed.M7.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M7.slow);
			sDrvMotorSpeed.M7.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M7.slow);
		
			sDrvMotorSpeed.M7.normal.start=2000;
			sDrvMotorSpeed.M7.normal.max=4000;	
			sDrvMotorSpeed.M7.normal.accSpeed=4000;
			sDrvMotorSpeed.M7.normal.dec=40;
			sDrvMotorSpeed.M7.normal.accTimes=10;
			sDrvMotorSpeed.M7.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M7.normal);
			sDrvMotorSpeed.M7.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M7.normal);
		
			sDrvMotorSpeed.M7.high.start=10000;
			sDrvMotorSpeed.M7.high.max=20000;	
			sDrvMotorSpeed.M7.high.accSpeed=10000;
			sDrvMotorSpeed.M7.high.dec=40;
			sDrvMotorSpeed.M7.high.accTimes=10;
			sDrvMotorSpeed.M7.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M7.high);
			sDrvMotorSpeed.M7.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M7.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M7.bk=sDrvMotorSpeed.M7.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M7.bk=sDrvMotorSpeed.M7.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M7.bk=sDrvMotorSpeed.M7.high;}
			break;
		

#endif




		
#ifdef USE_DRV8434_PECKER					
		case M8_BIG_IN_OUT:
			sDrvMotorSpeed.M8.slow.start=10000;
			sDrvMotorSpeed.M8.slow.max=20000;	
			sDrvMotorSpeed.M8.slow.accSpeed=10000;
			sDrvMotorSpeed.M8.slow.dec=40;
			sDrvMotorSpeed.M8.slow.accTimes=10;
			sDrvMotorSpeed.M8.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M8.slow);
			sDrvMotorSpeed.M8.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M8.slow);
		
			sDrvMotorSpeed.M8.normal.start=10000;
			sDrvMotorSpeed.M8.normal.max=20000;	
			sDrvMotorSpeed.M8.normal.accSpeed=10000;
			sDrvMotorSpeed.M8.normal.dec=40;
			sDrvMotorSpeed.M8.normal.accTimes=10;
			sDrvMotorSpeed.M8.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M8.normal);
			sDrvMotorSpeed.M8.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M8.normal);
		
			sDrvMotorSpeed.M8.high.start=10000;
			sDrvMotorSpeed.M8.high.max=20000;	
			sDrvMotorSpeed.M8.high.accSpeed=10000;
			sDrvMotorSpeed.M8.high.dec=40;
			sDrvMotorSpeed.M8.high.accTimes=10;
			sDrvMotorSpeed.M8.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M8.high);
			sDrvMotorSpeed.M8.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M8.high);

			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M8.bk=sDrvMotorSpeed.M8.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M8.bk=sDrvMotorSpeed.M8.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M8.bk=sDrvMotorSpeed.M8.high;}
			break;
		
		case M9_IN_OUT:
			sDrvMotorSpeed.M9.slow.start=10000;
			sDrvMotorSpeed.M9.slow.max=20000;	
			sDrvMotorSpeed.M9.slow.accSpeed=10000;
			sDrvMotorSpeed.M9.slow.dec=40;
			sDrvMotorSpeed.M9.slow.accTimes=10;
			sDrvMotorSpeed.M9.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M9.slow);
			sDrvMotorSpeed.M9.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M9.slow);
		
			sDrvMotorSpeed.M9.normal.start=10000;
			sDrvMotorSpeed.M9.normal.max=30000;	
			sDrvMotorSpeed.M9.normal.accSpeed=10000;
			sDrvMotorSpeed.M9.normal.dec=40;
			sDrvMotorSpeed.M9.normal.accTimes=10;
			sDrvMotorSpeed.M9.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M9.normal);
			sDrvMotorSpeed.M9.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M9.normal);
		
			sDrvMotorSpeed.M9.high.start=10000;
			sDrvMotorSpeed.M9.high.max=30000;	
			sDrvMotorSpeed.M9.high.accSpeed=10000;
			sDrvMotorSpeed.M9.high.dec=40;
			sDrvMotorSpeed.M9.high.accTimes=10;
			sDrvMotorSpeed.M9.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M9.high);
			sDrvMotorSpeed.M9.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M9.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M9.bk=sDrvMotorSpeed.M9.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M9.bk=sDrvMotorSpeed.M9.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M9.bk=sDrvMotorSpeed.M9.high;}
			break;
		
		case M10_UP_DOWM:
			sDrvMotorSpeed.M10.slow.start=10000;
			sDrvMotorSpeed.M10.slow.max=20000;	
			sDrvMotorSpeed.M10.slow.accSpeed=10000;
			sDrvMotorSpeed.M10.slow.dec=40;
			sDrvMotorSpeed.M10.slow.accTimes=10;
			sDrvMotorSpeed.M10.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M10.slow);
			sDrvMotorSpeed.M10.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M10.slow);
		
			sDrvMotorSpeed.M10.normal.start=10000;
			sDrvMotorSpeed.M10.normal.max=30000;	
			sDrvMotorSpeed.M10.normal.accSpeed=10000;
			sDrvMotorSpeed.M10.normal.dec=40;
			sDrvMotorSpeed.M10.normal.accTimes=10;
			sDrvMotorSpeed.M10.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M10.normal);
			sDrvMotorSpeed.M10.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M10.normal);
		
			sDrvMotorSpeed.M10.high.start=10000;
			sDrvMotorSpeed.M10.high.max=40000;	
			sDrvMotorSpeed.M10.high.accSpeed=10000;
			sDrvMotorSpeed.M10.high.dec=40;
			sDrvMotorSpeed.M10.high.accTimes=10;
			sDrvMotorSpeed.M10.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M10.high);
			sDrvMotorSpeed.M10.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M10.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M10.bk=sDrvMotorSpeed.M10.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M10.bk=sDrvMotorSpeed.M10.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M10.bk=sDrvMotorSpeed.M10.high;}
			break;
		
		case M11_FAR_NEAR:
			sDrvMotorSpeed.M11.slow.start=5000;
			sDrvMotorSpeed.M11.slow.max=15000;	
			sDrvMotorSpeed.M11.slow.accSpeed=10000;
			sDrvMotorSpeed.M11.slow.dec=40;
			sDrvMotorSpeed.M11.slow.accTimes=10;
			sDrvMotorSpeed.M11.slow.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M11.slow);
			sDrvMotorSpeed.M11.slow.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M11.slow);
		
			sDrvMotorSpeed.M11.normal.start=5000;
			sDrvMotorSpeed.M11.normal.max=15000;	
			sDrvMotorSpeed.M11.normal.accSpeed=10000;
			sDrvMotorSpeed.M11.normal.dec=40;
			sDrvMotorSpeed.M11.normal.accTimes=10;
			sDrvMotorSpeed.M11.normal.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M11.normal);
			sDrvMotorSpeed.M11.normal.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M11.normal);
		
			sDrvMotorSpeed.M11.high.start=5000;
			sDrvMotorSpeed.M11.high.max=15000;	
			sDrvMotorSpeed.M11.high.accSpeed=10000;
			sDrvMotorSpeed.M11.high.dec=40;
			sDrvMotorSpeed.M11.high.accTimes=10;
			sDrvMotorSpeed.M11.high.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M11.high);
			sDrvMotorSpeed.M11.high.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M11.high);
			
			if(LOW_SPEED==speed_type){sDrvMotorSpeed.M11.bk=sDrvMotorSpeed.M11.slow;}
			else if(NORMAL_SPEED==speed_type){sDrvMotorSpeed.M11.bk=sDrvMotorSpeed.M11.normal;}
			else if(HIGH_SPEED==speed_type){sDrvMotorSpeed.M11.bk=sDrvMotorSpeed.M11.high;}
			break;
			
#endif			
		default:
	}
		
	__DRV8434_Motor_Speed_To_PWM_Config_Group_Ready__(deviceId);
}


void DRV8434_Motor_Run_Control_Timing(uint8_t deviceId,motorDir_t direction)
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
	}	
	delay_us(50);//延时起timing
}	

void DRV8434_Motor_Stop_Control_Timing(uint8_t deviceId)
{
	  //nothing to do
	//	delay_ms(1);
	//  return ;
	
		delay_us(50);//延时起timing
		//return;	
	switch(deviceId)
	{
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
			nSLEEP_1_OUT=1;
			ENABLE_1_OUT=0;
			break;
		
		case M2_MIX:
			nSLEEP_2_OUT=1;
			ENABLE_2_OUT=0;
			break;
		
		case M3_LEFT_WAIT:
			nSLEEP_3_OUT=1;
			ENABLE_3_OUT=0;
			break;
		case M4_BLANK_NEXT:
			nSLEEP_4_OUT=1;
			ENABLE_4_OUT=0;
			break;
		case M5_WAIT_NEXT:
			nSLEEP_5_OUT=1;
			ENABLE_5_OUT=0;
			break;
		case M6_BLANK_LEFT:
			nSLEEP_6_OUT=1;
			ENABLE_6_OUT=0;
			break;
		
		case M7_HIGH_TURN:
			nSLEEP_7_OUT=1;
			ENABLE_7_OUT=0;
			break;
#endif				
		
		
		
#ifdef USE_DRV8434_PECKER				
		case M8_BIG_IN_OUT:
			nSLEEP_8_OUT=1;
			ENABLE_8_OUT=0;
		
			break;
		case M9_IN_OUT:
			nSLEEP_9_OUT=1;
			ENABLE_9_OUT=0;
		
			break;
		case M10_UP_DOWM://****************************电路修改降低空闲电流获得发热少
			nSLEEP_10_OUT=0;
			ENABLE_10_OUT=1;
		
			break;
		case M11_FAR_NEAR:	
			nSLEEP_11_OUT=1;
			ENABLE_11_OUT=0;
		
			break;			
#endif

		default:
	}	
	//delay_ms(1);//延时起timing

}	



int __DRV8434_Motor_Caculate_Next_Acc_Speed__(uint8_t deviceId,int currentIndex)
{
	int config=0;
	int index=0;	
	
	Drv_Motor_Speed_Config_t Ms;
	memset(&Ms, 0, sizeof(Ms));
	switch(deviceId)
	{
		case M1_MIX_V:Ms=sDrvMotorSpeed.M1;break;
		case M2_MIX:Ms=sDrvMotorSpeed.M2;break;
		case M3_LEFT_WAIT:Ms=sDrvMotorSpeed.M3;break;
		case M4_BLANK_NEXT:Ms=sDrvMotorSpeed.M4;break;
		case M5_WAIT_NEXT:Ms=sDrvMotorSpeed.M5;break;
		case M6_BLANK_LEFT:Ms=sDrvMotorSpeed.M6;break;
		case M7_HIGH_TURN:Ms=sDrvMotorSpeed.M7;break;
		case M8_BIG_IN_OUT:Ms=sDrvMotorSpeed.M8;break;
		case M9_IN_OUT:Ms=sDrvMotorSpeed.M9;break;
		case M10_UP_DOWM:Ms=sDrvMotorSpeed.M10;break;
		case M11_FAR_NEAR:Ms=sDrvMotorSpeed.M11;break;	
		default:
	}
	
	//LOGD("start:%f max:%f \r\n",Ms.use.start, Ms.use.max);
	if( (Ms.use.start < Ms.use.max) && (0 == currentIndex%Ms.use.interval_pwm))
	{
			//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
			if(Ms.use.start + Ms.use.interval_acc > Ms.use.max)
			{
					Ms.use.start=Ms.use.max;
			}else
			{
					Ms.use.start+=Ms.use.interval_acc;
			}
			
			index = currentIndex/Ms.use.interval_pwm;
			config = Ms.use.config[index];		
	}
	
			switch(deviceId)
			{	
				case M1_MIX_V:sDrvMotorSpeed.M1=Ms;break;
				case M2_MIX:sDrvMotorSpeed.M2=Ms;break;
				case M3_LEFT_WAIT:sDrvMotorSpeed.M3=Ms;break;
				case M4_BLANK_NEXT:sDrvMotorSpeed.M4=Ms;break;
				case M5_WAIT_NEXT:sDrvMotorSpeed.M5=Ms;break;
				case M6_BLANK_LEFT:sDrvMotorSpeed.M6=Ms;break;
				case M7_HIGH_TURN:sDrvMotorSpeed.M7=Ms;break;
				case M8_BIG_IN_OUT:sDrvMotorSpeed.M8=Ms;break;
				case M9_IN_OUT:sDrvMotorSpeed.M9=Ms;break;
				case M10_UP_DOWM:sDrvMotorSpeed.M10=Ms;break;
				case M11_FAR_NEAR:sDrvMotorSpeed.M11=Ms;break;	
				default:
			}
	return config;
}



int __DRV8434_Motor_Caculate_Next_Dec_Speed__(uint8_t deviceId)
{
	int next=0;

	
	
	return next;
}









