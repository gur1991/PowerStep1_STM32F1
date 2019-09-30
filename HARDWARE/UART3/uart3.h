#ifndef __UART3_H
#define __UART3_H
#include "sys.h"

#define LEN_MAX_UART3 128

extern u8 UART3_RX_BUF[LEN_MAX_UART3]; 		
extern int UART3_RX_CNT;   			//接收到的数据长度

extern u8 FLAG_RECEIVE_ACK_PUMP100;
extern u8 FLAG_RECEIVE_ANSOWER_PUMP100;
extern u8 FLAG_UART_FM100;
extern u8 FLAG_UART_FM100_INTO;
extern u8 FLAG_UART_FM100_EXIT;
extern u8 FLAG_UART_FM100_ACK;
extern u8 FLAG_UART_BL180_ACK;

#define UART3_TX_EN		1

#define EN_USART3_RX 	1		

void UART3_Init(u32 bound);
void UART3_Send_Data(u8 *buf,int len);
void UART3_Receive_Data(u8 *buf,int *len);	
#endif
