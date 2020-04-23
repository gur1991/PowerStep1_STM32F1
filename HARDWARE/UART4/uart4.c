#include "uart4.h"
#include "delay.h"
#include "uart_command_control_protocol.h"
#include "config.h"

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
		HAL_UART_Receive(&UART4_Handler,&res,1,30);//115200 256byte 需要20ms，现在给30ms
			
		//如果上次指令尚没有结束，就不会接受新的指令 
		if(ARM_RS232_ASK)return;
		 
		 
		if(UART4_RX_CNT<LEN_MAX_UART4)
		{
			UART4_RX_BUF[UART4_RX_CNT]=res;		//记录接收到的值
			//LOGD("r[%d]0x%x\r\n",UART4_RX_CNT,res);
			UART4_RX_CNT++;						//接收数据增加1	
			
		}
/*		
		if(UART4_RX_CNT == 2)
		{
				if( UART4_RX_BUF[0]==START_UART_VALUE0 && UART4_RX_BUF[1]==START_UART_VALUE1){ ;}
				else {UART4_RX_BUF[0]= UART4_RX_BUF[1];UART4_RX_CNT=1; LOGE("fuck\r\n");}
		}
*/
//		else 
			if(UART4_RX_CNT==sizeof(Powerstep1_contorl_motor_command_t))
		{
					ARM_RS232_ASK=1;
		}

	}
} 

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
void UART4_Receive_Data(u8 *buf,int RxLen, int *len)
{
	int rxlen=UART4_RX_CNT;
	int i=0;
	*len=0;				
	if(rxlen<RxLen)
	{
		*len=rxlen;
		return;
	}
		
	else if(rxlen==UART4_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<RxLen;i++)
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

