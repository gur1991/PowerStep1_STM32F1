#ifndef __UART4_H
#define __UART4_H
#include "sys.h"
#include "config.h"

void Uart4_Rx_Clear(void);

#define LEN_MAX_UART4 128
extern u8 ARM_RS232_ASK;

extern u8 UART4_RX_BUF[LEN_MAX_UART4]; 		
extern int UART4_RX_CNT;   			

//Ä£Ê½¿ØÖÆ
#define UART4_TX_EN		1 
#define EN_USART4_RX 	1		

void UART4_Init(u32 bound);
void UART4_Send_Data(u8 *buf,int len);
void UART4_Receive_Data(u8 *buf,int RxLen, int *len);
void Uart4_Rx_Clear(void);
void UART4_Enable_Interrupt(void);
void UART4_Disable_Interrupt(void);
#endif
