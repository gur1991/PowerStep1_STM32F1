#include "drv8434.h"


TIM_HandleTypeDef 	DRV_TIM3_Handler;      	//定时器句柄 
TIM_HandleTypeDef 	DRV_TIM4_Handler;      	//定时器句柄 

TIM_OC_InitTypeDef 	DRV_CHHandler;

const int divClock=2;
//max 50k pwm

void DRV8434_MOTOR_Config_Init(void)
{
	//PWM频率=72000000/900=80Khz
	DRV8434_GPIO_Init();
	DRV8434_PWM3_Init(900-1,divClock-1);//2-1 为40K
	DRV8434_PWM4_Init(900-1,divClock-1);//10-10为8K  
	
	__HAL_TIM_ENABLE_IT(&DRV_TIM3_Handler, TIM_IT_UPDATE);
//	HAL_TIM_PWM_Start_IT(&DRV_TIM3_Handler,TIM_CHANNEL_3|TIM_CHANNEL_4);
	HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
	HAL_NVIC_EnableIRQ(TIM3_IRQn);  
	
	__HAL_TIM_ENABLE_IT(&DRV_TIM4_Handler, TIM_IT_UPDATE);
//	HAL_TIM_PWM_Start_IT(&DRV_TIM4_Handler,TIM_CHANNEL_1|TIM_CHANNEL_2);
	HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
  HAL_NVIC_EnableIRQ(TIM4_IRQn);  
	


#ifdef USE_DRV8434_CAMEL	
	DRV8434_Motor_Select_Speed(M1_MIX_V, NORMAL_SPEED);
	DRV8434_Motor_Select_Speed(M2_MIX, NORMAL_SPEED);
	DRV8434_Motor_Select_Speed(M3_LEFT_WAIT, NORMAL_SPEED);
	DRV8434_Motor_Select_Speed(M4_BLANK_NEXT, NORMAL_SPEED);
	
	DRV8434_Motor_Select_Speed(M5_WAIT_NEXT, NORMAL_SPEED);
	DRV8434_Motor_Select_Speed(M6_BLANK_LEFT, NORMAL_SPEED);
	DRV8434_Motor_Select_Speed(M7_HIGH_TURN, NORMAL_SPEED);
#endif	
	
//配置初始速度
#ifdef USE_DRV8434_PECKER	
	DRV8434_Motor_Select_Speed(M8_BIG_IN_OUT, NORMAL_SPEED);
	DRV8434_Motor_Select_Speed(M9_IN_OUT, NORMAL_SPEED);
	DRV8434_Motor_Select_Speed(M10_UP_DOWM, NORMAL_SPEED);
	DRV8434_Motor_Select_Speed(M11_FAR_NEAR, NORMAL_SPEED);
#endif


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
    DRV_CHHandler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
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
    DRV_CHHandler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
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
		
		
		//GPIO-PD12-HIGH
	//	DRV_CHHandler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
  //  DRV_CHHandler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
   // DRV_CHHandler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	 	
		//HAL_TIM_PWM_ConfigChannel(&DRV_TIM4_Handler,&DRV_CHHandler,TIM_CHANNEL_1);//配置TIM3通道4	
		//HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_1);
		
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



		__HAL_RCC_TIM4_CLK_ENABLE();
		__HAL_AFIO_REMAP_TIM4_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_12|GPIO_PIN_13;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOD,&GPIO_InitStruct); 	
		
		
		
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器	
		__HAL_AFIO_REMAP_TIM3_ENABLE();   //TIM3通道引脚重映射使能
		__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_9|GPIO_PIN_8;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOC,&GPIO_InitStruct); 	
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


		__HAL_RCC_TIM4_CLK_ENABLE();
		__HAL_AFIO_REMAP_TIM4_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_InitStruct.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;  //PC9
		GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_InitStruct.Pull=GPIO_PULLUP;          //上拉
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOD,&GPIO_InitStruct); 	
		
		
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
		GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOC,&GPIO_InitStruct); 	
