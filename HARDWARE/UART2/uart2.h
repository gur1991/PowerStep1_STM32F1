#ifndef __UART2_H
#define __UART2_H
#include "sys.h"
#include "config.h"
#include "uart3.h"

#define MAX_LENGTH 260

extern u8 USART2_RX_BUF[MAX_LENGTH]; 	
extern int USART2_RX_CNT;   			

extern u8 FLAG_UART_CHEMINERT;
extern u8 FLAG_UART_BL180_ACK;
extern u8 FLAG_RECEIVE_ANSOWER_BL180;

extern u8 FLAG_UART_FM100;
extern u8 FLAG_UART_FM100_INTO;
extern u8 FLAG_UART_FM100_EXIT;
extern u8 FLAG_UART_FM100_ACK;

extern u8 FLAG_RECEIVE_ACK_PUMP100;
extern u8 FLAG_RECEIVE_ANSOWER_PUMP100;

void UART2_Init(u32 bound);
void UART2_Init_Check(u32 bound);

void UART2_Send_Data(u8 *buf,int len);
void UART2_Receive_Data(u8 *buf,int *len);	
#endif
