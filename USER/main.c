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
#include "pump_s100.h"
#include "fm100.h"
#include "S1125.h"
#include "liquid_sensor.h"
#include "bl180.h"
#include "m6e_apply.h"
#include "key.h"

int main(void)
{	
	HAL_Init();                    	 	
  Stm32_Clock_Init(RCC_PLL_MUL9);   	
	delay_init(72);               		
	uart_init(115200);					
	//usmart_dev.init(84); 		   	
	UART4_Init(115200);
	//IWDG_Init(4,625*6); //6s   	MAX
  //IWDG_Start();
	int i=0;
	KEY_Init();

	#if USE_SENSOR_BOARD	
	TIM3_PWM_Init(500-1,72-1);
	Pid_init();
	TIM_SetTIM3Compare4(500);	
	ThermometerChooseHandle(DS18B20);
	ThermometerHandle->init();
  ThermometerHandle->set_degree(0,TMEPERATURE_CURRENT);
	Liquid_Sensor_Init();
	
	//UART2_Init_Check(9600);
	UART2_Init(9600);
	UART3_Init(115200);
  Uart_cs_init();
	AD_Sensor_Init();//四个重力传感器初始化
	
	Electromagnetic_init();//电磁阀
	
	//ScanChooseHandle(BL180);
	//ScanHandle->init(true);
	
	//PumpChooseHandle(S1125);
	//PumpHandle->init();
	printf("sensor board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

#if USE_MOTOR_BOARD
	Light_Sensor_Init();
	BSP_Motor_Control_Init();
	
	printf("motor board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

#if 0
//ThermometerHandle->set_degree(375,TMEPERATURE_CURRENT);
printf("zzz1\r\n");
 //test_pump_s100_open();
 //Gradient_control_buffer(1700, 0,0,0,0);
 electromagnetic_control(ELECTROMAGNETIC_B, CLOSE_FT);
 //electromagnetic_control(ELECTROMAGNETIC_C, CLOSE_FT);
 //electromagnetic_control(ELECTROMAGNETIC_B, CLOSE_FT);
  //electromagnetic_control(ELECTROMAGNETIC_C, OPEN_FT);
 electromagnetic_control(ELECTROMAGNETIC_A, OPEN_FT);
 //test_pump_s100_close();
 electromagnetic_control(ELECTROMAGNETIC_C, CLOSE_FT);
 
 electromagnetic_control(DEGASSER_CONTORL, CLOSE_FT);
printf("zzz2\r\n");

#endif


/*
int len=0;
static	char string[128];

u8 key=0;

while(1)
{
	key=KEY_Scan(0);
	if(key)
	{
		switch(key)
		{
			case KEY0_PRES:
					printf("test start .\r\n");
					Init_M6e_Config(TMR_REGION_PRC,2500,3000);

					
					printf("test end .\r\n");
				break;
			case KEY1_PRES:
					printf("zz2 \r\n");
	
					memset(string,0,128);
					Get_EPC_String(&len, string);
					printf("str:%s\r\n",string);
				break;
			case KEY2_PRES:
					printf("zz3 \r\n");
			
				break;
			case WKUP_PRES:
					printf("zz4 \r\n");
					Destory_M6e_Config();
					
				break;
		}	
	}

}
*/

while(1){


#if 1

		if(ARM_RS232_ASK)
		{
						printf("start receive !\r\n");
#if USE_SENSOR_BOARD						
						KeepTemperatureDegree_Duty();
#endif			
						protocol_handle_uart_powerstep01_plain_slave_cmd();
						ARM_RS232_ASK=0;
		}	
		
		delay_ms(10);

		
		
#if USE_SENSOR_BOARD
				i++;
		if(i==100)
		{
			i=0;	
			
			keep_thermometer_degree();
		}		
#endif
	
#endif
		
		//IWDG_Feed();
	}

	return 0;
}