#endif


}	


/********************************************************************/

static uint32_t sTIM_PWM_COUNT_INDEX=0, sTIM_PWM_COUNT_MAX=0xFFFFFFFE;
static bool DRV_MOTOR_WAIT_OK=false;
static Drv_Motor_Speed_Config_Group_t sDrvMotorSpeed;
static uint8_t sMOTOR_DEVICE=0;
static int sMOTOR_CONFIG=0;




//中断统计脉冲数量，清除中断标识
void TIM3_IRQHandler(void)
{ 	
	    
    if(__HAL_TIM_GET_IT_SOURCE(&DRV_TIM3_Handler,TIM_IT_UPDATE)==SET)//溢出中断
    {
				__HAL_TIM_CLEAR_IT(&DRV_TIM3_Handler, TIM_IT_UPDATE);//清除中断标志位		
				sTIM_PWM_COUNT_INDEX++;
				if(sTIM_PWM_COUNT_INDEX>=sTIM_PWM_COUNT_MAX)
				{	
					switch(sMOTOR_DEVICE)
					{
#ifdef USE_DRV8434_PECKER			
							case M8_BIG_IN_OUT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_4);break;
							case M9_IN_OUT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_3);break;		
#endif

#ifdef USE_DRV8434_CAMEL			
							case M1_MIX_V:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_4);break;
							case M2_MIX:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_3);break;
							case M3_LEFT_WAIT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_2);break;
							case M4_BLANK_NEXT:HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_1);break;		
#endif
							default:
					}
					sTIM_PWM_COUNT_INDEX=0;
					sTIM_PWM_COUNT_MAX=0xFFFFFFFE;
					DRV_MOTOR_WAIT_OK=true;
				}		
				else
				{
						sMOTOR_CONFIG= __DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
		
						if(0!=sMOTOR_CONFIG)
						{	
							switch(sMOTOR_DEVICE)
							{
							#ifdef USE_DRV8434_PECKER		
							case M8_BIG_IN_OUT:
							if(0!=sMOTOR_CONFIG){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG);
								TIM3->CCR4=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2; 
							}
							break;

							case M9_IN_OUT:
							if(0!=sMOTOR_CONFIG){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG);
								TIM3->CCR3=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2; 
							}
							break;
							#endif

							#ifdef USE_DRV8434_CAMEL		
							case M1_MIX_V:
							if(0!=sMOTOR_CONFIG){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG);
								TIM3->CCR4=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2; 
							}
							break;
							case M2_MIX:
							if(0!=sMOTOR_CONFIG){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG);
								TIM3->CCR3=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2; 
							}
							break;
							case M3_LEFT_WAIT:
							if(0!=sMOTOR_CONFIG){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG);
								TIM3->CCR2=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2; 
							}
							break;
							case M4_BLANK_NEXT:
							if(0!=sMOTOR_CONFIG){
								__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,sMOTOR_CONFIG);
								TIM3->CCR1=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2; 
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
				sTIM_PWM_COUNT_INDEX++;
				
				if(sTIM_PWM_COUNT_INDEX>=sTIM_PWM_COUNT_MAX)
				{
						switch(sMOTOR_DEVICE){
				#ifdef USE_DRV8434_PECKER			
								case M10_UP_DOWM:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_2);break;
								case M11_FAR_NEAR:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_1);break;			
				#endif
				#ifdef USE_DRV8434_CAMEL			
								case M5_WAIT_NEXT:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_4);break;
								case M6_BLANK_LEFT:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_3);break;
								case M7_HIGH_TURN:HAL_TIM_PWM_Stop(&DRV_TIM4_Handler,TIM_CHANNEL_2);break;
				#endif
								default:
				}
					
					sTIM_PWM_COUNT_INDEX=0;
					sTIM_PWM_COUNT_MAX=0xFFFFFFFE;
					DRV_MOTOR_WAIT_OK=true;
				}else
				{
						sMOTOR_CONFIG= __DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
						if(0!=sMOTOR_CONFIG)
						{	
								switch(sMOTOR_DEVICE)
								{
								#ifdef USE_DRV8434_PECKER		
								case M10_UP_DOWM:
								if(0!=sMOTOR_CONFIG){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG);
									TIM4->CCR2=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2; 
								}
								break;

								case M11_FAR_NEAR:
								if(0!=sMOTOR_CONFIG){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG);
									TIM4->CCR1=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2; 
								}
								break;
								#endif

								#ifdef USE_DRV8434_CAMEL		
								case M5_WAIT_NEXT:
								if(0!=sMOTOR_CONFIG){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG);
									TIM4->CCR4=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2; 
								}
								break;
								case M6_BLANK_LEFT:
								if(0!=sMOTOR_CONFIG){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG);
									TIM4->CCR3=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2; 
								}
								break;
								case M7_HIGH_TURN:
								if(0!=sMOTOR_CONFIG){
									__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,sMOTOR_CONFIG);
									TIM4->CCR2=__HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2; 
								} 
								break;
								#endif
								default:
								}
					}		
				}
		}
 
}


