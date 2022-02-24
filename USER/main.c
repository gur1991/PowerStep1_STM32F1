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
#include "check_rs232.h"
#include "export_liquid.h"
#include "drv8434.h"


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
	
#ifdef USE_DRV8434_PECKER
	DRV8434_MOTOR_Config_Init();
#else	
	BSP_Motor_Control_Init();//没设备连接会卡住
#endif	
	
	Electromagnetic_init();//电磁阀
	LOGD("[Pecker],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

#if USE_AUTOMATIC_INJECTION_BOARD
  Uart2_Config_Init();
	Export_Liquid_Control_Init();
	Liquid_Sensor_Init();
	
	Weight_Sensor_Init();
	Light_Sensor_Init();
	UART5_Init(115200);
	
	LOGD("init scan. \r\n");
	ScanChooseHandle(FM100);
	ScanHandle->init(true);
	
	//Init_M6e_Config(TMR_REGION_PRC,0,0);
#ifdef USE_DRV8434_CAMEL
	DRV8434_MOTOR_Config_Init();
#else	
	BSP_Motor_Control_Init();//没设备连接会卡住
#endif	

	LOGD("[Camel],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

//1:加热的定时器方波heating 2：PID算法  3：温度传感器 4:初始化并设置初始温度为0	
//看门狗
#if  USE_KEEP_TEMPERATURE_BOARD
	  
		TIM3_PWM_Init(500-1,72-1);
		Pid_init();

#ifdef USE_NEW_PID_CONTROL_HEATER		
		New_Pid_Setup();
#endif

		TIM_SetTIM3Compare4(500);	
		ThermometerChooseHandle(DS18B20);
		ThermometerHandle->init();
		ThermometerHandle->set_degree(0,TMEPERATURE_CURRENT);
	  //ThermometerHandle->set_degree(380,TMEPERATURE_CURRENT);
		
	  IWDG_Init(4,625*4); //4s   	MAX
    IWDG_Start();
		LOGD("[Mini],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));

#endif

		
	  __HAL_AFIO_REMAP_SWJ_NOJTAG();
}	
void __ack_error_msg__(void)
{	
	  check_bit_type_t check;
		Powerstep1_contorl_motor_command_t slave_sendCommand;
		memset(&slave_sendCommand,0,sizeof(Powerstep1_contorl_motor_command_t));
	
		slave_sendCommand.type=ERROR_TYPE;
		slave_sendCommand.CommandPowerStep1.error.response.ret=1;	
	
		Uart_Clear_Context();
		slave_sendCommand.StartReceiveFlag[0]=START_UART_VALUE0;
		slave_sendCommand.StartReceiveFlag[1]=START_UART_VALUE1;
		slave_sendCommand.OverReceiveFlag[0]=OVER_UART_VALUE0;
		slave_sendCommand.OverReceiveFlag[1]=OVER_UART_VALUE1;
		check=caculate_tansfer_check_bit(slave_sendCommand);	//不对flag进行校验
		slave_sendCommand.CheckBit[0]=check.H;
		slave_sendCommand.CheckBit[1]=check.L;
		UART4_Send_Data((u8*)(&slave_sendCommand),sizeof(Powerstep1_contorl_motor_command_t));
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
	int temp_a=0,temp_b=0;
	//KEY_Init(); 
	LOGD("STM_VERSION:%s%s%s\r\n",_SHARK_HEADER_,_STM_BOARD_,_SHARK_VERSION_);
	if(Check_Board_Define_Config())return 0;
	Init_Board_Config();
	
//RestFarAndDownMotorOrgin();
	
	//Rest_high_wheel();
	//March_high_wheel();
	//Rest_high_wheel();
	//Belt_Move_At_SameTime();
/*	
	
	RestSelectMotorOrgin(M4_BLANK_NEXT, M4_LIGHT, M4_BLANK_TO_NEXT, 60000);
	Normal_Goto_First_Position();
		
		int k=0;
    for(k=0;k<10;k++){
				Normal_Pitch_Move_Next();
				delay_ms(500);	
		}
		Normal_Pitch_Move_Next_The_Last_Two();
		Normal_Pitch_Move_Next_The_Last_Two();
	RestSelectMotorOrgin(M4_BLANK_NEXT, M4_LIGHT, M4_BLANK_TO_NEXT, 60000);
*/	
	
	//Mix_Blood_High_Speed();
	//delay_ms(5000);
	//DRV8434_Motor_HardStop_And_Goto_Sleep(M2_MIX);
	//RestSelectMotorOrgin(M1_MIX_V, M1_LIGHT, M1_MIX_V_UP, 60000);
	
	
	
	//DRV8434_Motor_Move_And_Wait(M1_MIX_V, M8_BIG_IN, 10000);
	
	
	

//	Weight_Sensor_Init();
/*
	while(1)
	{
		LOGD("W1:%d \r\n",get_weight_sensor_value(WEIGHT_ONE));
		LOGD("W2:%d \r\n",get_weight_sensor_value(WEIGHT_TWO));
		LOGD("W3:%d \r\n",get_weight_sensor_value(WEIGHT_THREE));
		LOGD("W4:%d \r\n",get_weight_sensor_value(WEIGHT_FOUR));
		delay_ms(500);
	}	
*/	
	//RestFarAndDownMotorOrgin();	
		
	//First_Open_Motor_AutoCheck_Sensor();
	//March_Drain_And_Wash_Motor_Orgin();
	//Rest_Drain_And_Wash_Motor_Orgin();
		//First_Open_Motor_AutoCheck_Motor();
		//Rest_Injection_Module_Motor(10000,10000,200);
		
		//RestFarAndDownMotorOrgin();

//	Mix_Work_Goto_Postion();
	
	//Scan_Motor_Slow_Spin();
	//delay_ms(3000);
	//DRV8434_Motor_HardStop_And_Goto_Sleep(M2_MIX);
	//DRV8434_Motor_Move_Steps_Enable_Acc(M2_MIX);
	//Mix_Blood_High_Speed();	
	//RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 600*1000);
	//Mix_Work_Goto_Postion();
	//RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 600*1000);
	
	/*
		RestSelectMotorOrgin(M3_LEFT_WAIT,M3_LIGHT, M3_WAIT_TO_LEFT,92000);
		LeftMoveTowardWaitPosition();
		RestSelectMotorOrgin(M3_LEFT_WAIT,M3_LIGHT, M3_WAIT_TO_LEFT,92000);
		return 0;
	
		RestSelectMotorOrgin(M4_BLANK_NEXT, M4_LIGHT, M4_BLANK_TO_NEXT, 32000);
		Normal_Goto_First_Position();
		
		int k=0;
    for(k=0;k<10;k++){
				Normal_Pitch_Move_Next();
				delay_ms(500);	
		}
		Normal_Pitch_Move_Next_The_Last_Two();
		Normal_Pitch_Move_Next_The_Last_Two();
*/		
		//Normal_Move_Blank();
	
		//NORMAL_SPEED  HIGH_SPEED  LOW_SPEED
		//DRV8434_Motor_Select_Speed(M2_MIX, NORMAL_SPEED);
		//RestSelectMotorOrgin(M1_MIX_V, M1_LIGHT_WORK, M1_MIX_V_DOWN, 60000);
		//RestSelectMotorOrgin(M1_MIX_V, M1_LIGHT, M1_MIX_V_UP, 60000);
		//DRV8434_Motor_Move_And_Wait(M2_MIX, M2_MIX_LEFT, 60000);
		//DRV8434_Motor_Move_And_Wait(M4_BLANK_NEXT, M5_WAIT_TO_NEXT, 60000);		
		
		//DRV8434_Motor_Select_Speed(M1_MIX_V, NORMAL_SPEED);
		//RestSelectMotorOrgin(M1_MIX_V, M1_LIGHT, M1_MIX_V_UP, 60000);
		
	 //RestSelectMotorOrgin(M8_BIG_IN_OUT, M8_LIGHT, M8_BIG_OUT, 60000);
	 //DRV8434_Motor_Move_And_Wait(M8_BIG_IN_OUT, M8_BIG_IN, 10000);
	 
	
	//DRV8434_Motor_Move_Steps(M1_MIX_V, M1_MIX_V_UP, 40000);
	//delay_ms(100);
	//DRV8434_Motor_HardStop_And_Goto_Sleep(M1_MIX_V);
	
	
	//RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 30000);
	
	//delay_ms(1000);
	//DRV8434_Motor_Move_And_Wait(M1_MIX_V, M1_MIX_V_DOWN, 40000);
	//delay_ms(1000);
	//RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 60000);
	
	
	//DRV8434_MOTOR_Config_Init();
	//DRV8434_Motor_Move_And_Wait(3, M8_BIG_IN, 200000);
	
	
	//DRV8434_Motor_Move_And_Wait(3, M4_BLANK_TO_NEXT, 40000);
	
	//RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 60*10000);
	
	
	#if USE_KEEP_TEMPERATURE_BOARD
  delay_ms(1);
  #else	
	delay_ms(1000*8);
	#endif
	
	Uart4_Rx_Clear();
	UART4_Enable_Interrupt();
	
	while(1)
	{
			temp_a=UART4_RX_CNT;

#if USE_KEEP_TEMPERATURE_BOARD		
			delay_ms(1);
#elif USE_AUTOMATIC_INJECTION_BOARD		
		  delay_ms(1);
#else
			delay_ms(1);
#endif	
		
			temp_b=UART4_RX_CNT;
			
			if(temp_b!=0)
			{		
					if((temp_a==temp_b) && (temp_b!=sizeof(Powerstep1_contorl_motor_command_t)))
					{
							LOGE("rx len is error [%d] \r\n",temp_a);
							UART4_RX_CNT=0;
#if (USE_AUTOMATIC_INJECTION_BOARD||USE_GRADIENT_CONTROL_BOARD||USE_CLEANING_DILUTION_BOARD)						
							__ack_error_msg__();
#endif						
					}
					
					if((temp_a==temp_b) && (temp_b==sizeof(Powerstep1_contorl_motor_command_t)))
					{
							UART4_Disable_Interrupt();
							protocol_handle_uart_powerstep01_plain_slave_cmd();
						  UART4_Enable_Interrupt();
							UART4_RX_CNT=0;
					}	
			}
			
#if USE_KEEP_TEMPERATURE_BOARD
			i++;
#ifdef USE_NEW_PID_CONTROL_HEATER			
			if(i==50)
#else
			if(i==1000)
#endif			
			{
				i=0;		
				keep_thermometer_degree();
				IWDG_Feed();
			}
#endif		


			
	}

	return 0;
}
