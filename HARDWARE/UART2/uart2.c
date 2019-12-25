#include "uart2.h"
#include "delay.h"
#include "uart_command_control_protocol.h"


UART_HandleTypeDef USART2_Handler;  

//接收缓存区 	
u8 USART2_RX_BUF[MAX_LENGTH];  	
//接收到的数据长度
int USART2_RX_CNT=0;  
u8 FLAG_UART_BL180_ACK=0;
u8 FLAG_RECEIVE_ANSOWER_BL180=0;

u8 FLAG_UART_CHEMINERT=0;

void USART2_IRQHandler(void)
{
    u8 res;	  
    if((__HAL_UART_GET_FLAG(&USART2_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断
	{	
		HAL_UART_Receive(&USART2_Handler,&res,1,1);
		if(USART2_RX_CNT<MAX_LENGTH)
		{
			
			USART2_RX_BUF[USART2_RX_CNT]=res;		//记录接收到的值
			//printf("zzzz%d\r\n",res);
			USART2_RX_CNT++;						//接收数据增加1 
		}
		
#if USE_CLEANING_DILUTION_BOARD		
		//具体根据CHEMINERT实际
		if(USART2_RX_CNT>=3&&(USART2_RX_BUF[USART2_RX_CNT-2]==0x0d&&USART2_RX_BUF[USART2_RX_CNT-1]==0x0a))
		{
				FLAG_UART_CHEMINERT=1;
		}
#endif

#if USE_AUTOMATIC_INJECTION_BOARD		
		if(USART2_RX_CNT>=7 && USART2_RX_BUF[0]==0xff)
		{
				if(USART2_RX_CNT == USART2_RX_BUF[1]+7)FLAG_RECEIVE_RFID=1;
		}
		
		if((USART2_RX_BUF[1]=='K')&&(USART2_RX_BUF[0]=='O'))
		{
				FLAG_UART_BL180_ACK=1;
		}
		if(USART2_RX_CNT>5&&(USART2_RX_BUF[USART2_RX_CNT-1]==0x0a||USART2_RX_BUF[USART2_RX_CNT-1]==0x0d)){
					FLAG_RECEIVE_ANSOWER_BL180=1;
		}
		
		if(USART2_RX_CNT>4&&USART2_RX_BUF[USART2_RX_CNT-1]==0x0a&&USART2_RX_BUF[USART2_RX_CNT-2]==0x0d)
		{
				FLAG_UART_FM100=1;
		}
		if(USART2_RX_CNT==4&&(USART2_RX_BUF[USART2_RX_CNT-1]=='@')&&(USART2_RX_BUF[USART2_RX_CNT-2]=='@'))
		{
				FLAG_UART_FM100_INTO=1;
		}
		if(USART2_RX_CNT==4&&(USART2_RX_BUF[USART2_RX_CNT-1]=='^')&&(USART2_RX_BUF[USART2_RX_CNT-2]=='^'))
		{
				FLAG_UART_FM100_EXIT=1;
		}
		if(USART2_RX_CNT==10&&(USART2_RX_BUF[USART2_RX_CNT-1]==';'))
		{
				FLAG_UART_FM100_ACK=1;
		}
#endif		

#if USE_GRADIENT_CONTROL_BOARD				

		
		if(USART2_RX_CNT==1&&(USART2_RX_BUF[0]==0x23||USART2_RX_BUF[0]==0x24||USART2_RX_BUF[0]==0x25)){
					FLAG_RECEIVE_ACK_PUMP100=1;
		}
		
		if(USART2_RX_CNT>2&&(USART2_RX_BUF[USART2_RX_CNT-1]==0x0a||USART2_RX_BUF[USART2_RX_CNT-1]==0x0d)){
					FLAG_RECEIVE_ANSOWER_PUMP100=1;
		}	
#endif		
		
	} 
}    


//初始化IO 串口2
//bound:波特率
void UART2_Init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
	__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
	
	GPIO_Initure.Pin=GPIO_PIN_2; 			//PA2
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;		//高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2
	
	GPIO_Initure.Pin=GPIO_PIN_3; 			//PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//复用输入
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3
/*	
	//PD7推挽输出，485模式控制  
    GPIO_Initure.Pin=GPIO_PIN_7; 			//PD7
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
  */  
    //USART 初始化设置
	USART2_Handler.Instance=USART2;			        //USART2
	USART2_Handler.Init.BaudRate=bound;		        //波特率
	USART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//字长为8位数据格式
	USART2_Handler.Init.StopBits=UART_STOPBITS_1;		//一个停止位
	USART2_Handler.Init.Parity=UART_PARITY_NONE;		//无奇偶校验位
	USART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	USART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	USART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART2_Handler);			        //HAL_UART_Init()会使能USART2
    
  __HAL_UART_DISABLE_IT(&USART2_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&USART2_Handler,UART_IT_RXNE);//开启接收中断
	HAL_NVIC_EnableIRQ(USART2_IRQn);				        //使能USART1中断
	HAL_NVIC_SetPriority(USART2_IRQn,3,3);			        //抢占优先级3，子优先级3
}


//初始化IO 串口2
//bound:波特率
void UART2_Init_Check(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
	__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
	
	GPIO_Initure.Pin=GPIO_PIN_2; 			//PA2
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;		//高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2
	
	GPIO_Initure.Pin=GPIO_PIN_3; 			//PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//复用输入
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3
	

    //USART 初始化设置
	USART2_Handler.Instance=USART2;			        //USART2
	USART2_Handler.Init.BaudRate=bound;		        //波特率
	USART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//字长为8位数据格式
	USART2_Handler.Init.StopBits=UART_STOPBITS_1;		//一个停止位
	USART2_Handler.Init.Parity=UART_PARITY_EVEN;		//无奇偶校验位
	USART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//无硬件流控
	USART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	USART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART2_Handler);			        //HAL_UART_Init()会使能USART2
    
  __HAL_UART_DISABLE_IT(&USART2_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&USART2_Handler,UART_IT_RXNE);//开启接收中断
	HAL_NVIC_EnableIRQ(USART2_IRQn);				        //使能USART1中断
	HAL_NVIC_SetPriority(USART2_IRQn,3,3);			        //抢占优先级3，子优先级3
}



//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void UART2_Send_Data(u8 *buf,int len)
{
	//printf("uart2 send\r\n");
	HAL_UART_Transmit(&USART2_Handler,buf,len,100);//串口2发送数据
	USART2_RX_CNT=0;
}
//buf:接收缓存首地址
//len:读到的数据长度
void UART2_Receive_Data(u8 *buf,int *len)
{
	int rxlen=USART2_RX_CNT;
	int i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==USART2_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART2_RX_BUF[i];	
		}		
		*len=USART2_RX_CNT;	//记录本次数据长度
		USART2_RX_CNT=0;		//清零
	}
} 
