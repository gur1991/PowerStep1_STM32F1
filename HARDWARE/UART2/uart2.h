#ifndef __UART2_H
#define __UART2_H
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

#define MAX_LENGTH 128

extern u8 USART2_RX_BUF[MAX_LENGTH]; 	
extern int USART2_RX_CNT;   			

extern int FLAG_UART_MASTER;
extern int FLAG_UART_CHEMINERT;
extern int FLAG_RECEIVE_ACK;

void UART2_Init(u32 bound);
void UART2_Send_Data(u8 *buf,int len);
void UART2_Receive_Data(u8 *buf,int *len);	
#endif
