#ifndef __UART_5_H
#define __UART_5_H
#include "sys.h"
#include "config.h"
#define LEN_MAX_UART5 128
void Uart5_Rx_Clear(void);


void UART5_Enable_Interrupt(void);	

void UART5_Disable_Interrupt(void);	


extern u8 UART5_RX_BUF[LEN_MAX_UART5]; 		
extern int UART5_RX_CNT;   			

//Ä£Ê½¿ØÖÆ
#define UART5_TX_EN		1 
#define EN_USART4_RX 	1		

void UART5_Init(u32 bound);
void UART5_Send_Data(u8 *buf,int len);
void UART5_Receive_Data(u8 *buf,int *len);

#endif


