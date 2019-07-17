#ifndef __UART4_H
#define __UART4_H
#include "sys.h"

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


#define LEN_MAX_UART4 256
extern u8 ARM_RS232_ASK;
extern UART_HandleTypeDef UART4_Handler;
extern u8 UART4_RX_BUF[LEN_MAX_UART4]; 		
extern int UART4_RX_CNT;   			

//模式控制
#define UART4_TX_EN		1 
#define EN_USART4_RX 	1		

void UART4_Init(u32 bound);
void UART4_Send_Data(u8 *buf,int len);
void UART4_Receive_Data(u8 *buf,int *len);

void HAL_UART_RxCpltCallback_Uart4(void);
#endif
