#ifndef __UART4_H
#define __UART4_H
#include "sys.h"



#define LEN_MAX_UART4 128
extern u8 ARM_RS232_ASK;

extern u8 UART4_RX_BUF[LEN_MAX_UART4]; 		
extern int UART4_RX_CNT;   			

//ģʽ����
#define UART4_TX_EN		1 
#define EN_USART4_RX 	1		

void UART4_Init(u32 bound);
void UART4_Send_Data(u8 *buf,int len);
void UART4_Receive_Data(u8 *buf,int *len);


#endif