void DRV8434_Motor_Move_Steps(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
	
	//step-1 初始化脉冲计数
	sTIM_PWM_COUNT_INDEX=0;
	sTIM_PWM_COUNT_MAX=stepCount;
	DRV_MOTOR_WAIT_OK=false;
	sMOTOR_DEVICE=deviceId;
		
	//step-2 运动
	DRV8434_Motor_Move( deviceId,  direction);
	
	//step-3:  DRV goto sleep
	//DRV8434_Motor_Stop_Control_Timing(deviceId);
#endif



}



void DRV8434_Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
	
//	LOGD("start \r\n");
	//step-1 初始化脉冲计数
	sTIM_PWM_COUNT_INDEX=0;
	sTIM_PWM_COUNT_MAX=stepCount;
	DRV_MOTOR_WAIT_OK=false;
	sMOTOR_DEVICE=deviceId;
	
//	LOGD("index:%d  max:%d \r\n",sTIM_PWM_COUNT_INDEX,sTIM_PWM_COUNT_MAX);
	
		//step-2 运动
	DRV8434_Motor_Move( deviceId,  direction);
	//step-3 计数等待结束
	while(1)
	{
		if(DRV_MOTOR_WAIT_OK)break;//等待执行结束
		delay_ms(1);
		//LOGD("* \r\n");
	}
	
	//step-3:  DRV goto sleep
	DRV8434_Motor_Stop_Control_Timing(deviceId);
	//LOGD("end \r\n");
#endif	
}





void DRV8434_Motor_Move(uint8_t deviceId, motorDir_t direction )
{
	sMOTOR_DEVICE=deviceId;
	
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

			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM3->CCR4= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_4);
			break;
		
		case M9_IN_OUT:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM3->CCR3= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_3);
			break;
		
		case M10_UP_DOWM:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM4->CCR2= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_2);
			break;
		
		case M11_FAR_NEAR:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM4->CCR1= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_1);
			break;
#endif

