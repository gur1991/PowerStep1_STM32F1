#ifndef __UART2_H
#define __UART2_H
#include "sys.h"
#include "uart3.h"
#include "config.h"


#define MAX_LENGTH 128

void Uart2_Rx_Clear(void);

extern u8 FLAG_SCAN_OK;
extern int USART2_RX_CNT;   			
extern u8 FLAG_RECEIVE_RFID;

void UART2_Config_Baurd(u32 bound);
void UART2_Init(u32 bound);
void UART2_Init_Check(u32 bound);

void UART2_Send_Data(u8 *buf,int len);
void UART2_Receive_Data(u8 *buf,int *len);	
#endif
