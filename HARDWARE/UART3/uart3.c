#include "uart3.h"
#include "usart.h"
#include "delay.h"
#include "uart_command_control_protocol.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//RS485驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

UART_HandleTypeDef USART3_Handler;  //USART2句柄(用于RS485)
//#define  RXBUFFERSIZE_UART3 1
//u8 aRxBuffer_uart3[RXBUFFERSIZE_UART3];//HAL库使用的串口接收缓冲

#if EN_USART3_RX   		//如果使能了接收   	  
//接收缓存区 	
u8 UART3_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 UART3_RX_CNT=0;  
u8 FLAG_UART_SLAVE =0;

void USART3_IRQHandler(void)
{
    u8 res;
    if((__HAL_UART_GET_FLAG(&USART3_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断
	{	 	
		HAL_UART_Receive(&USART3_Handler,&res,1,1000);
		if(UART3_RX_CNT<64)
		{
			UART3_RX_BUF[UART3_RX_CNT]=res;		//记录接收到的值
			//printf("slave uart %d  UART3_RX_BUF:%d\r\n",UART3_RX_BUF[UART3_RX_CNT],UART3_RX_CNT);
			//printf(" rx_buf[%d] %d \r\n",UART3_RX_CNT,UART3_RX_BUF[UART3_RX_CNT]);
			UART3_RX_CNT++;						//接收数据增加1	
		} 
		
		if(UART3_RX_CNT>=3&&UART3_RX_BUF[UART3_RX_CNT-1]==OVER_UART_VALUE1&&UART3_RX_BUF[UART3_RX_CNT-2]==OVER_UART_VALUE0){
					//printf(" slave over \r\n");
					//protocol_handle_uart_powerstep01_plain_slave_cmd();
					FLAG_UART_SLAVE=1;
		}

	} 
} 
/*

//串口1中断服务程序
void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif

	HAL_UART_IRQHandler(&USART3_Handler);	//调用HAL库中断处理公用函数
	timeout=0;
   while (HAL_UART_GetState(&USART3_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&USART3_Handler, (u8 *)aRxBuffer_UART3, RXBUFFERSIZE_UART3) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 
*/
#endif

#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB1Periph_USART3            ((uint32_t)0x00040000)
#define GPIO_PartialRemap_USART3    ((uint32_t)0x00140010)  /*!< USART3 Partial Alternate Function mapping */

//初始化IO 串口2
//bound:波特率
void UART3_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
	//RCC_APB2PeriphClockCmd();
//	GPIO_PinRemapConfig();
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟
 // RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	
#if 1  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
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
	USART3_Handler.Init.Parity=UART_PARITY_NONE;		//无奇偶校验位
	USART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	USART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	USART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART3_Handler);			        //HAL_UART_Init()会使能USART2
  //HAL_UART_Receive_IT(&USART3_Handler, (u8 *)aRxBuffer_UART3, RXBUFFERSIZE_UART3);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  __HAL_UART_DISABLE_IT(&USART3_Handler,UART_IT_TC);
#if EN_USART3_RX
	__HAL_UART_ENABLE_IT(&USART3_Handler,UART_IT_RXNE);//开启接收中断
	HAL_NVIC_EnableIRQ(USART3_IRQn);				        //使能USART1中断
	HAL_NVIC_SetPriority(USART3_IRQn,3,3);			        //抢占优先级3，子优先级3
#endif	
//	UART3_TX_EN=0;											//默认为接收模式		
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void UART3_Send_Data(u8 *buf,u8 len)
{
//	UART3_TX_EN=1;			//设置为发送模式
	HAL_UART_Transmit(&USART3_Handler,buf,len,1000);//串口2发送数据
//	UART3_TX_EN=0;				//设置为接收模式	
	UART3_RX_CNT=0;
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void UART3_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=UART3_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	//delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
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
