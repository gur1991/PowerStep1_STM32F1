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
//����2 ��ŷ�	CSƬѡ�ź� Ӳ�����ź�
#if USE_GRADIENT_CONTROL_BOARD	
  Uart2_Config_Init();//����2���ü��������豸�Ĳ�ͬ����
	Uart_Rts_Control_Init();//Ӳ�����س�ʼ��
	Electromagnetic_init();//��ŷ�
	
	printf("init pump. \r\n");
	PumpChooseHandle(S1125);
	PumpHandle->init();
	
	printf("[Hummingbird],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif


#if USE_CLEANING_DILUTION_BOARD
  Uart2_Config_Init();//����2���ü��������豸�Ĳ�ͬ����
	Light_Sensor_Init();
	BSP_Motor_Control_Init();//û�豸���ӻῨס
	Electromagnetic_init();//��ŷ�
	printf("[Pecker],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

#if USE_AUTOMATIC_INJECTION_BOARD
  Uart2_Config_Init();
	Liquid_Sensor_Init();
	Weight_Sensor_Init();
	Light_Sensor_Init();
	BSP_Motor_Control_Init();//û�豸���ӻῨס
	UART5_Init(115200);
	
	printf("init scan. \r\n");
	ScanChooseHandle(FM100);
	ScanHandle->init(true);
	//Init_M6e_Config(TMR_REGION_PRC,0,0);
	printf("[Camel],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

//1:���ȵĶ�ʱ������heating 2��PID�㷨  3���¶ȴ����� 4:��ʼ�������ó�ʼ�¶�Ϊ0	
//���Ź�
#if  USE_KEEP_TEMPERATURE_BOARD
	  
		TIM3_PWM_Init(500-1,72-1);
		Pid_init();
		TIM_SetTIM3Compare4(500);	
		ThermometerChooseHandle(DS18B20);
		ThermometerHandle->init();
		ThermometerHandle->set_degree(0,TMEPERATURE_CURRENT);
	  
	  IWDG_Init(4,625*4); //4s   	MAX
    IWDG_Start();
		printf("[Mini],protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));

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


#if 0	
	Powerstep1_contorl_motor_command_t command;
	u8 stringCommand[100];
	memset(stringCommand, 0, sizeof(stringCommand));
	
	command.StartReceiveFlag[0]=START_UART_VALUE0;
	command.StartReceiveFlag[1]=START_UART_VALUE1;
	command.OverReceiveFlag[0]=OVER_UART_VALUE0;
	command.OverReceiveFlag[1]=OVER_UART_VALUE1;
	
	command.type=ERROR_TYPE;
	command.CommandPowerStep1.error.response.ret=1;	
	memcpy(stringCommand, &command, sizeof(Powerstep1_contorl_motor_command_t));
	

	{
		printf("xx 0x%x \r\n",stringCommand[0]);
		printf("xx 0x%x \r\n",stringCommand[1]);
		printf("xx 0x%x \r\n",stringCommand[2]);
		
		printf("error_type_t %d \r\n",sizeof(error_type_t));
		printf("move_type_t %d \r\n",sizeof(move_type_t));
		printf("power_type_t %d \r\n",sizeof(power_type_t));
		printf("rest_pos_type_t %d \r\n",sizeof(rest_pos_type_t));
		printf("send_command_and_wait_no_busy_type_t %d \r\n",sizeof(send_command_and_wait_no_busy_type_t));
		
		printf("delay_time_type_t %d \r\n",sizeof(delay_time_type_t));
		printf("one_device_move_type_t %d \r\n",sizeof(one_device_move_type_t));
		printf("one_device_wait_type_t %d \r\n",sizeof(one_device_wait_type_t));
		printf("one_device_get_pos_type_t %d \r\n",sizeof(one_device_get_pos_type_t));
		printf("one_device_set_mark_type_t %d \r\n",sizeof(one_device_set_mark_type_t));
		
		printf("get_para_type_t %d \r\n",sizeof(get_para_type_t));
		printf("set_para_type_t %d \r\n",sizeof(set_para_type_t));
		printf("select_step_mode_t %d \r\n",sizeof(select_step_mode_t));
		printf("init_motor_speed_tension_type_t %d \r\n",sizeof(init_motor_speed_tension_type_t));
		printf("move_many_motor_type_t %d \r\n",sizeof(move_many_motor_type_t));
		
		printf("wait_many_motor_type_t %d \r\n",sizeof(wait_many_motor_type_t));
		printf("motor_command_package_type_t %d \r\n",sizeof(motor_command_package_type_t));
		printf("get_light_sensor_level_t %d \r\n",sizeof(get_light_sensor_level_t));
		printf("get_all_light_sensor_level_t %d \r\n",sizeof(get_all_light_sensor_level_t));
		printf("cheminert_c52_c55_type_t %d \r\n",sizeof(cheminert_c52_c55_type_t));
		
		printf("pump_s100_command_type_t %d \r\n",sizeof(pump_s100_command_type_t));
		printf("set_weight_warning_line_type_t %d \r\n",sizeof(set_weight_warning_line_type_t));
		printf("get_single_weight_warning_result_type_t %d \r\n",sizeof(get_single_weight_warning_result_type_t));
		printf("get_all_weight_warning_result_type_t %d \r\n",sizeof(get_all_weight_warning_result_type_t));
		printf("set_single_temperature_degree_type_t %d \r\n",sizeof(set_single_temperature_degree_type_t));
	
		printf("get_single_temperature_degree_type_t %d \r\n",sizeof(get_single_temperature_degree_type_t));
		printf("electromagnetic_type_t %d \r\n",sizeof(electromagnetic_type_t));
		printf("get_weight_current_gram_type_t %d \r\n",sizeof(get_weight_current_gram_type_t));
		printf("get_all_weight_warning_result_type_t %d \r\n",sizeof(get_all_weight_warning_result_type_t));
		printf("rest_select_motor_orgin_type_t %d \r\n",sizeof(rest_select_motor_orgin_type_t));
	
		printf("move_wait_motor_type_t %d \r\n",sizeof(move_wait_motor_type_t));
		printf("Set_Pumps100_Press_type_t %d \r\n",sizeof(Set_Pumps100_Press_type_t));
		printf("Balance_Chromatographic_Column_type_t %d \r\n",sizeof(Balance_Chromatographic_Column_type_t));
		printf("Gradient_control_buffer_type_t %d \r\n",sizeof(Gradient_control_buffer_type_t));
		printf("Gravity_Sensor_Setting_type_t %d \r\n",sizeof(Gravity_Sensor_Setting_type_t));
	
		printf("Gravity_Sensor_Getting_type_t %d \r\n",sizeof(Gravity_Sensor_Getting_type_t));
		printf("scan_barcode_t %d \r\n",sizeof(scan_barcode_t));
		printf("pump_type_t %d \r\n",sizeof(pump_type_t));
		printf("rest_injection_module_motor_type_t %d \r\n",sizeof(rest_injection_module_motor_type_t));
		printf("get_liquid_sensor_level_t %d \r\n",sizeof(get_liquid_sensor_level_t));
	
		printf("pump_s100_set_flowSpeed_type_t %d \r\n",sizeof(pump_s100_set_flowSpeed_type_t));
		printf("mix_and_reach_type_t %d \r\n",sizeof(mix_and_reach_type_t));
		printf("rfid_init_config_type_t %d \r\n",sizeof(rfid_init_config_type_t));
		printf("rfid_destory_config_type_t %d \r\n",sizeof(rfid_destory_config_type_t));
		printf("rfid_send_read_command_type_t %d \r\n",sizeof(rfid_send_read_command_type_t));
	
		printf("rfid_receive_index_tag_type_t %d \r\n",sizeof(rfid_receive_index_tag_type_t));
		printf("rfid_get_epc_string_type_t %d \r\n",sizeof(rfid_get_epc_string_type_t));
		printf("rfid_write_epc_type_t %d \r\n",sizeof(rfid_write_epc_type_t));
		printf("rfid_write_blank_type_t %d \r\n",sizeof(rfid_write_blank_type_t));
		printf("mini_set_single_temperature_degree_type_t %d \r\n",sizeof(mini_set_single_temperature_degree_type_t));
		
		printf("mini_get_single_temperature_degree_type_t %d \r\n",sizeof(mini_get_single_temperature_degree_type_t));
		printf("Real_Time_Polling_t %d \r\n",sizeof(Real_Time_Polling_t));
		printf("Real_Time_Polling_Press_t %d \r\n",sizeof(Real_Time_Polling_Press_t));
		printf("electromagnetic_package_type_t %d \r\n",sizeof(electromagnetic_package_type_t));
	
	}
#endif
	
	while(1)
	{
			if(ARM_RS232_ASK)
			{
							//printf("start receive !\r\n");
							protocol_handle_uart_powerstep01_plain_slave_cmd();
							ARM_RS232_ASK=0;
			}	
			delay_ms(20);			
			
#if USE_KEEP_TEMPERATURE_BOARD
			i++;
			if(i==50)
			{
				i=0;		
				keep_thermometer_degree();
				IWDG_Feed();
			}
#endif		

#if 0//(USE_AUTOMATIC_INJECTION_BOARD|USE_GRADIENT_CONTROL_BOARD)
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
