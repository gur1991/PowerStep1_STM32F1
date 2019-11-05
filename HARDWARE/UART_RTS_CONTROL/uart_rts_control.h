#ifndef __UART_RTS_CONTROL_H
#define __UART_RTS_CONTROL_H
#include "sys.h"


#define	RTS0 		PAout(7)
#define	RTS1 		PCout(4) 
#define	RTS2 		PCout(5)
#define	RTS3 		PBout(0) 

void Uart_Rts_Control_Init(void);
void uart_rts_control(uint8_t devices, uint8_t status);

#endif

