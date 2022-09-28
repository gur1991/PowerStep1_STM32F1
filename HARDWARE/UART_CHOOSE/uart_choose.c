#include "uart_choose.h"
#include "delay.h"
#include "config.h"

//static  UART_CS_TYPE UART2_STAUS_LAST=CS_FOUR,UART3_STAUS_LAST=CS_FOUR;

void Uart_cs_init(void)
{

//PA5--1  PA6--0
#if USE_GRADIENT_CONTROL_BOARD	
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);
  
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

//PF5--1  PF6--0
#if USE_CLEANING_DILUTION_BOARD	
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOF_CLK_ENABLE();
  
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);
  
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
#endif


//PA1--1  PA4--0
#if USE_AUTOMATIC_INJECTION_BOARD	
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_4, GPIO_PIN_RESET);
  
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

}



//uart 2或 3
//chip 0-3
int Uart_Select_Baby(UART_TYPE uart, UART_CS_TYPE cs)
{
		int ret=0;
		if(uart==UART2_RS232){
				switch(cs)
				{
					case CS_ZERO:
						UART2_L_CS=0;
						UART2_H_CS=0;
						break;
					case CS_ONE:
						UART2_L_CS=1;
						UART2_H_CS=0;
						break;
					case CS_TWO:
						UART2_L_CS=0;
						UART2_H_CS=1;
						break;
					case CS_THREE:
						UART2_L_CS=1;
						UART2_H_CS=1;
						break;
					default:
						ret=-1;
						break;
				}
		}else if(uart==UART3_RS232)
		{
				switch(cs)
				{
					case CS_ZERO:
						UART3_L_CS=0;
						UART3_H_CS=0;
						break;
					case CS_ONE:
						UART3_L_CS=1;
						UART3_H_CS=0;
						break;
					case CS_TWO:
						UART3_L_CS=0;
						UART3_H_CS=1;
						break;
					case CS_THREE:
						UART3_L_CS=1;
						UART3_H_CS=1;
						break;
					default:
						ret=-1;
						break;
				}
			}

		//此处一定要延时，两个作用，模拟电路电平发出需要时间切换
		//rs232接收数据需要等待
		//delay_ms(5);
		return ret;
	
	
/*	
		int ret=0;
		static int flag_first_uart2=1;
		static int flag_first_uart3=1;
	
		if(uart==UART2_RS232){
			if(UART2_STAUS_LAST==cs) return 0;
			else UART2_STAUS_LAST=cs;
			
				switch(cs)
				{
					case CS_ZERO:
						UART2_L_CS=0;
						UART2_H_CS=0;
						break;
					case CS_ONE:
						UART2_L_CS=1;
						UART2_H_CS=0;
						break;
					case CS_TWO:
						UART2_L_CS=0;
						UART2_H_CS=1;
						break;
					case CS_THREE:
						UART2_L_CS=1;
						UART2_H_CS=1;
						break;
					default:
						ret=-1;
						break;
				}
				if(flag_first_uart2)
				{
					delay_ms(100);
					flag_first_uart2=0;
				}
		}else if(uart==UART3_RS232)
		{

				if(UART3_STAUS_LAST==cs) return 0;
				else UART3_STAUS_LAST=cs;
			
				switch(cs)
				{
					case CS_ZERO:
						UART3_L_CS=0;
						UART3_H_CS=0;
						break;
					case CS_ONE:
						UART3_L_CS=1;
						UART3_H_CS=0;
						break;
					case CS_TWO:
						UART3_L_CS=0;
						UART3_H_CS=1;
						break;
					case CS_THREE:
						UART3_L_CS=1;
						UART3_H_CS=1;
						break;
					default:
						ret=-1;
						break;
				}
				if(flag_first_uart3)
				{
					delay_ms(10);
					flag_first_uart3=0;
				}
		}else{
				ret=-1;
		}
		
		
		//此处一定要延时，两个作用，模拟电路电平发出需要时间切换
		//rs232接收数据需要等待
		delay_ms(5);
		return ret;
*/		
}	

/*
*
*获取接收函数，CS并进行选择
*/
 Uart_Receive_Data GetUartReceive(UART_TYPE uart, UART_CS_TYPE cs)
{	
		//if(Uart_Select_Baby(uart, cs))return NULL;
	
		if(uart==UART2_RS232){
			return UART2_Receive_Data;
		}
		else if(uart==UART3_RS232){
			return UART3_Receive_Data;
		}

		return NULL;
}

/*
*
*获取发送函数，CS并进行选择
*/
 Uart_Send_Data GetUartSend(UART_TYPE uart, UART_CS_TYPE cs)
{
		//if(Uart_Select_Baby(uart, cs))return NULL;
	
		if(uart==UART2_RS232)return UART2_Send_Data;
		else if(uart==UART3_RS232)return UART3_Send_Data;
		
		return NULL;
}

/****************************************/
void example(void)
{
	u8 txbuf[2]={0x0a, 0x03};
	
	Uart_Receive_Data R232_Read = GetUartReceive(UART2_RS232, CS_ZERO);
	Uart_Send_Data R232_Write = GetUartSend(UART2_RS232, CS_ZERO);
	R232_Write(txbuf, sizeof(txbuf));
	//printf();

}



