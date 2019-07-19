#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"
#include "light.h"
#include "uart3.h"
#include "uart4.h"
#include "uart2.h"
#include "light.h"
#include "slave_uart_control_interface.h"
#include "printpaper.h"
#include "baby18dad400.h"
#include "factory_single.h"
#include "factory_many.h"
#include "keep_temperature.h"
#include "temperature.h"
#include "uart_choose.h"
#include "config.h"
#include "electromagnetic.h"


int main(void)
{	
	HAL_Init();                    	 	
  Stm32_Clock_Init(RCC_PLL_MUL9);   	
	delay_init(72);               		
	uart_init(115200);					
	usmart_dev.init(84); 		   	
	UART4_Init(115200);
	
		
#if USE_SENSOR_BOARD	
	TIM3_PWM_Init(500-1,72-1);
	TIM_SetTIM3Compare4(500);	
	//温度传感器选择DS18B20，并初始化
	ThermometerChooseHandle(DS18B20);
	ThermometerHandle->init();

	UART2_Init(115200);
	UART3_Init(115200);
  Uart_cs_init();
	
	AD_Sensor_Init();//四个重力传感器初始化
	DS18B20_Init();//初始化溫度傳感器
	Electromagnetic_init();//电磁阀
	
	printf("sensor board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#else
	Light_Sensor_Init();
	BSP_Motor_Control_Init();
	printf("main board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif	
	

while(1){

		if(ARM_RS232_ASK){
						printf("start receive !\r\n");
						protocol_handle_uart_powerstep01_plain_slave_cmd();
						ARM_RS232_ASK=0;
		}	
		delay_ms(MAIN_DELAY);
		#if USE_SENSOR_BOARD	
			ThermometerHandle->keep_degree();
		#endif
	}
	return 0;
}
