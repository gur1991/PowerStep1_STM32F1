/*
此文件本来是针对PowerStep01调试的串口协议
后来在此基础上拓展成为ARM-STM32 RS232 通用协议

STM32---SLVAE设备的协议和这个不同


*/


#ifndef _UART_COMMAND_CONTROL_H_INCLUDED
#define _UART_COMMAND_CONTROL_H_INCLUDED

#ifdef __cplusplus
 extern "C" {
#endif 
#include "stdint.h"	 
#include "powerstep01.h" 
#include "motor.h"
#include "x_nucleo_ihmxx.h"	 
#include "stdio.h"	 
#include "sys.h"
#include "pump_s100.h"
#include "Cheminert_c52.h"	
#include "light.h"	 
#include "printpaper.h"
#include "weight.h"	 
#include "temperature.h"		 
#include "step01.h"
#include "electromagnetic.h"
//#include "factory_many.h"
#include "execute.h"	
#include "fm100.h"	 
#include "S1125.h"
#include "liquid_sensor.h"
#include "m6e_apply.h"
#include "rs232_change_protocol.h"
#include "real_time_polling.h"

uint8_t motorInit(void);	 
void MyFlagInterruptHandler(void);
void MyBusyInterruptHandler(void);	 
void MyErrorHandler(uint16_t error);	 
	 



#define OVER_UART_VALUE0 0x0d
#define OVER_UART_VALUE1 0x0a

#define START_UART_VALUE0 0xaa
#define START_UART_VALUE1 0xdd

typedef enum Command_type{ 
		ERROR_TYPE=0,
		MOVE_TYPE=1,
		POWER_TYPE,
		REST_TYPE,
		SEND_COMMAND_AND_WAIT_NOTBUSY_TYPE,
		DELAY_TIME_TYPE,
	
		ONE_DEVICE_MOVE_TYPE,
		ONE_DEVICE_WAIT_TYPE,
		ONE_DEVICE_GET_POS_TYPE,
		ONE_DEVICE_SET_MARK_TYPE,
	
		GET_PARAM_TYPE,
		SET_PARAM_TYPE,
		SElECT_STEP_MODE_TYPE,
		INIT_MOTOR_SPEED_TENSION_TYPE,
		MOVE_MANY_MOTOR,
		WAIT_MANY_MOTOR,
		COMMAND_PACKAGE_MOTOR,
	
		GET_LIGHT_LEVEL_TYPE=50,//light sensor level
		GET_ALL_LIGHT_LEVEL_TYPE,
		CHEMINERT_C52_C55_TYPE,//actuator
		PUMP_S100_TYPE,//beng 10ml
	
		//PRINTER_F37C_INFO_TYPE,//打印信息
		//PRINTER_F37C_BMP_TYPE,//打印機圖像
	  //PRINTER_F37C_OUTCOME_TYPE,//打印機結果
		
		WEIGHT_SENSOR_SET_LINE_TYPE,//重力传感器
		WEIGHT_SENSOR_GET_SINGLE_RESULT_TYPE,
		WEIGHT_SENSOR_GET_ALL_RESULT_TYPE,
		
		TEMPERATURE_SENSOR_GET_TYPE,
		TEMPERATURE_SENSOR_SET_TYPE,
		ELECTROMAGNETIC_TYPE,
		
		
		CONNECT_TEST_TYPE,
		GET_WEIGHT_CURRENT_GRAM,
		REST_MOTOR_ORGIN,
		
		MOTOR_MOVE_AND_WAIT,
		SET_PUMPS100_PRESS,
		BALANCE_CHROMATOGRAPHIC_COLUMN,
		GRADIENT_CONTROL_BUFFER,
		GRAVITY_SENSOR_SETTING,
		GRAVITY_SENSOR_GETTING,
		
		SCAN_BARCODE,
		PUMP_INTERFACE,
		REST_INJECTION_MODLUE_MOTOR,
		LIQUID_SENSOR,
		
		PUMPS100_SET_FLOWSPEED,
		
		RFID_INIT,
		RFID_DESTORY,
		RFID_SEND_READ_COMMAND,
		RFID_RECEIVE_INDEX_TAG,
		RFID_GET_EPC_STRING,
		RFID_WRITE_EPC,
		RFID_WRITE_BLANK,
		
		MINI_TEMPERATURE_SENSOR_GET_TYPE,
		MINI_TEMPERATURE_SENSOR_SET_TYPE,
		
		POLLING_DATA,
		POLLING_PRESS,
		
		ELECTROMAGNETIC_PACKAGE_TYPE,

}Command_type_t;


typedef union{ 
	struct{
		uint8_t nor[4];
	}request;
	struct{
		uint8_t ret;
	}response;
}error_type_t;


typedef union{ 
	struct{
		uint32_t up_Steps;
		uint32_t big_Steps;
		int time;
	}request;
	struct{
		uint8_t ret;
	}response;
}rest_injection_module_motor_type_t;

typedef union{ 
	struct{
		uint8_t nor[4];
	}request;
	struct{
		uint8_t ret;
	}response;
}connect_test_type_t;

typedef enum Command_Package_type{ 
			SLEF_TEST=0,//not use
	
			FIRST_START_CHECK_SENSOR_BOARD,//sensor
			REST_SENSOR_BOARD_MOTOR,//sensor
			FIRST_START_CHECK_MOTOR_BOARD,//motor
			REST_MOTOR_BOARD_MOTOR,//motor

			REST_C55_C52,//motor
			REST_DRAIN_AND_WASH_MOTOR,//motor
			REST_SAMPLE_MOTOR,//motor

			CLEAR_BLANK,//sensor
			CLEARL_WAIT,//sensor
			READY_NEXT,//sensor
			READY_LEFT,//sensor
			LEFT_MOVE_TO_WAIT,//sensor
	

			REST_HIGH_WHEEL,//sensor
			REST_TRANSPORTER_BELT,//sensor

			REACH_DEGREE_WAIT,//sensor
			MIX_SCAN_SLOW,//sensor
			MIX_BLOOD_HIGH,//sensor

			BELT_MOVE_SAMETIME,//sensor
			MIX_WORK_GOTO,//sensor
			NORMAL_PITCH_MOVE_NEXT,//sensor
			NORMAL_BLANK_REST,//sensor
			NORMAL_FIRST_POSITON,//sensor
			MIX_AND_REACH_POSITION,//sensor
			BIG_IN_OUT_SLOW,//sensor
			BIG_IN_OUT_NORMAL,//sensor
			
			NORMAL_PITCH_MOVE_NEXT_THE_LAST_TWO,//sensor
			
			C55_C52_CONNECT,//motor


}Command_Package_t;

typedef union{ 
	struct{
		uint8_t command;
	}request;
	struct{
		uint8_t value;
		uint8_t ret;
		uint8_t nor[2];
	}response;
}motor_command_package_type_t;


typedef union{ 
	struct{
		int32_t steps;//+forward -backward
		uint8_t devices;//0,1,2
		uint8_t types;//gpowerstep01_Commands_t
		uint8_t nor[2];
	}request;
	struct{
		uint8_t ret;
	}response;
}move_type_t;

//单个电机强制停止----3
typedef union{ 
	struct{
		uint8_t devices;//1,2  0xff stop all
		uint8_t power;//废弃
		uint8_t nor[2];
	}request;
	struct{
		uint8_t ret;
	}response;
}power_type_t;

typedef union{ 
	struct{
		uint8_t devices;//0,1,2
		uint8_t nor[3];
	}request;
	struct{
		uint8_t ret;
	}response;
}rest_pos_type_t;

typedef union{ 
	struct{
		uint8_t nor[4];//保留位
	}request;
	struct{
		uint8_t ret;
	}response;
}	send_command_and_wait_no_busy_type_t;

typedef union{
struct{
		uint16_t time_ms;
	  uint8_t nor[2];
}request;
struct{
		uint8_t ret;
}response;
}delay_time_type_t;


//单个电机移动-----1
typedef union{ 
struct{
		uint8_t devices;//1,2
		uint8_t dir;
		uint32_t steps;//+forward -backward
		uint8_t  nor[2];
}request;
struct{
		uint8_t ret;
}response;
}one_device_move_type_t;

//单个电机等待停止----2
typedef union{
struct{
		uint8_t devices;//0,1,2
		uint8_t nor[3];
}request;
struct{
		uint8_t ret;
}response;
}one_device_wait_type_t;

typedef union{ 
	struct{
		uint8_t devices;//0,1,2
}request;
struct{
		int32_t pos;
		uint8_t ret;
		uint8_t nor[3]; 
}response;
}one_device_get_pos_type_t;

typedef union{
struct{
		uint8_t devices;//0,1,2
		int32_t pos;
	  uint8_t nor[3]; 
}request;
struct{
		uint8_t ret;
}response;
}one_device_set_mark_type_t;

typedef union{

//获得参数----4
struct{
		uint8_t devices;//1,2
		uint8_t registe;
}request;
struct{
		uint32_t result_para;
		uint8_t ret;
		uint8_t nor[3];
}response;
}get_para_type_t;
//设置参数----5
typedef union{
struct{
		uint8_t devices;//1,2
		uint8_t registe;
		uint32_t para;
	  uint8_t nor[2];
}request;
struct{
		uint8_t ret;
}response;
}set_para_type_t;

typedef union{ 
struct{
		uint8_t devices;//1,2
		uint8_t StepMode;
	  uint8_t nor[2];
}request;
struct{
		uint8_t ret;
}response;
}select_step_mode_t;

typedef struct{
		u8 StartReceiveFlag[2];
		uint32_t type;//Command_type_t type;
		union {
				error_type_t error;
				move_type_t move;
				power_type_t power;
				rest_pos_type_t rest_post;
				send_command_and_wait_no_busy_type_t send_and_wait;
				delay_time_type_t delay;
				one_device_move_type_t one_device_move;
				one_device_wait_type_t one_device_wait;
				one_device_get_pos_type_t  one_device_get_pos;
				one_device_set_mark_type_t one_device_set_mark;
				get_para_type_t  get_para;
				set_para_type_t  set_para;
				select_step_mode_t  select_step_mode;
				init_motor_speed_tension_type_t init_motor_speed_tension;
				move_many_motor_type_t move_many_motor;
				wait_many_motor_type_t wait_many_motor;
				motor_command_package_type_t motor_command_package;
			
				get_light_sensor_level_t get_light_sensor_level;
				get_all_light_sensor_level_t get_all_light_sensor_level;
				
				cheminert_c52_c55_type_t cheminert_c52_c55;
				
				pump_s100_command_type_t pump_s100_command;


				set_weight_warning_line_type_t	set_weight_warning_line;
				get_single_weight_warning_result_type_t get_single_weight_warning_result;
				get_all_weight_warning_result_type_t get_all_weight_warning_result;
				
				set_single_temperature_degree_type_t set_single_temperature_degree;
				get_single_temperature_degree_type_t get_single_temperature_degree;
				
				electromagnetic_type_t electromagnetic;
				connect_test_type_t connect_test;
				
				get_weight_current_gram_type_t get_weight_current_gram;
				rest_select_motor_orgin_type_t rest_select_motor_orgin;
				
				
				move_wait_motor_type_t move_wait_motor;
				Set_Pumps100_Press_type_t Set_Pumps100_Press;
				Balance_Chromatographic_Column_type_t Balance_Chromatographic_Column;
				Gradient_control_buffer_type_t Gradient_control_buffer;
				Gravity_Sensor_Setting_type_t Gravity_Sensor_Setting;
				Gravity_Sensor_Getting_type_t Gravity_Sensor_Getting;
				
				scan_barcode_t scan;
				
				pump_type_t pump; 
				rest_injection_module_motor_type_t rest_injection_module_motor;
				get_liquid_sensor_level_t get_liquid_sensor;
				
				pump_s100_set_flowSpeed_type_t pump_s100_set_flowSpeed;
				mix_and_reach_type_t mix_and_reach;
				
				rfid_init_config_type_t rfid_init_config;
				rfid_destory_config_type_t rfid_destory_config;
				rfid_send_read_command_type_t rfid_send_read_command;
				rfid_receive_index_tag_type_t rfid_receive_index_tag;
				rfid_get_epc_string_type_t rfid_get_epc_string;
				rfid_write_epc_type_t rfid_write_epc;
				rfid_write_blank_type_t rfid_write_blank;
				
				mini_set_single_temperature_degree_type_t mini_set_single_temperature_degree;
				mini_get_single_temperature_degree_type_t mini_get_single_temperature_degree;
				
				Real_Time_Polling_t Real_Time_Polling;
				Real_Time_Polling_Press_t Real_Time_Polling_Press;
				
				electromagnetic_package_type_t electromagnetic_package;

/*
*print move rk3188
*/
/*
				print_info_type_t print_info;
				print_bmp_type_t print_bmp; 
				print_outcome_type_t  print_outcome;
*/			
 		}CommandPowerStep1;
		u8 OverReceiveFlag[2];
}Powerstep1_contorl_motor_command_t;


	 
#endif 
