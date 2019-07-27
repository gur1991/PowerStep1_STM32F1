#ifndef __UART_CHOOSE_H
#define __UART_CHOOSE_H
#include "uart3.h"
#include "uart2.h"

#define	UART2_H_CS 		PDout(14)  		
#define	UART2_L_CS 		PDout(15)  	

#define	UART3_H_CS 		PGout(3)  		
#define	UART3_L_CS 		PGout(2)  		


typedef enum{
		UART2_RS232=2,
		UART3_RS232
}UART_TYPE; 

typedef enum{
		CS_ZERO=0,
		CS_ONE,
		CS_TWO,
		CS_THREE
}UART_CS_TYPE; 

typedef struct UART_STAUS{
		UART_TYPE uart;
		UART_CS_TYPE cs;
}UART_STAUS_TYPE;


void Uart_cs_init(void);

int Uart_Select_Baby(UART_TYPE uart, UART_CS_TYPE cs);

typedef void(*Uart_Receive_Data)(u8 *buf,int *len);

typedef void(*Uart_Send_Data)(u8 *buf,int len);



Uart_Receive_Data GetUartReceive(UART_TYPE uart, UART_CS_TYPE cs);

Uart_Send_Data GetUartSend(UART_TYPE uart, UART_CS_TYPE cs);

void example(void);
#endif