#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM3->CCR4= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_4);
			break;
		case M2_MIX:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM3->CCR3= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_3);
			break;
		case M3_LEFT_WAIT:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM3->CCR2= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_2);
			break;
		case M4_BLANK_NEXT:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM3_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM3->CCR1= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM3_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM3_Handler,TIM_CHANNEL_1);
			break;
		case M5_WAIT_NEXT:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM4->CCR4= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_4);
			break;
		case M6_BLANK_LEFT:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM4->CCR3= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2;
			HAL_TIM_PWM_Start(&DRV_TIM4_Handler,TIM_CHANNEL_3);
			break;
		case M7_HIGH_TURN:
			sMOTOR_CONFIG=__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE);
			if(0!=sMOTOR_CONFIG){
				__HAL_TIM_SET_AUTORELOAD(&DRV_TIM4_Handler,__DRV8434_Motor_Caculate_Next_Acc_Speed__(sMOTOR_DEVICE));
			}
			TIM4->CCR2= __HAL_TIM_GET_AUTORELOAD(&DRV_TIM4_Handler)/2;
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
		case M2_MIX:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_3);
			break;
		case M3_LEFT_WAIT:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_2);
			break;
		case M4_BLANK_NEXT:
			HAL_TIM_PWM_Stop(&DRV_TIM3_Handler,TIM_CHANNEL_1);
			break;
		case M5_WAIT_NEXT:
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
	//LOGD("&& %d \r\n",(int)(72000000/(10*speed)));
	
	//LOGD("speed %f \r\n",ceil(72000000/(divClock*speed)));
	//return (int)(72000000/(divClock*speed));
	return ceil(72000000/(divClock*speed));
}	

double __DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(double max, double start, double acc)
{
	double f = (max+start)*0.5;
	//LOGD("f: %f \r\n",f);
	double perAcc= acc/f;	
	//return ceil(perAcc);
	//LOGD("acc: %f \r\n",perAcc);
	return perAcc;
}	

