#ifndef __UART_CHOOSE_H
#define __UART_CHOOSE_H
#include "uart3.h"
#include "uart2.h"
#include "config.h"


 //PA5--1  PA6--0
#if USE_GRADIENT_CONTROL_BOARD	
#define	UART2_H_CS 		PAout(5)  		
#define	UART2_L_CS 		PAout(6) 
#endif


//PF5--1  PF6--0
#if USE_CLEANING_DILUTION_BOARD	
#define	UART2_H_CS 		PFout(5)  		
#define	UART2_L_CS 		PFout(6) 
#endif

//PA1--1  PA4--0
#if USE_AUTOMATIC_INJECTION_BOARD	
#define	UART2_H_CS 		PAout(1)  		
#define	UART2_L_CS 		PAout(4) 
#endif
	

#define	UART3_H_CS 		PGout(3)  		
#define	UART3_L_CS 		PGout(2)  		


typedef enum{
		UART2_RS232=2,
		UART3_RS232  //串口3，硬件并未使用，所以程序不可使用
}UART_TYPE; 

typedef enum{
		CS_ZERO=0,
		CS_ONE,
		CS_TWO,
		CS_THREE,
		CS_FOUR,
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
