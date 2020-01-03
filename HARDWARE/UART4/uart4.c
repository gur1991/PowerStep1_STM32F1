#include "uart4.h"
#include "delay.h"
#include "uart_command_control_protocol.h"

UART_HandleTypeDef UART4_Handler;  

//接收缓存区 	
u8 UART4_RX_BUF[LEN_MAX_UART4];  	
//接收到的数据长度
 int UART4_RX_CNT=0;  
u8 ARM_RS232_ASK =0;


void UART4_IRQHandler(void)
{
    u8 res;
    if((__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断
	 {	 	
		HAL_UART_Receive(&UART4_Handler,&res,1,10);//115200 256byte 需要20ms，现在给30ms
		if(UART4_RX_CNT<LEN_MAX_UART4)
		{
			UART4_RX_BUF[UART4_RX_CNT]=res;		//记录接收到的值
			UART4_RX_CNT++;						//接收数据增加1	
		} 
		if(UART4_RX_CNT==sizeof(Powerstep1_contorl_motor_command_t))
		{
					ARM_RS232_ASK=1;
		}

	}
} 
/*
u16 USART_RX_STA4=0;       //接收状态标记	  
u16 USART_RX_STA_UART4=0; 

#if 1
void HAL_UART_RxCpltCallback_Uart4(void){    
	u8 aRxBuffer[1];
		if((USART_RX_STA4&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA4&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA4=0;//接收错误,重新开始
				else USART_RX_STA4|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA4|=0x4000;
				else
				{
							UART4_RX_BUF[UART4_RX_CNT]=aRxBuffer[0];		//记录接收到的值
							printf("rx[%d]%d\r\n",UART4_RX_CNT,aRxBuffer[0]);
							UART4_RX_CNT++;						//接收数据增加1	
					if(USART_RX_STA4>(64-1))USART_RX_STA4=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
}

#endif

//串口1中断服务程序
void UART4_IRQHandler(void)                	
{ 
	u32 timeout=0;

	HAL_UART_IRQHandler(&UART4_Handler);	//调用HAL库中断处理公用函数
	timeout=0;
   while (HAL_UART_GetState(&UART4_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART4_Handler, &aRxBuffer_UART4, 1) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; //超时处理
		if(timeout>HAL_MAX_DELAY) break;	
		
		

	}

} 
*/

//初始化IO 串口2
//bound:波特率
void UART4_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	//__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_UART4_CLK_ENABLE();			

    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_11;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

	
	
    //USART 初始化设置
	UART4_Handler.Instance=UART4;			        //USART2
	UART4_Handler.Init.BaudRate=bound;		        //波特率
	UART4_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//字长为8位数据格式
	UART4_Handler.Init.StopBits=UART_STOPBITS_1;		//一个停止位
	UART4_Handler.Init.Parity=UART_PARITY_NONE;		//无奇偶校验位
	UART4_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	UART4_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	UART4_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART4_Handler);			        //HAL_UART_Init()会使能USART2
  
	//HAL_UART_Receive_IT(&UART4_Handler, &aRxBuffer_UART4, 1);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  __HAL_UART_DISABLE_IT(&UART4_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&UART4_Handler,UART_IT_RXNE);//开启接收中断
	HAL_NVIC_EnableIRQ(UART4_IRQn);				        //使能UART4中断
	HAL_NVIC_SetPriority(UART4_IRQn,3,3);			        //抢占优先级3，子优先级3

}

//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void UART4_Send_Data(u8 *buf,int len)
{
	HAL_UART_Transmit(&UART4_Handler,buf,len,100);//串口4发送数据
	UART4_RX_CNT=0;
}
//buf:接收缓存首地址
//len:读到的数据长度
void UART4_Receive_Data(u8 *buf,int *len)
{
	int rxlen=UART4_RX_CNT;
	int i=0;
	*len=0;				//默认为0
	if(rxlen==UART4_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=UART4_RX_BUF[i];	
		}		
		*len=UART4_RX_CNT;	//记录本次数据长度
		UART4_RX_CNT=0;		//清零
	}
} 
void Uart4_Rx_Clear(void)
{
	UART4_RX_CNT=0;			//清零
}	

