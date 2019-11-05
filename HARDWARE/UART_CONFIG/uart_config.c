#include "uart_config.h"




void Uart2_Config_Init(void)
{
#if USE_CLEANING_DILUTION_BOARD			
		UART2_Init(9600);
#endif

#if USE_GRADIENT_CONTROL_BOARD			
		UART2_Init(115200);
#endif	

//条码器和RFID共用一个串口，需要把RFID工作后，再把串口配置成115200	
#if USE_AUTOMATIC_INJECTION_BOARD			
		UART2_Init(9600);
#endif	

}


void Load_RFID_Uart_Config(void)
{
		UART2_Init(115200);
}

void Exit_RFID_Uart_Config(void)
{
		UART2_Init(9600);
}

void Load_BL180_Uart_Config(void)
{
		UART2_Init_Check(9600);
}

void Exit_BL180_Uart_Config(void)
{
		UART2_Init(9600);
}


