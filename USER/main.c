#include "sys.h"
#include "delay.h"
#include "usart.h"
//#include "led.h"
//#include "key.h"
//#include "lcd.h"
#include "usmart.h"
//#include "w25qxx.h"
#include "light.h"
#include "uart3.h"
#include "uart4.h"
#include "rs485.h"
#include "light.h"
//#include "uart_command_control_protocol.h"
#include "slave_uart_control_interface.h"
#include "printpaper.h"
#include "baby18dad400.h"
#include "factory_single.h"
#include "factory_many.h"


int main(void)
{	
	HAL_Init();                    	 	
  Stm32_Clock_Init(RCC_PLL_MUL9);   	
	delay_init(72);               		
	uart_init(115200);					
	usmart_dev.init(84); 		   	
	Light_Sensor_Init();
	BSP_Motor_Control_Init();
	UART4_Init(115200);
	
//	TIM3_PWM_Init(500-1,72-1);       	//72M/72=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
//	AD_SENSOR_init();
	
//	RS485_Init(115200);//with rk3188 rs232 exchange info
//	UART3_Init(115200);//with slave rs232 exchange info
		
	printf("into slave device size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));


		
//	TIM_SetTIM3Compare4(500);	//修改比较值，修改占空比

while(1){
		if(ARM_RS232_ASK){
						printf("start receive !\r\n");
						protocol_handle_uart_powerstep01_plain_slave_cmd();
						ARM_RS232_ASK=0;
		}	
		delay_ms(1);
	}
	return 0;
}
