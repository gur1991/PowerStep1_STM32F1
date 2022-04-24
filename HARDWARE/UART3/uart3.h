#ifndef __UART3_H
#define __UART3_H
#include "sys.h"
#include "config.h"

#define LEN_MAX_UART3 128

void Uart3_Rx_Clear(void);


extern int UART3_RX_CNT;   	


#define UART3_TX_EN		1

#define EN_USART3_RX 	1		

void UART3_Init(u32 bound);
void UART3_Send_Data(u8 *buf,int len);
void UART3_Receive_Data(u8 *buf,int *len);	
#endif
