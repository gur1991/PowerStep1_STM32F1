#ifndef __UART3_H
#define __UART3_H
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
#define LEN_MAX_UART3 128

extern u8 FLAG_UART_SLAVE3;
extern UART_HandleTypeDef USART3_Handler;
extern u8 UART3_RX_BUF[LEN_MAX_UART3]; 		//接收缓冲,最大64个字节
extern int UART3_RX_CNT;   			//接收到的数据长度


#define UART3_TX_EN		1

#define EN_USART3_RX 	1		

void UART3_Init(u32 bound);
void UART3_Send_Data(u8 *buf,int len);
void UART3_Receive_Data(u8 *buf,int *len);	
#endif
