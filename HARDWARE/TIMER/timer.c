#include "timer.h"
#include "led.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "config.h"
	
TIM_HandleTypeDef 	TIM3_Handler;      	//定时器句柄 
TIM_HandleTypeDef 	TIM5_Handler;      	//定时器句柄 

TIM_OC_InitTypeDef 	TIM3_CH4Handler;


//通用定时器5中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM5_Init(u16 arr,u16 psc)
{  
    TIM5_Handler.Instance=TIM5;                          //通用定时器4
    TIM5_Handler.Init.Prescaler=psc;                     //分频系数
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM5_Handler.Init.Period=arr;                        //自动装载值
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); //使能定时器5和定时器5更新中断：TIM_IT_UPDATE   
}

//TIM3 PWM部分初始化 
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;          	//定时器3
    TIM3_Handler.Init.Prescaler=psc;       //定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM3_Handler.Init.Period=arr;          //自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
    
    //TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    //TIM3_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    //TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		//HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);//配置TIM3通道2
		//HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//开启PWM通道2
	
	  TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH4Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
	 	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//配置TIM3通道4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//开启PWM通道4
}
#if USE_SENSOR_BOARD
//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟
		HAL_NVIC_SetPriority(TIM5_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM5中断   
	}
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
		//__HAL_AFIO_REMAP_TIM3_PARTIAL();		//TIM3通道引脚部分重映射使能
		//__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
		__HAL_AFIO_REMAP_TIM3_ENABLE();//TIM3通道引脚重映射使能
		__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
		
		GPIO_Initure.Pin=GPIO_PIN_9;           	//PC9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
		HAL_GPIO_Init(GPIOC,&GPIO_Initure); 	
	}
}
#endif
//设置TIM通道2的占空比
//compare:比较值
void TIM_SetTIM3Compare2(u32 compare)
{
	TIM3->CCR2=compare; 
}

//设置TIM通道2的占空比
//compare:比较值
void TIM_SetTIM3Compare4(u32 compare)
{
	TIM3->CCR4=compare; 
}

void TIM5_IRQHandler(void)
{ 		    		  			       
    if(__HAL_TIM_GET_IT_SOURCE(&TIM5_Handler,TIM_IT_UPDATE)==SET)//溢出中断
    {
				keep_thermometer_degree();
        __HAL_TIM_SET_COUNTER(&TIM5_Handler,0);;    //清空定时器的CNT
        __HAL_TIM_SET_AUTORELOAD(&TIM5_Handler,COUNT_TIME);//恢复原来的设置
    }
    __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_UPDATE);//清除中断标志位
}


/*
//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM4_Handler);
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM3_Handler))
    {
			printf("hello \r\n");
       // LED1=!LED1;        //LED1反转
    }
}
*/