void __DRV8434_Motor_Get_Speed_BK_Config__(uint8_t deviceId)
{
	switch(deviceId)
	{
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:sDrvMotorSpeed.M1.use=sDrvMotorSpeed.M1.bk;break;		
		case M2_MIX:sDrvMotorSpeed.M2.use=sDrvMotorSpeed.M2.bk;break;
		case M3_LEFT_WAIT:sDrvMotorSpeed.M3.use=sDrvMotorSpeed.M3.bk;break;
		case M4_BLANK_NEXT:sDrvMotorSpeed.M4.use=sDrvMotorSpeed.M4.bk;break;
		case M5_WAIT_NEXT:sDrvMotorSpeed.M5.use=sDrvMotorSpeed.M5.bk;break;
		case M6_BLANK_LEFT:sDrvMotorSpeed.M6.use=sDrvMotorSpeed.M6.bk;break;
		case M7_HIGH_TURN:sDrvMotorSpeed.M7.use=sDrvMotorSpeed.M7.bk;break;
#endif
#ifdef USE_DRV8434_PECKER					
		case M8_BIG_IN_OUT:sDrvMotorSpeed.M8.use=sDrvMotorSpeed.M8.bk;break;
		case M9_IN_OUT:sDrvMotorSpeed.M9.use=sDrvMotorSpeed.M9.bk;break;
		case M10_UP_DOWM:sDrvMotorSpeed.M10.use=sDrvMotorSpeed.M10.bk;break;
		case M11_FAR_NEAR:sDrvMotorSpeed.M11.use=sDrvMotorSpeed.M11.bk;break;
#endif			
		default:
	}

}	
void DRV8434_Motor_Select_Speed(uint8_t deviceId, MOTOR_SPEED_type_t speed_type)
{
	Drv_Motor_Speed_Config_t Ms;
	int config=0;
	switch(deviceId)
	{
		
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:
			Ms.slow.start=2000;
			Ms.slow.max=10000;	
			Ms.slow.accSpeed=300;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=2000;
			Ms.normal.max=10000;	
			Ms.normal.accSpeed=300;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=10000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
			
		case M2_MIX:
			Ms.slow.start=2000;
			Ms.slow.max=10000;	
			Ms.slow.accSpeed=300;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=2000;
			Ms.normal.max=10000;	
			Ms.normal.accSpeed=300;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=10000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		case M3_LEFT_WAIT:
			Ms.slow.start=2000;
			Ms.slow.max=1000;	
			Ms.slow.accSpeed=300;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=2000;
			Ms.normal.max=1000;	
			Ms.normal.accSpeed=300;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=1000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

		
			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		case M4_BLANK_NEXT:
			Ms.slow.start=2000;
			Ms.slow.max=1000;	
			Ms.slow.accSpeed=300;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=2000;
			Ms.normal.max=1000;	
			Ms.normal.accSpeed=300;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=1000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		case M5_WAIT_NEXT:
			Ms.slow.start=2000;
			Ms.slow.max=1000;	
			Ms.slow.accSpeed=300;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=2000;
			Ms.normal.max=1000;	
			Ms.normal.accSpeed=300;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=1000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		case M6_BLANK_LEFT:
			Ms.slow.start=2000;
			Ms.slow.max=1000;	
			Ms.slow.accSpeed=300;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=2000;
			Ms.normal.max=1000;	
			Ms.normal.accSpeed=300;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=1000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);
			
			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		case M7_HIGH_TURN:
			Ms.slow.start=2000;
			Ms.slow.max=1000;	
			Ms.slow.accSpeed=300;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=2000;
			Ms.normal.max=1000;	
			Ms.normal.accSpeed=300;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=1000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		

#endif




		
#ifdef USE_DRV8434_PECKER					
		case M8_BIG_IN_OUT:
			Ms.slow.start=2000;
			Ms.slow.max=6000;	
			Ms.slow.accSpeed=3000;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=3000;
			Ms.normal.max=21000;	
			Ms.normal.accSpeed=3000;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=1000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		
		case M9_IN_OUT:
			Ms.slow.start=2000;
			Ms.slow.max=6000;	
			Ms.slow.accSpeed=3000;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=3000;
			Ms.normal.max=21000;	
			Ms.normal.accSpeed=3000;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=10000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		
		case M10_UP_DOWM:
			Ms.slow.start=2000;
			Ms.slow.max=6000;	
			Ms.slow.accSpeed=3000;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=3000;
			Ms.normal.max=21000;	
			Ms.normal.accSpeed=3000;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=1000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
		
		case M11_FAR_NEAR:
			Ms.slow.start=2000;
			Ms.slow.max=6000;	
			Ms.slow.accSpeed=3000;
			Ms.slow.dec=40;
			Ms.slow.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.slow.max,Ms.slow.start,Ms.slow.accSpeed);

		
			Ms.normal.start=3000;
			Ms.normal.max=21000;	
			Ms.normal.accSpeed=3000;
			Ms.normal.dec=40;
			Ms.normal.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.normal.max,Ms.normal.start,Ms.normal.accSpeed);

		
			Ms.high.start=2000;
			Ms.high.max=1000;	
			Ms.high.accSpeed=300;
			Ms.high.dec=40;
			Ms.high.acc=__DRV8434_GET_PER_PWM_ACC_FROM_ACC_AND_MAX_SPEED__(Ms.high.max,Ms.high.start,Ms.high.accSpeed);

			if(LOW_SPEED==speed_type){Ms.bk=Ms.slow;}
			else if(NORMAL_SPEED==speed_type){Ms.bk=Ms.normal;}
			else if(HIGH_SPEED==speed_type){Ms.bk=Ms.high;}
			break;
			
#endif			
		default:
	}
	

			
	switch(deviceId)
	{
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:sDrvMotorSpeed.M1=Ms;break;		
		case M2_MIX:sDrvMotorSpeed.M2=Ms;break;
		case M3_LEFT_WAIT:sDrvMotorSpeed.M3=Ms;break;
		case M4_BLANK_NEXT:sDrvMotorSpeed.M4=Ms;break;
		case M5_WAIT_NEXT:sDrvMotorSpeed.M5=Ms;break;
		case M6_BLANK_LEFT:sDrvMotorSpeed.M6=Ms;break;
		case M7_HIGH_TURN:sDrvMotorSpeed.M7=Ms;break;
#endif
#ifdef USE_DRV8434_PECKER					
		case M8_BIG_IN_OUT:sDrvMotorSpeed.M8=Ms;break;
		case M9_IN_OUT:sDrvMotorSpeed.M9=Ms;break;
		case M10_UP_DOWM:sDrvMotorSpeed.M10=Ms;break;
		case M11_FAR_NEAR:sDrvMotorSpeed.M11=Ms;break;
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
	delay_ms(1);//延时起timing
}	

