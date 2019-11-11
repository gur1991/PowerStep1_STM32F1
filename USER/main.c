#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"
#include "light.h"
#include "uart5.h"
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
#include "uart_config.h"
#include "uart_rts_control.h"
#include "real_time_polling.h"



int Check_Board_Define_Config(void)
{
	int value=0;
	value=USE_GRADIENT_CONTROL_BOARD;
	value+=USE_CLEANING_DILUTION_BOARD;
  value+=USE_AUTOMATIC_INJECTION_BOARD;
	value+=USE_KEEP_TEMPERATURE_BOARD;
	if(!value)printf("you have not choose Define Board in [config.h]. \r\n");
	else if(value==1)printf("you have choose Define Board OK in [config.h]. \r\n");
	else printf("you have choose Define Board too many in [config.h]. \r\n");

	return value-1;
}	

void Init_Board_Config(void)
{	
	printf("start Init_Board_Config. \r\n");
//串口2 电磁阀	CS片选信号 硬件流信号
#if USE_GRADIENT_CONTROL_BOARD	
  Uart2_Config_Init();//串口2配置及各串口设备的不同配置
	Uart_Rts_Control_Init;//硬件流控初始化
	Electromagnetic_init();//电磁阀
	printf("gradient board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif


#if USE_CLEANING_DILUTION_BOARD
  Uart2_Config_Init();//串口2配置及各串口设备的不同配置
	Light_Sensor_Init();
	BSP_Motor_Control_Init();
	Electromagnetic_init();//电磁阀
	printf("cleaning dilution board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

#if USE_AUTOMATIC_INJECTION_BOARD
  Uart2_Config_Init();
	Liquid_Sensor_Init();
	Weight_Sensor_Init();
	Light_Sensor_Init();
	BSP_Motor_Control_Init();
	UART5_Init(115200);
	
	printf("init scan. \r\n");
	ScanChooseHandle(FM100);
	ScanHandle->init(true);
	
	printf("init pump. \r\n");
	PumpChooseHandle(S1125);
	PumpHandle->init();
	
	printf("automatic inject board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

//1:加热的定时器方波heating 2：PID算法  3：温度传感器 4:初始化并设置初始温度为0	
//看门狗
#if  USE_KEEP_TEMPERATURE_BOARD
	  
		TIM3_PWM_Init(500-1,72-1);
		Pid_init();
		TIM_SetTIM3Compare4(500);	
		ThermometerChooseHandle(DS18B20);
		ThermometerHandle->init();
		ThermometerHandle->set_degree(0,TMEPERATURE_CURRENT);
	  
	  IWDG_Init(4,625*4); //4s   	MAX
    IWDG_Start();
		printf("keep temperature board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));

#endif

}	



int main(void)
{	
	HAL_Init();                    	 	
  Stm32_Clock_Init(RCC_PLL_MUL9);   	
	delay_init(72);               		
	uart_init(115200);					
	UART4_Init(115200);
	Real_Time_Polling_Init();
	int i=0;
	//KEY_Init(); 
	if(Check_Board_Define_Config())return 0;
	Init_Board_Config();
		
	
	
	while(1)
	{
		if(ARM_RS232_ASK)
		{
						printf("start receive !\r\n");
						protocol_handle_uart_powerstep01_plain_slave_cmd();
						ARM_RS232_ASK=0;
		}	
		delay_ms(10);

		
		
#if USE_KEEP_TEMPERATURE_BOARD
		i++;
		if(i==100)
		{
			i=0;		
			keep_thermometer_degree();
			IWDG_Feed();
		}		
#elif (USE_AUTOMATIC_INJECTION_BOARD|USE_GRADIENT_CONTROL_BOARD)
		i++;
		if(i==50)
		{		
				i=0;
				Real_Time_Polling_Current_Index();
		}
#endif		
	}

	return 0;
}
