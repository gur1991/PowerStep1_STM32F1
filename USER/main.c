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
#include "uart_choose.h"


int Check_Board_Define_Config(void)
{
	int value=0;
	value=USE_GRADIENT_CONTROL_BOARD;
	value+=USE_CLEANING_DILUTION_BOARD;
  value+=USE_AUTOMATIC_INJECTION_BOARD;
	value+=USE_KEEP_TEMPERATURE_BOARD;
	if(!value)LOGD("you have not choose Define Board in [config.h]. \r\n");
	else if(value==1)LOGD("you have choose Define Board OK in [config.h]. \r\n");
	else LOGD("you have choose Define Board too many in [config.h]. \r\n");

	return value-1;
}	

void Init_Board_Config(void)
{	
	LOGD("start Init_Board_Config. \r\n");
	//串口2 电磁阀	CS片选信号 硬件流信号
#if USE_GRADIENT_CONTROL_BOARD	
  Uart2_Config_Init();//串口2配置及各串口设备的不同配置
	Uart_Rts_Control_Init();//硬件流控初始化
	Electromagnetic_init();//电磁阀
	
	LOGD("init pump. \r\n");
	PumpChooseHandle(S1125);
	PumpHandle->init();
	
	LOGD("[Hummingbird],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif


#if USE_CLEANING_DILUTION_BOARD
  Uart2_Config_Init();//串口2配置及各串口设备的不同配置
	Light_Sensor_Init();
	BSP_Motor_Control_Init();//没设备连接会卡住
	Electromagnetic_init();//电磁阀
	LOGD("[Pecker],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

#if USE_AUTOMATIC_INJECTION_BOARD
  Uart2_Config_Init();
	
	Liquid_Sensor_Init();
	Weight_Sensor_Init();
	Light_Sensor_Init();
	BSP_Motor_Control_Init();//没设备连接会卡住
	UART5_Init(115200);
	
	LOGD("init scan. \r\n");
	ScanChooseHandle(FM100);
	ScanHandle->init(true);
	
	//Init_M6e_Config(TMR_REGION_PRC,0,0);
	LOGD("[Camel],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
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
		LOGD("[Mini],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));

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
	LOGD("STM_VERSION:%s%s%s\r\n",_SHARK_HEADER_,_STM_BOARD_,_SHARK_VERSION_);
	if(Check_Board_Define_Config())return 0;
	Init_Board_Config();
	
	
	//Normal_Goto_First_Position();
#if 0	
	while(1)
	{	
		delay_ms(100);	
		LOGD("light[9][10][11]:%d  %d  %d\r\n",Light_Sensor_Get(9),Light_Sensor_Get(10),Light_Sensor_Get(11));
		//LOGD("light[9][10][11]:%d  %d  %d\r\n",__Light_Sensor_Get__(9),__Light_Sensor_Get__(10),__Light_Sensor_Get__(11));
	}
#endif
	
	
/*
	Init_M6e_Config(TMR_REGION_PRC,2500,3000,ONLY_SECOND);
	
	uint8_t length=0;
	uint8_t epc[100];
	memset(epc,0,sizeof(epc));
	
	uint32_t ret = Get_EPC_String(&length, epc);
	LOGD("ret:0x%x\r\n",ret);
	LOGD("len:%d EPC:%s\r\n",length,epc);
	
	
	uint8_t len=0;
	ret= M6e_Magic_Read_Rfid_Info(&len);
	LOGD("ret:0x%x\r\n",ret);
	int index=0;
	Chemical_reagent_Info_type info;
	
	for(index=0;index<len;index++)
	{
		info=M6e_Magic_Get_One_Rfid_Info(index);
		Protocol_Output_Report(info);
	}
	LOGD("结束! \r\n");
	
	return 0;

*/


	//Motor_Move_And_Wait(M1_MIX_V, M1_MIX_V_DOWN, 400000);
	
	
	
	
	
	while(1)
	{
			if(ARM_RS232_ASK)
			{
							protocol_handle_uart_powerstep01_plain_slave_cmd();
							ARM_RS232_ASK=0;
			}	
			delay_ms(1);			
			
#if USE_KEEP_TEMPERATURE_BOARD
			i++;
			if(i==1000)
			{
				i=0;		
				keep_thermometer_degree();
				IWDG_Feed();
			}
#endif		

#if (USE_AUTOMATIC_INJECTION_BOARD||USE_GRADIENT_CONTROL_BOARD)
			i++;
			if(i==1000)
			{		
					i=0;
					Real_Time_Polling_Current_Index();
					
			}
#endif  
			
	}

	return 0;
}
