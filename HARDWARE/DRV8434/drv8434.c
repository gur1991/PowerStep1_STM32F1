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







//梯度加速，整个加速过程分为十次加上去，而不是每次都要计算一下
//accTimes为几次加上去
int __DRV8434_GET_ACC_PER_TIMES_ADD__(Drv_Motor_Speed_Config_t Ms)
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

int __DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(Drv_Motor_Speed_Config_t Ms)
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
		case M1_MIX_V:sDrvMotorSpeed.M1.start=sDrvMotorSpeed.M1.start_bk;break;		
		case M2_MIX:sDrvMotorSpeed.M2.start=sDrvMotorSpeed.M2.start_bk;break;
		case M3_LEFT_WAIT:sDrvMotorSpeed.M3.start=sDrvMotorSpeed.M3.start_bk;break;
		case M4_BLANK_NEXT:sDrvMotorSpeed.M4.start=sDrvMotorSpeed.M4.start_bk;break;
		case M5_WAIT_NEXT:sDrvMotorSpeed.M5.start=sDrvMotorSpeed.M5.start_bk;break;
		case M6_BLANK_LEFT:sDrvMotorSpeed.M6.start=sDrvMotorSpeed.M6.start_bk;break;
		case M7_HIGH_TURN:sDrvMotorSpeed.M7.start=sDrvMotorSpeed.M7.start_bk;break;				
		case M8_BIG_IN_OUT:sDrvMotorSpeed.M8.start=sDrvMotorSpeed.M8.start_bk;break;
		case M9_IN_OUT:sDrvMotorSpeed.M9.start=sDrvMotorSpeed.M9.start_bk;break;
		case M10_UP_DOWM:sDrvMotorSpeed.M10.start=sDrvMotorSpeed.M10.start_bk;break;
		case M11_FAR_NEAR:sDrvMotorSpeed.M11.start=sDrvMotorSpeed.M11.start_bk;break;	
		default:
	}

}

