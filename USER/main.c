
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"
#include "light.h"
#include "uart3.h"
#include "uart4.h"
#include "uart2.h"
#include "light.h"
#include "weight.h"
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
#include "wdg.h"
#include "step01.h"


int main(void)
{	
	HAL_Init();                    	 	
  Stm32_Clock_Init(RCC_PLL_MUL9);   	
	delay_init(72);               		
	uart_init(115200);					
	usmart_dev.init(84); 		   	
	UART4_Init(115200);
	IWDG_Init(4,625*6); //6s   	MAX
  IWDG_Start();
	
		
#if USE_SENSOR_BOARD	
	TIM3_PWM_Init(500-1,72-1);
	//TIM3_PWM_Init(CLK_COUNT-1,72-1);
	TIM_SetTIM3Compare4(500);	
	ThermometerChooseHandle(DS18B20);
	ThermometerHandle->init();
	
	
	UART2_Init(9600);
	UART3_Init(9600);
  Uart_cs_init();
	
	AD_Sensor_Init();//四个重力传感器初始化
	
	Electromagnetic_init();//电磁阀
	
	printf("sensor board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

#if USE_MOTOR_BOARD
	Light_Sensor_Init();
	BSP_Motor_Control_Init();
	
	printf("motor board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif	

   // RestSelectMotorOrgin(1,6, BACKWARD ,25000,0);

		//PowerStep_Select_Motor_Baby(4);
		//BSP_MotorControl_Move(0, FORWARD, 20000);

while(1){

	//printf("light:%d \r\n",Light_Sensor_Get(7));
	//delay_ms(100);
#if 1	

		if(ARM_RS232_ASK){
						printf("start receive !\r\n");
						delay_ms(5);
						protocol_handle_uart_powerstep01_plain_slave_cmd();
						ARM_RS232_ASK=0;
		}	
		
		#if USE_SENSOR_BOARD
			delay_ms(MAIN_DELAY);
		#else
			delay_ms(5);
		#endif
		
		#if USE_SENSOR_BOARD	
			ThermometerHandle->keep_degree();
			//printf("tmp1:%d\r\n",DS18B20_Get_Temp(1));
			//printf("tmp2:%d\r\n",DS18B20_Get_Temp(2));
		#endif
		
#endif		
		IWDG_Feed();
	}

	return 0;
}
