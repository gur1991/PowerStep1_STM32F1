#include "mini_board_protocol.h"
#include "uart5.h"
#include "config.h"

uint8_t Master_Mini_Board_Transfer_Interface(Powerstep1_contorl_motor_command_t* master,int length_data,int* len){
		uint8_t ret=0;
	  check_bit_type_t check;
	  int temp_a=0,temp_b=0;
#if USE_AUTOMATIC_INJECTION_BOARD			
		int times =100;
	  check=caculate_tansfer_check_bit(*master);
		master->OverReceiveFlag[0]=OVER_UART_VALUE0;
		master->OverReceiveFlag[1]=OVER_UART_VALUE1;
		master->StartReceiveFlag[0]=START_UART_VALUE0;
	  master->StartReceiveFlag[1]=START_UART_VALUE1;
		master->CheckBit[0]=check.H;
	  master->CheckBit[1]=check.L;
		UART5_Enable_Interrupt();
	  UART5_RX_CNT=0;
		UART5_Send_Data((u8*)(master),length_data);
		while(1)
		{
			temp_a=UART5_RX_CNT;
			delay_ms(1);	
			temp_b=UART5_RX_CNT;
			
			if(temp_b!=0)
			{		
					if((temp_a==temp_b) && (temp_b!=sizeof(Powerstep1_contorl_motor_command_t)))
					{
						LOGE("rx len is error \r\n");
						UART5_RX_CNT=0;
						ret=1;
						break;
					}
					if((temp_a==temp_b) && (temp_b==sizeof(Powerstep1_contorl_motor_command_t)))
					{
							UART5_Receive_Data((u8*)(master),len);
							UART5_RX_CNT=0;
						  break;
					}	
			}

			//times--;
			if(times ==0)
			{
					ret=1;
					LOGE("transfer error ! \r\n");
					break;
			}
		}
		UART5_Disable_Interrupt();
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
//		LOGD("start mini_set_single_temperature_degree \r\n");	
		
		ret=Master_Mini_Board_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		//LOGD("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.mini_set_single_temperature_degree.response.ret;
				}else{
						ret = 1;//fail
				}
		}
	
//		LOGD("end mini_set_single_temperature_degree ,ret:%d \r\n",ret);
#endif
		
		return ret;
}

uint8_t Master_Mini_Board_Get_Degree(mini_get_single_temperature_degree_type_t data,uint32_t* degree,uint8_t* status)
{
		uint8_t ret =0;
	
#if USE_AUTOMATIC_INJECTION_BOARD			
		int len =0;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = MINI_TEMPERATURE_SENSOR_GET_TYPE;
		master_motorCommand.CommandPowerStep1.mini_get_single_temperature_degree=data;
//		LOGD("start mini_get_single_temperature_degree \r\n");	
		
		ret=Master_Mini_Board_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		//LOGD("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.mini_get_single_temperature_degree.response.ret;
					 *degree=master_motorCommand.CommandPowerStep1.mini_get_single_temperature_degree.response.degree;
					 *status=master_motorCommand.CommandPowerStep1.mini_get_single_temperature_degree.response.status;
				}else{
						ret = 1;//fail
					 *degree=0;
				}
		}
//		LOGD("end mini_get_single_temperature_degree ,ret:%d \r\n",ret);
#endif
		
		return ret;
}