void DRV8434_Motor_Select_Speed(uint8_t deviceId, MOTOR_SPEED_type_t speed_type)
{
	//__DRV8434_Motor_Select_Speed__( deviceId,  speed_type);
	__DRV8434_Motor_Select_Speed__( deviceId,  speed_type);
}	
void __DRV8434_Motor_Select_Speed__(uint8_t deviceId, MOTOR_SPEED_type_t speed_type)
{
	int config=0;
	int speed =0;
	int i=0;

	
	
	switch(deviceId)
	{
		
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
		
		memset(&sDrvMotorSpeed.M1, 0, sizeof(Drv_Motor_Speed_Config_t));
		
		if(LOW_SPEED==speed_type){
			sDrvMotorSpeed.M1.start_bk=sDrvMotorSpeed.M1.start=1000;
			sDrvMotorSpeed.M1.max=2000;	
			sDrvMotorSpeed.M1.accSpeed=1000;
			sDrvMotorSpeed.M1.dec=40;
			sDrvMotorSpeed.M1.accTimes=10;
			sDrvMotorSpeed.M1.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M1);
			sDrvMotorSpeed.M1.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M1);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M1.start_bk=sDrvMotorSpeed.M1.start=10000;
			sDrvMotorSpeed.M1.max=20000;	
			sDrvMotorSpeed.M1.accSpeed=10000;
			sDrvMotorSpeed.M1.dec=40;
			sDrvMotorSpeed.M1.accTimes=10;
			sDrvMotorSpeed.M1.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M1);
			sDrvMotorSpeed.M1.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M1);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M1.start_bk=sDrvMotorSpeed.M1.start=10000;
			sDrvMotorSpeed.M1.max=20000;	
			sDrvMotorSpeed.M1.accSpeed=10000;
			sDrvMotorSpeed.M1.dec=40;
			sDrvMotorSpeed.M1.accTimes=10;
			sDrvMotorSpeed.M1.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M1);
			sDrvMotorSpeed.M1.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M1);
		}	
		for(i=0; i<=sDrvMotorSpeed.M1.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M1.start + i*sDrvMotorSpeed.M1.interval_acc;
			sDrvMotorSpeed.M1.config[i]=ceil(72000000/(divClock*speed));
		}
		
		
			break;
			
		case M2_MIX:
			memset(&sDrvMotorSpeed.M2,0,  sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){
			sDrvMotorSpeed.M2.start_bk=sDrvMotorSpeed.M2.start=3000;
			sDrvMotorSpeed.M2.max=5000;	
			sDrvMotorSpeed.M2.accSpeed=10000;
			sDrvMotorSpeed.M2.dec=40;
			sDrvMotorSpeed.M2.accTimes=10;
			sDrvMotorSpeed.M2.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M2);
			sDrvMotorSpeed.M2.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M2);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M2.start_bk=sDrvMotorSpeed.M2.start=10000;
			sDrvMotorSpeed.M2.max=40000;	
			sDrvMotorSpeed.M2.accSpeed=30000;
			sDrvMotorSpeed.M2.dec=40;
			sDrvMotorSpeed.M2.accTimes=10;
			sDrvMotorSpeed.M2.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M2);
			sDrvMotorSpeed.M2.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M2);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M2.start_bk=sDrvMotorSpeed.M2.start=10000;
			sDrvMotorSpeed.M2.max=20000;	
			sDrvMotorSpeed.M2.accSpeed=10000;
			sDrvMotorSpeed.M2.dec=40;
			sDrvMotorSpeed.M2.accTimes=10;
			sDrvMotorSpeed.M2.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M2);
			sDrvMotorSpeed.M2.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M2);
		}
		for(i=0; i<=sDrvMotorSpeed.M2.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M2.start + i*sDrvMotorSpeed.M2.interval_acc;
			sDrvMotorSpeed.M2.config[i]=ceil(72000000/(divClock*speed));
		}		
		break;
		case M3_LEFT_WAIT:
			memset(&sDrvMotorSpeed.M3, 0, sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){	
			sDrvMotorSpeed.M3.start_bk=sDrvMotorSpeed.M3.start=10000;
			sDrvMotorSpeed.M3.max=20000;	
			sDrvMotorSpeed.M3.accSpeed=10000;
			sDrvMotorSpeed.M3.dec=40;
			sDrvMotorSpeed.M3.accTimes=10;
			sDrvMotorSpeed.M3.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M3);
			sDrvMotorSpeed.M3.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M3);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M3.start_bk=sDrvMotorSpeed.M3.start=20000;
			sDrvMotorSpeed.M3.max=40000;	
			sDrvMotorSpeed.M3.accSpeed=20000;
			sDrvMotorSpeed.M3.dec=40;
			sDrvMotorSpeed.M3.accTimes=10;
			sDrvMotorSpeed.M3.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M3);
			sDrvMotorSpeed.M3.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M3);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M3.start_bk=sDrvMotorSpeed.M3.start=20000;
			sDrvMotorSpeed.M3.max=40000;	
			sDrvMotorSpeed.M3.accSpeed=20000;
			sDrvMotorSpeed.M3.dec=40;
			sDrvMotorSpeed.M3.accTimes=10;
			sDrvMotorSpeed.M3.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M3);
			sDrvMotorSpeed.M3.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M3);
		}
		for(i=0; i<=sDrvMotorSpeed.M3.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M3.start + i*sDrvMotorSpeed.M3.interval_acc;
			sDrvMotorSpeed.M3.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
		case M4_BLANK_NEXT:
			memset(&sDrvMotorSpeed.M4, 0, sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){		
			sDrvMotorSpeed.M4.start_bk=sDrvMotorSpeed.M4.start=1000;
			sDrvMotorSpeed.M4.max=2000;	
			sDrvMotorSpeed.M4.accSpeed=1000;
			sDrvMotorSpeed.M4.dec=40;
			sDrvMotorSpeed.M4.accTimes=10;
			sDrvMotorSpeed.M4.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M4);
			sDrvMotorSpeed.M4.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M4);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M4.start_bk=sDrvMotorSpeed.M4.start=20000;
			sDrvMotorSpeed.M4.max=40000;	
			sDrvMotorSpeed.M4.accSpeed=20000;
			sDrvMotorSpeed.M4.dec=40;
			sDrvMotorSpeed.M4.accTimes=10;
			sDrvMotorSpeed.M4.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M4);
			sDrvMotorSpeed.M4.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M4);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M4.start_bk=sDrvMotorSpeed.M4.start=20000;
			sDrvMotorSpeed.M4.max=40000;	
			sDrvMotorSpeed.M4.accSpeed=20000;
			sDrvMotorSpeed.M4.dec=40;
			sDrvMotorSpeed.M4.accTimes=10;
			sDrvMotorSpeed.M4.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M4);
			sDrvMotorSpeed.M4.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M4);
		}	
		for(i=0; i<=sDrvMotorSpeed.M4.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M4.start + i*sDrvMotorSpeed.M4.interval_acc;
			sDrvMotorSpeed.M4.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
		case M5_WAIT_NEXT:
			memset(&sDrvMotorSpeed.M5, 0, sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){	
			sDrvMotorSpeed.M5.start_bk=sDrvMotorSpeed.M5.start=1000;
			sDrvMotorSpeed.M5.max=2000;	
			sDrvMotorSpeed.M5.accSpeed=1000;
			sDrvMotorSpeed.M5.dec=40;
			sDrvMotorSpeed.M5.accTimes=10;
			sDrvMotorSpeed.M5.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M5);
			sDrvMotorSpeed.M5.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M5);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M5.start_bk=sDrvMotorSpeed.M5.start=10000;
			sDrvMotorSpeed.M5.max=15000;	
			sDrvMotorSpeed.M5.accSpeed=5000;
			sDrvMotorSpeed.M5.dec=40;
			sDrvMotorSpeed.M5.accTimes=10;
			sDrvMotorSpeed.M5.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M5);
			sDrvMotorSpeed.M5.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M5);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M5.start_bk=sDrvMotorSpeed.M5.start=10000;
			sDrvMotorSpeed.M5.max=20000;	
			sDrvMotorSpeed.M5.accSpeed=5000;
			sDrvMotorSpeed.M5.dec=40;
			sDrvMotorSpeed.M5.accTimes=10;
			sDrvMotorSpeed.M5.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M5);
			sDrvMotorSpeed.M5.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M5);
		}	
		for(i=0; i<=sDrvMotorSpeed.M5.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M5.start + i*sDrvMotorSpeed.M5.interval_acc;
			sDrvMotorSpeed.M5.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
		case M6_BLANK_LEFT:
			memset(&sDrvMotorSpeed.M6, 0, sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){		
			sDrvMotorSpeed.M6.start_bk=sDrvMotorSpeed.M6.start=1000;
			sDrvMotorSpeed.M6.max=2000;	
			sDrvMotorSpeed.M6.accSpeed=1000;
			sDrvMotorSpeed.M6.dec=40;
			sDrvMotorSpeed.M6.accTimes=10;
			sDrvMotorSpeed.M6.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M6);
			sDrvMotorSpeed.M6.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M6);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M6.start_bk=sDrvMotorSpeed.M6.start=10000;
			sDrvMotorSpeed.M6.max=15000;	
			sDrvMotorSpeed.M6.accSpeed=5000;
			sDrvMotorSpeed.M6.dec=40;
			sDrvMotorSpeed.M6.accTimes=10;
			sDrvMotorSpeed.M6.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M6);
			sDrvMotorSpeed.M6.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M6);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M6.start_bk=sDrvMotorSpeed.M6.start=10000;
			sDrvMotorSpeed.M6.max=20000;	
			sDrvMotorSpeed.M6.accSpeed=10000;
			sDrvMotorSpeed.M6.dec=40;
			sDrvMotorSpeed.M6.accTimes=10;
			sDrvMotorSpeed.M6.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M6);
			sDrvMotorSpeed.M6.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M6);
		}	
		for(i=0; i<=sDrvMotorSpeed.M6.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M6.start + i*sDrvMotorSpeed.M6.interval_acc;
			sDrvMotorSpeed.M6.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
		case M7_HIGH_TURN:
			memset(&sDrvMotorSpeed.M7, 0, sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){	
			sDrvMotorSpeed.M7.start_bk=sDrvMotorSpeed.M7.start=1000;
			sDrvMotorSpeed.M7.max=2000;	
			sDrvMotorSpeed.M7.accSpeed=1000;
			sDrvMotorSpeed.M7.dec=40;
			sDrvMotorSpeed.M7.accTimes=10;
			sDrvMotorSpeed.M7.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M7);
			sDrvMotorSpeed.M7.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M7);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M7.start_bk=sDrvMotorSpeed.M7.start=1000;
			sDrvMotorSpeed.M7.max=1500;	
			sDrvMotorSpeed.M7.accSpeed=500;
			sDrvMotorSpeed.M7.dec=40;
			sDrvMotorSpeed.M7.accTimes=10;
			sDrvMotorSpeed.M7.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M7);
			sDrvMotorSpeed.M7.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M7);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M7.start_bk=sDrvMotorSpeed.M7.start=10000;
			sDrvMotorSpeed.M7.max=20000;	
			sDrvMotorSpeed.M7.accSpeed=10000;
			sDrvMotorSpeed.M7.dec=40;
			sDrvMotorSpeed.M7.accTimes=10;
			sDrvMotorSpeed.M7.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M7);
			sDrvMotorSpeed.M7.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M7);
		}	
		for(i=0; i<=sDrvMotorSpeed.M7.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M7.start + i*sDrvMotorSpeed.M7.interval_acc;
			sDrvMotorSpeed.M7.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
		

#endif




		
#ifdef USE_DRV8434_PECKER					
		case M8_BIG_IN_OUT:
			memset(&sDrvMotorSpeed.M8,0,  sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){		
			sDrvMotorSpeed.M8.start_bk=sDrvMotorSpeed.M8.start=10000;
			sDrvMotorSpeed.M8.max=20000;	
			sDrvMotorSpeed.M8.accSpeed=10000;
			sDrvMotorSpeed.M8.dec=40;
			sDrvMotorSpeed.M8.accTimes=10;
			sDrvMotorSpeed.M8.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M8);
			sDrvMotorSpeed.M8.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M8);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M8.start_bk=sDrvMotorSpeed.M8.start=15000;
			sDrvMotorSpeed.M8.max=30000;	
			sDrvMotorSpeed.M8.accSpeed=15000;
			sDrvMotorSpeed.M8.dec=40;
			sDrvMotorSpeed.M8.accTimes=10;
			sDrvMotorSpeed.M8.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M8);
			sDrvMotorSpeed.M8.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M8);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M8.start_bk=sDrvMotorSpeed.M8.start=10000;
			sDrvMotorSpeed.M8.max=20000;	
			sDrvMotorSpeed.M8.accSpeed=10000;
			sDrvMotorSpeed.M8.dec=40;
			sDrvMotorSpeed.M8.accTimes=10;
			sDrvMotorSpeed.M8.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M8);
			sDrvMotorSpeed.M8.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M8);
		}
		for(i=0; i<=sDrvMotorSpeed.M8.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M8.start + i*sDrvMotorSpeed.M8.interval_acc;
			sDrvMotorSpeed.M8.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
		
		case M9_IN_OUT:
			memset(&sDrvMotorSpeed.M9, 0, sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){	
			sDrvMotorSpeed.M9.start_bk=sDrvMotorSpeed.M9.start=10000;
			sDrvMotorSpeed.M9.max=20000;	
			sDrvMotorSpeed.M9.accSpeed=10000;
			sDrvMotorSpeed.M9.dec=40;
			sDrvMotorSpeed.M9.accTimes=10;
			sDrvMotorSpeed.M9.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M9);
			sDrvMotorSpeed.M9.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M9);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M9.start_bk=sDrvMotorSpeed.M9.start=15000;
			sDrvMotorSpeed.M9.max=30000;	
			sDrvMotorSpeed.M9.accSpeed=15000;
			sDrvMotorSpeed.M9.dec=40;
			sDrvMotorSpeed.M9.accTimes=10;
			sDrvMotorSpeed.M9.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M9);
			sDrvMotorSpeed.M9.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M9);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M9.start_bk=sDrvMotorSpeed.M9.start=10000;
			sDrvMotorSpeed.M9.max=30000;	
			sDrvMotorSpeed.M9.accSpeed=10000;
			sDrvMotorSpeed.M9.dec=40;
			sDrvMotorSpeed.M9.accTimes=10;
			sDrvMotorSpeed.M9.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M9);
			sDrvMotorSpeed.M9.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M9);
		}	
		for(i=0; i<=sDrvMotorSpeed.M9.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M9.start + i*sDrvMotorSpeed.M9.interval_acc;
			sDrvMotorSpeed.M9.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
		
		case M10_UP_DOWM:
			memset(&sDrvMotorSpeed.M10, 0, sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){	
			sDrvMotorSpeed.M10.start_bk=sDrvMotorSpeed.M10.start=10000;
			sDrvMotorSpeed.M10.max=20000;	
			sDrvMotorSpeed.M10.accSpeed=10000;
			sDrvMotorSpeed.M10.dec=40;
			sDrvMotorSpeed.M10.accTimes=10;
			sDrvMotorSpeed.M10.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M10);
			sDrvMotorSpeed.M10.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M10);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M10.start_bk=sDrvMotorSpeed.M10.start=5000;
			sDrvMotorSpeed.M10.max=10000;	
			sDrvMotorSpeed.M10.accSpeed=5000;
			sDrvMotorSpeed.M10.dec=40;
			sDrvMotorSpeed.M10.accTimes=10;
			sDrvMotorSpeed.M10.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M10);
			sDrvMotorSpeed.M10.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M10);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M10.start_bk=sDrvMotorSpeed.M10.start=10000;
			sDrvMotorSpeed.M10.max=40000;	
			sDrvMotorSpeed.M10.accSpeed=10000;
			sDrvMotorSpeed.M10.dec=40;
			sDrvMotorSpeed.M10.accTimes=10;
			sDrvMotorSpeed.M10.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M10);
			sDrvMotorSpeed.M10.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M10);
		}	
		
		for(i=0; i<=sDrvMotorSpeed.M10.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M10.start + i*sDrvMotorSpeed.M10.interval_acc;
			sDrvMotorSpeed.M10.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
		
		case M11_FAR_NEAR:
			memset(&sDrvMotorSpeed.M11, 0, sizeof(Drv_Motor_Speed_Config_t));
		if(LOW_SPEED==speed_type){		
			sDrvMotorSpeed.M11.start_bk=sDrvMotorSpeed.M11.start=3000;
			sDrvMotorSpeed.M11.max=5000;	
			sDrvMotorSpeed.M11.accSpeed=2000;
			sDrvMotorSpeed.M11.dec=40;
			sDrvMotorSpeed.M11.accTimes=10;
			sDrvMotorSpeed.M11.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M11);
			sDrvMotorSpeed.M11.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M11);
		}else if(NORMAL_SPEED==speed_type){
			sDrvMotorSpeed.M11.start_bk=sDrvMotorSpeed.M11.start=3000;
			sDrvMotorSpeed.M11.max=5000;	
			sDrvMotorSpeed.M11.accSpeed=2000;
			sDrvMotorSpeed.M11.dec=40;
			sDrvMotorSpeed.M11.accTimes=10;
			sDrvMotorSpeed.M11.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M11);
			sDrvMotorSpeed.M11.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M11);
		}else if(HIGH_SPEED==speed_type){
			sDrvMotorSpeed.M11.start_bk=sDrvMotorSpeed.M11.start=5000;
			sDrvMotorSpeed.M11.max=15000;	
			sDrvMotorSpeed.M11.accSpeed=10000;
			sDrvMotorSpeed.M11.dec=40;
			sDrvMotorSpeed.M11.accTimes=10;
			sDrvMotorSpeed.M11.interval_pwm=__DRV8434_GET_ACC_INTERVAL_PWM_COUNT_(sDrvMotorSpeed.M11);
			sDrvMotorSpeed.M11.interval_acc=__DRV8434_GET_ACC_PER_TIMES_ADD__(sDrvMotorSpeed.M11);
		}	
		for(i=0; i<=sDrvMotorSpeed.M11.accTimes; i++)
		{
			speed = sDrvMotorSpeed.M11.start + i*sDrvMotorSpeed.M11.interval_acc;
			sDrvMotorSpeed.M11.config[i]=ceil(72000000/(divClock*speed));
		}
			break;
			
