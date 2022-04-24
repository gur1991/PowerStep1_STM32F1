#include "uart3.h"
#include "usart.h"
#include "delay.h"
#include "uart_command_control_protocol.h"

//串口3只编译，没有使用；串口2会使用串口3定义的变量


UART_HandleTypeDef USART3_Handler; 

u8 UART3_RX_BUF[LEN_MAX_UART3];  
int UART3_RX_CNT=0;  


void USART3_IRQHandler(void)
{
    u8 res;
    if((__HAL_UART_GET_FLAG(&USART3_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断
	{	 	
		HAL_UART_Receive(&USART3_Handler,&res,1,2);
		if(UART3_RX_CNT<LEN_MAX_UART3)
		{
			UART3_RX_BUF[UART3_RX_CNT]=res;		//记录接收到的值
			UART3_RX_CNT++;						//接收数据增加1	
		} 
		

	}
} 

//初始化IO 串口2
//bound:波特率
void UART3_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
	__HAL_RCC_AFIO_CLK_ENABLE();
#if 1  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_Initure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_11;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
#else	
//PDF 错误
		GPIO_Initure.Pin = GPIO_PIN_11;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
#endif	
	
	
    //USART 初始化设置
	USART3_Handler.Instance=USART3;			        //USART2
	USART3_Handler.Init.BaudRate=bound;		        //波特率
	USART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//字长为8位数据格式
	USART3_Handler.Init.StopBits=UART_STOPBITS_1;		//一个停止位
	USART3_Handler.Init.Parity= UART_PARITY_NONE;//UART_PARITY_NONE;		//无奇偶校验位
	USART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	USART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	USART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART3_Handler);			        //HAL_UART_Init()会使能USART2
  //HAL_UART_Receive_IT(&USART3_Handler, (u8 *)aRxBuffer_UART3, RXBUFFERSIZE_UART3);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  __HAL_UART_DISABLE_IT(&USART3_Handler,UART_IT_TC);
#if EN_USART3_RX
	__HAL_UART_ENABLE_IT(&USART3_Handler,UART_IT_RXNE);//开启接收中断
	HAL_NVIC_EnableIRQ(USART3_IRQn);				        //使能USART1中断
	HAL_NVIC_SetPriority(USART3_IRQn,2,3);			        //抢占优先级3，子优先级3
#endif	
}

//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void UART3_Send_Data(u8 *buf,int len)
{
	HAL_UART_Transmit(&USART3_Handler,buf,len,10);
	UART3_RX_CNT=0;
}
//buf:接收缓存首地址
//len:读到的数据长度
void UART3_Receive_Data(u8 *buf,int *len)
{
	int rxlen=UART3_RX_CNT;
	int i=0;
	*len=0;				//默认为0
	if(rxlen==UART3_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=UART3_RX_BUF[i];	
		}		
		*len=UART3_RX_CNT;	//记录本次数据长度
		UART3_RX_CNT=0;		//清零
	}
} 
void Uart3_Rx_Clear(void)
{
	UART3_RX_CNT=0;		//清零
}	
