#ifndef __UART_CONFIG_H
#define __UART_CONFIG_H
#include "uart2.h"
#include "uart3.h"

void Uart_Clear_Context(void);

void Uart2_Config_Init(void);


void Load_RFID_Uart_Config(void);
void Exit_RFID_Uart_Config(void);


void Load_BL180_Uart_Config(void);
void Exit_BL180_Uart_Config(void);

#endif