#endif			
		default:
	}
		
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
	
	
	switch(deviceId)
	{
		case M1_MIX_V:
			if( (sDrvMotorSpeed.M1.start < sDrvMotorSpeed.M1.max) && (0 == currentIndex%sDrvMotorSpeed.M1.interval_pwm))
			{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M1.start + sDrvMotorSpeed.M1.interval_acc > sDrvMotorSpeed.M1.max){sDrvMotorSpeed.M1.start=sDrvMotorSpeed.M1.max;}
					else{sDrvMotorSpeed.M1.start+=sDrvMotorSpeed.M1.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M1.interval_pwm;
					config = sDrvMotorSpeed.M1.config[index];		
			}	
		break;
			
		case M2_MIX:
				if( (sDrvMotorSpeed.M2.start < sDrvMotorSpeed.M2.max) && (0 == currentIndex%sDrvMotorSpeed.M2.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M2.start + sDrvMotorSpeed.M2.interval_acc > sDrvMotorSpeed.M2.max){sDrvMotorSpeed.M2.start=sDrvMotorSpeed.M2.max;}
					else{sDrvMotorSpeed.M2.start+=sDrvMotorSpeed.M2.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M2.interval_pwm;
					config = sDrvMotorSpeed.M2.config[index];		
				}	
				break;
		
		case M3_LEFT_WAIT:
				if( (sDrvMotorSpeed.M3.start < sDrvMotorSpeed.M3.max) && (0 == currentIndex%sDrvMotorSpeed.M3.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M3.start + sDrvMotorSpeed.M3.interval_acc > sDrvMotorSpeed.M3.max){sDrvMotorSpeed.M3.start=sDrvMotorSpeed.M3.max;}
					else{sDrvMotorSpeed.M3.start+=sDrvMotorSpeed.M3.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M3.interval_pwm;
					config = sDrvMotorSpeed.M3.config[index];		
				}	
				break;
		

		case M4_BLANK_NEXT:
				if( (sDrvMotorSpeed.M4.start < sDrvMotorSpeed.M4.max) && (0 == currentIndex%sDrvMotorSpeed.M4.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M4.start + sDrvMotorSpeed.M4.interval_acc > sDrvMotorSpeed.M4.max){sDrvMotorSpeed.M4.start=sDrvMotorSpeed.M4.max;}
					else{sDrvMotorSpeed.M4.start+=sDrvMotorSpeed.M4.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M4.interval_pwm;
					config = sDrvMotorSpeed.M4.config[index];		
				}	
				break;
		
		
		case M5_WAIT_NEXT:
				if( (sDrvMotorSpeed.M5.start < sDrvMotorSpeed.M5.max) && (0 == currentIndex%sDrvMotorSpeed.M5.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M5.start + sDrvMotorSpeed.M5.interval_acc > sDrvMotorSpeed.M5.max){sDrvMotorSpeed.M5.start=sDrvMotorSpeed.M5.max;}
					else{sDrvMotorSpeed.M5.start+=sDrvMotorSpeed.M5.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M5.interval_pwm;
					config = sDrvMotorSpeed.M5.config[index];		
				}	
				break;
		
		
		case M6_BLANK_LEFT:
				if( (sDrvMotorSpeed.M6.start < sDrvMotorSpeed.M6.max) && (0 == currentIndex%sDrvMotorSpeed.M6.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M6.start + sDrvMotorSpeed.M6.interval_acc > sDrvMotorSpeed.M6.max){sDrvMotorSpeed.M6.start=sDrvMotorSpeed.M6.max;}
					else{sDrvMotorSpeed.M6.start+=sDrvMotorSpeed.M6.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M6.interval_pwm;
					config = sDrvMotorSpeed.M6.config[index];		
				}	
				break;
		
		case M7_HIGH_TURN:
				if( (sDrvMotorSpeed.M7.start < sDrvMotorSpeed.M7.max) && (0 == currentIndex%sDrvMotorSpeed.M7.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M7.start + sDrvMotorSpeed.M7.interval_acc > sDrvMotorSpeed.M7.max){sDrvMotorSpeed.M7.start=sDrvMotorSpeed.M7.max;}
					else{sDrvMotorSpeed.M7.start+=sDrvMotorSpeed.M7.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M7.interval_pwm;
					config = sDrvMotorSpeed.M7.config[index];		
				}	
				break;
		
		
		case M8_BIG_IN_OUT:
				if( (sDrvMotorSpeed.M8.start < sDrvMotorSpeed.M8.max) && (0 == currentIndex%sDrvMotorSpeed.M8.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M8.start + sDrvMotorSpeed.M8.interval_acc > sDrvMotorSpeed.M8.max){sDrvMotorSpeed.M8.start=sDrvMotorSpeed.M8.max;}
					else{sDrvMotorSpeed.M8.start+=sDrvMotorSpeed.M8.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M8.interval_pwm;
					config = sDrvMotorSpeed.M8.config[index];		
				}	
				break;
		
		case M9_IN_OUT:
				if( (sDrvMotorSpeed.M9.start < sDrvMotorSpeed.M9.max) && (0 == currentIndex%sDrvMotorSpeed.M9.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M9.start + sDrvMotorSpeed.M9.interval_acc > sDrvMotorSpeed.M9.max){sDrvMotorSpeed.M9.start=sDrvMotorSpeed.M9.max;}
					else{sDrvMotorSpeed.M9.start+=sDrvMotorSpeed.M9.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M9.interval_pwm;
					config = sDrvMotorSpeed.M9.config[index];		
				}	
				break;
		
		case M10_UP_DOWM:
				if( (sDrvMotorSpeed.M10.start < sDrvMotorSpeed.M10.max) && (0 == currentIndex%sDrvMotorSpeed.M10.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M10.start + sDrvMotorSpeed.M10.interval_acc > sDrvMotorSpeed.M10.max){sDrvMotorSpeed.M10.start=sDrvMotorSpeed.M10.max;}
					else{sDrvMotorSpeed.M10.start+=sDrvMotorSpeed.M10.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M10.interval_pwm;
					config = sDrvMotorSpeed.M10.config[index];		
				}	
				break;
		
		case M11_FAR_NEAR:
				if( (sDrvMotorSpeed.M11.start < sDrvMotorSpeed.M11.max) && (0 == currentIndex%sDrvMotorSpeed.M11.interval_pwm))
				{
					//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
					if(sDrvMotorSpeed.M11.start + sDrvMotorSpeed.M11.interval_acc > sDrvMotorSpeed.M11.max){sDrvMotorSpeed.M11.start=sDrvMotorSpeed.M11.max;}
					else{sDrvMotorSpeed.M11.start+=sDrvMotorSpeed.M11.interval_acc;}

					index = currentIndex/sDrvMotorSpeed.M11.interval_pwm;
					config = sDrvMotorSpeed.M11.config[index];		
				}	
				break;
		
		default:
	}
	
	return config;
/*	
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
	if( (Ms.start < Ms.max) && (0 == currentIndex%Ms.interval_pwm))
	{
			//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
			if(Ms.start + Ms.interval_acc > Ms.max)
			{
					Ms.start=Ms.max;
			}else
			{
					Ms.start+=Ms.interval_acc;
			}
			
			index = currentIndex/Ms.interval_pwm;
			config = Ms.config[index];		
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
*/	
}



int __DRV8434_Motor_Caculate_Next_Dec_Speed__(uint8_t deviceId)
{
	int next=0;

	
	
	return next;
}









