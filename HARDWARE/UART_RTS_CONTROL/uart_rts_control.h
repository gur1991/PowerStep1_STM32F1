#ifndef __UART_RTS_CONTROL_H
#define __UART_RTS_CONTROL_H
#include "sys.h"
#include "config.h"

#define	RTS0 		PAout(7)
#define	RTS1 		PGout(9) 
#define	RTS2 		PCout(5)
#define	RTS3 		PBout(0) 

#define	CTS3 		PCout(4)

void Uart_Rts_Control_Init(void);
void uart_rts_control(uint8_t devices, uint8_t status);
void uart_cts_control(uint8_t devices, uint8_t status);

#endif