void DRV8434_Motor_Stop_Control_Timing(uint8_t deviceId)
{
	  //nothing to do
		delay_ms(1);
	  return ;
	
		delay_ms(1);//延时起timing
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
		case M10_UP_DOWM:
			nSLEEP_10_OUT=0;
			ENABLE_10_OUT=1;
		
			break;
		case M11_FAR_NEAR:	
			nSLEEP_11_OUT=0;
			ENABLE_11_OUT=1;
		
			break;			
#endif

		default:
	}	
	//delay_ms(1);//延时起timing

}	



int __DRV8434_Motor_Caculate_Next_Acc_Speed__(uint8_t deviceId)
{
	int config=0;
	Drv_Motor_Speed_Config_t Ms;
	switch(deviceId)
	{
#ifdef USE_DRV8434_CAMEL		
		case M1_MIX_V:Ms=sDrvMotorSpeed.M1;break;
		case M2_MIX:Ms=sDrvMotorSpeed.M2;break;
		case M3_LEFT_WAIT:Ms=sDrvMotorSpeed.M3;break;
		case M4_BLANK_NEXT:Ms=sDrvMotorSpeed.M4;break;
		case M5_WAIT_NEXT:Ms=sDrvMotorSpeed.M5;break;
		case M6_BLANK_LEFT:Ms=sDrvMotorSpeed.M6;break;
		case M7_HIGH_TURN:Ms=sDrvMotorSpeed.M7;break;
#endif				
#ifdef USE_DRV8434_PECKER
		case M8_BIG_IN_OUT:Ms=sDrvMotorSpeed.M8;break;
		case M9_IN_OUT:Ms=sDrvMotorSpeed.M9;break;
		case M10_UP_DOWM:Ms=sDrvMotorSpeed.M10;break;
		case M11_FAR_NEAR:Ms=sDrvMotorSpeed.M11;break;	
#endif
		default:
	}
	//LOGD("start:%f max:%f \r\n",Ms.use.start, Ms.use.max);
	if(Ms.use.start < Ms.use.max)
	{
			//当前速度+加速度后如果大于目标速度，则下一个速度为目标速度
			if(Ms.use.start + Ms.use.acc > Ms.use.max)
			{
					Ms.use.start=Ms.use.max;
			}else
			{
					Ms.use.start+=Ms.use.acc;
			}
			config=__DRV8434_Motor_Speed_To_PWM_Config__(Ms.use.start);		

			

	}
				switch(deviceId)
			{
		#ifdef USE_DRV8434_CAMEL		
				case M1_MIX_V:sDrvMotorSpeed.M1=Ms;break;
				case M2_MIX:sDrvMotorSpeed.M2=Ms;break;
				case M3_LEFT_WAIT:sDrvMotorSpeed.M3=Ms;break;
				case M4_BLANK_NEXT:sDrvMotorSpeed.M4=Ms;break;
				case M5_WAIT_NEXT:sDrvMotorSpeed.M5=Ms;break;
				case M6_BLANK_LEFT:sDrvMotorSpeed.M6=Ms;break;
				case M7_HIGH_TURN:sDrvMotorSpeed.M7=Ms;break;
		#endif				
		#ifdef USE_DRV8434_PECKER
				case M8_BIG_IN_OUT:sDrvMotorSpeed.M8=Ms;break;
				case M9_IN_OUT:sDrvMotorSpeed.M9=Ms;break;
				case M10_UP_DOWM:sDrvMotorSpeed.M10=Ms;break;
				case M11_FAR_NEAR:sDrvMotorSpeed.M11=Ms;break;	
		#endif
				default:
			}
	return config;
}



int __DRV8434_Motor_Caculate_Next_Dec_Speed__(uint8_t deviceId)
{
	int next=0;

	
	
	return next;
}









