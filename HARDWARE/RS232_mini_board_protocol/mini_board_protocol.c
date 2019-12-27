#include "mini_board_protocol.h"
#include "uart5.h"
#include "config.h"

uint8_t Master_Mini_Board_Transfer_Interface(Powerstep1_contorl_motor_command_t* master,int length_data,int* len){
		uint8_t ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD			
		int times =100;
		master->OverReceiveFlag[0]=OVER_UART_VALUE0;
		master->OverReceiveFlag[1]=OVER_UART_VALUE1;
	
		UART5_Send_Data((u8*)(master),length_data);
		while(1){
			if(MINI_RS232_ASK){
					UART5_Receive_Data((u8*)(master),len);
					MINI_RS232_ASK=0;
					//printf("master receive data ok\r\n");
					break;		
			}
			times--;
			if(times ==0){
					ret=1;
					printf("transfer error ! \r\n");
					break;
			}
			delay_ms(100);
			//printf("wait  \r\n");
		}
#endif
		
		return ret;
}


uint8_t Master_Mini_Board_Set_Degree(mini_set_single_temperature_degree_type_t data)
{
		uint8_t ret =0;
	
#if USE_AUTOMATIC_INJECTION_BOARD			
		int len =0;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = MINI_TEMPERATURE_SENSOR_SET_TYPE;
		master_motorCommand.CommandPowerStep1.mini_set_single_temperature_degree=data;
//		printf("start mini_set_single_temperature_degree \r\n");	
		
		ret=Master_Mini_Board_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		//printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.mini_set_single_temperature_degree.response.ret;
				}else{
						ret = 1;//fail
				}
		}
	
//		printf("end mini_set_single_temperature_degree ,ret:%d \r\n",ret);
#endif
		
		return ret;
}

uint8_t Master_Mini_Board_Get_Degree(mini_get_single_temperature_degree_type_t data,int* degree)
{
		uint8_t ret =0;
	
#if USE_AUTOMATIC_INJECTION_BOARD			
		int len =0;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = MINI_TEMPERATURE_SENSOR_GET_TYPE;
		master_motorCommand.CommandPowerStep1.mini_get_single_temperature_degree=data;
//		printf("start mini_get_single_temperature_degree \r\n");	
		
		ret=Master_Mini_Board_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		//printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.mini_get_single_temperature_degree.response.ret;
					 *degree=master_motorCommand.CommandPowerStep1.mini_get_single_temperature_degree.response.degree;
				}else{
						ret = 1;//fail
					 *degree=0;
				}
		}
//		printf("end mini_get_single_temperature_degree ,ret:%d \r\n",ret);
#endif
		
		return ret;
}


