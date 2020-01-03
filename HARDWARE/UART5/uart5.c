
#include "uart5.h"
#include "uart_command_control_protocol.h"

UART_HandleTypeDef UART5_Handler;  

u8 UART5_RX_BUF[LEN_MAX_UART5];  	
int UART5_RX_CNT=0;  
int MINI_RS232_ASK=0;

void UART5_IRQHandler(void)
{
    u8 res;
    if((__HAL_UART_GET_FLAG(&UART5_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断
	 {	 	
		HAL_UART_Receive(&UART5_Handler,&res,1,10);//115200 256byte 需要20ms，现在给30ms
		if(UART5_RX_CNT<LEN_MAX_UART5)
		{
			UART5_RX_BUF[UART5_RX_CNT]=res;		//记录接收到的值
			UART5_RX_CNT++;						//接收数据增加1	
		} 
		if(UART5_RX_CNT==sizeof(Powerstep1_contorl_motor_command_t))
		{
					MINI_RS232_ASK=1;
		}

	}
} 

void UART5_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_UART5_CLK_ENABLE();			

    GPIO_Initure.Pin = GPIO_PIN_12;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_2;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

	
	UART5_Handler.Instance=UART5;			        //UART5
	UART5_Handler.Init.BaudRate=bound;		        //波特率
	UART5_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//字长为8位数据格式
	UART5_Handler.Init.StopBits=UART_STOPBITS_1;		//一个停止位
	UART5_Handler.Init.Parity=UART_PARITY_NONE;		//无奇偶校验位
	UART5_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	UART5_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	UART5_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART5_Handler);			        //HAL_UART_Init()会使能USART2
  
	//HAL_UART_Receive_IT(&UART5_Handler, &aRxBuffer_UART5, 1);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  __HAL_UART_DISABLE_IT(&UART5_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&UART5_Handler,UART_IT_RXNE);//开启接收中断
	HAL_NVIC_EnableIRQ(UART5_IRQn);				        //使能UART5中断
	HAL_NVIC_SetPriority(UART5_IRQn,3,3);			        //抢占优先级3，子优先级3

}

void UART5_Send_Data(u8 *buf,int len)
{
	HAL_UART_Transmit(&UART5_Handler,buf,len,100);//串口4发送数据
	UART5_RX_CNT=0;
}
//buf:接收缓存首地址
//len:读到的数据长度
void UART5_Receive_Data(u8 *buf,int *len)
{
	int rxlen=UART5_RX_CNT;
	int i=0;
	*len=0;				//默认为0
	if(rxlen==UART5_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=UART5_RX_BUF[i];	
		}		
		*len=UART5_RX_CNT;	//记录本次数据长度
		UART5_RX_CNT=0;		//清零
	}
} 

void Uart5_Rx_Clear(void)
{
	UART5_RX_CNT=0;		//清零
}	