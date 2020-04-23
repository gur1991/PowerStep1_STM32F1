#include "uart_config.h"
#include "uart_choose.h"
#include "uart5.h"
#include "uart4.h"
#include "config.h"
#include "delay.h"
void Uart2_Config_Init(void)
{
#if USE_CLEANING_DILUTION_BOARD			
		UART2_Init(9600);
#endif

#if USE_GRADIENT_CONTROL_BOARD			
		UART2_Init(115200);
#endif	

//��������RFID����һ�����ڣ���Ҫ��RFID�������ٰѴ������ó�115200	
#if USE_AUTOMATIC_INJECTION_BOARD			
		UART2_Init(9600);
	  //UART2_Init(115200);
#endif	
	Uart_cs_init();
}


void Load_RFID_Uart_Config(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
		UART2_Init(115200);
#endif	
}

void Exit_RFID_Uart_Config(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
		UART2_Init(9600);
#endif	
}

void Load_BL180_Uart_Config(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
		UART2_Init_Check(9600);
#endif	
}

void Exit_BL180_Uart_Config(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
		UART2_Init(9600);
#endif	
}

void Uart_Clear_Context(void)
{
	
	Uart3_Rx_Clear();
	Uart2_Rx_Clear();

#if USE_KEEP_TEMPERATURE_BOARD	
	Uart5_Rx_Clear();
#endif
	
	Uart4_Rx_Clear();
}	
