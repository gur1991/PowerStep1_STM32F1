#ifndef __CONFIG_H
#define __CONFIG_H
#include <stdio.h>



//新的结构设计定义
#define USE_NEW_DESIGN 1
//new start for dev branch

#define USE_GRADIENT_CONTROL_BOARD 0//Hummingbird--梯度控制，输液泵，电磁阀ABC，脱气机，薄而贴，风扇

#define USE_CLEANING_DILUTION_BOARD 0//Pecker----C55 C52 两个注射器  控制上下和控制远近的电机

#define USE_AUTOMATIC_INJECTION_BOARD 1//Camel---其他温度重力等模块    旋转混匀 传送带 扫码器 RFID

#define USE_KEEP_TEMPERATURE_BOARD 0// mini control temperature




#define _SHARK_HEADER_ "VG1-"
#define _SHARK_VERSION_ "-3.1.200924.D"

#if USE_GRADIENT_CONTROL_BOARD
	#define _STM_BOARD_  "SH"
#endif
#if USE_CLEANING_DILUTION_BOARD
	#define _STM_BOARD_  "SP"
#endif
#if USE_AUTOMATIC_INJECTION_BOARD
	#define _STM_BOARD_  "SC"
#endif
#if USE_KEEP_TEMPERATURE_BOARD
	#define _STM_BOARD_  "ST"
#endif

//#define STM_VERSION #_SHARK_HEADER_###_STM_BOARD_###_SHARK_VERSION_


#define LOGD(format,...) do { printf("[%s:%d#]" format,__func__,__LINE__,##__VA_ARGS__); }while(0)
#define LOGE(format,...) do { printf("[ERROR %s:%d#]" format,__func__,__LINE__,##__VA_ARGS__); }while(0)
				
/*****************************************************/
//UART4 ----- ARM 

#define COUNT_TIME (10*1000-1) //4s

#define  C52_UART_PORT UART2_RS232
#define  C52_UART_CS  CS_ONE  

#define  C55_UART_PORT UART2_RS232
#define  C55_UART_CS CS_ZERO


#define  FM100_UART_PORT UART2_RS232
#define  FM100_UART_CS CS_ZERO

#define  BL180_UART_PORT UART2_RS232
#define  BL180_UART_CS CS_ZERO


#define  PUMP_UART_PORT UART2_RS232
#define  PUMP_UART_CS CS_THREE

#define  RFID_UART_PORT UART2_RS232
#define  RFID_UART_CS CS_ONE


/*
#define CLK_COUNT 50
#define DUTY_CYCLE 0.25

#define HIGH_TIME 40
#define HIGH_LOW_TIME 2000
#define LOW_TIME 1000

	//HIGH
	TIM_SetTIM3Compare4(0);
	delay_ms(HIGH_TIME);
	
	//PWM
	TIM_SetTIM3Compare4((int)(CLK_COUNT*(1-DUTY_CYCLE)));
	delay_ms(HIGH_LOW_TIME);
	
	//LOW
	TIM_SetTIM3Compare4(CLK_COUNT);
	delay_ms(LOW_TIME);
*/

















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
#endif
