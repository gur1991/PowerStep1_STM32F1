#include "master_uart_control_interface.h"


uint8_t Master_PowerStep01_Transfer_Interface(Powerstep1_contorl_motor_command_t* master,u8 length_data,u8* len){
		uint8_t ret=0;
		u8 times =1000;
		master->OverReceiveFlag[0]=OVER_UART_VALUE0;
		master->OverReceiveFlag[1]=OVER_UART_VALUE1;
	
		RS485_Send_Data((u8*)(master),length_data);
		while(1){
			if(FLAG_UART_MASTER){
					RS485_Receive_Data((u8*)(master),len);
					FLAG_UART_MASTER=0;
					printf("master receive data ok\r\n");
					break;		
			}
			times--;
			if(times ==0){
					ret=1;
					printf("transfer error ! \r\n");
					break;
			}
			delay_ms(10);
			//printf("wait  \r\n");
		}
		return ret;
}


uint8_t master_powerStep01_move_command(move_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = MOVE_TYPE;
		master_motorCommand.CommandPowerStep1.move=data;
		printf("start master_powerStep01_move_command \r\n");	
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
					ret = master_motorCommand.CommandPowerStep1.move.response.ret;
				}else{
					ret = 1;//fail
				}
		}
		printf("end master_powerStep01_move_command,ret:%d \r\n",ret);
		return ret;
}



uint8_t master_powerStep01_power_command(power_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = POWER_TYPE;
		master_motorCommand.CommandPowerStep1.power=data;
		printf("start master_powerStep01_power_command \r\n");	
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.power.response.ret;
				}else{
					ret = 1;//fail
				}
		}
		printf("end master_powerStep01_power_command ,ret:%d \r\n",ret);
		return ret;
}


uint8_t master_powerStep01_rest_pos_command(rest_pos_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = REST_TYPE;
		master_motorCommand.CommandPowerStep1.rest_post=data;
		printf("start master_powerStep01_rest_pos_command \r\n");	
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");

		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.rest_post.response.ret;
				}else{
						ret = 1;//fail
				}
		}
		printf("end master_powerStep01_rest_pos_command ,ret:%d \r\n",ret);
		return ret;
}


uint8_t master_powerStep01_send_command_and_wait_no_busy(send_command_and_wait_no_busy_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = SEND_COMMAND_AND_WAIT_NOTBUSY_TYPE;
		master_motorCommand.CommandPowerStep1.send_and_wait=data;
		printf("start master_powerStep01_send_command_and_wait_no_busy \r\n");	
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
			if(len!=0){
					ret = master_motorCommand.CommandPowerStep1.send_and_wait.response.ret;
			}else{
					ret = 1;//fail
			}
		}	
		printf("end master_powerStep01_send_command_and_wait_no_busy ,ret:%d \r\n",ret);
		return ret;
}

uint8_t master_powerStep01_one_device_move(one_device_move_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = ONE_DEVICE_MOVE_TYPE;
		master_motorCommand.CommandPowerStep1.one_device_move=data;
		printf("start master_powerStep01_one_device_move \r\n");	
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.one_device_move.response.ret;
				}else{
						ret = 1;//fail
				}
		}
		printf("end master_powerStep01_one_device_move ,ret:%d \r\n",ret);
		return ret;
}


uint8_t master_powerStep01_one_device_wait(one_device_wait_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = ONE_DEVICE_WAIT_TYPE;
		master_motorCommand.CommandPowerStep1.one_device_wait=data;
		printf("start master_powerStep01_one_device_wait \r\n");	
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.one_device_wait.response.ret;
				}else{
						ret = 1;//fail
				}
		}
		printf("end master_powerStep01_one_device_wait ,ret:%d \r\n",ret);
		return ret;
}

uint8_t master_powerStep01_one_device_get_pos(one_device_get_pos_type_t data,int32_t*pos){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = ONE_DEVICE_GET_POS_TYPE;
		master_motorCommand.CommandPowerStep1.one_device_get_pos=data;
		printf("start master_powerStep01_one_device_get_pos \r\n");	
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.one_device_get_pos.response.ret;
				}else{
					ret = 1;//fail
				}
		}
		*pos=master_motorCommand.CommandPowerStep1.one_device_get_pos.response.pos; 
		printf("end master_powerStep01_one_device_get_pos ,ret:%d \r\n",ret);
		return ret;
}


uint8_t master_powerStep01_one_device_set_mark(one_device_set_mark_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = ONE_DEVICE_SET_MARK_TYPE;
		master_motorCommand.CommandPowerStep1.one_device_set_mark=data;
		printf("start master_powerStep01_one_device_set_mark \r\n");	
		
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.one_device_set_mark.response.ret;
				}else{
						ret = 1;//fail
				}
		}
		printf("end master_powerStep01_one_device_set_mark,ret:%d \r\n",ret);
		return ret;
}


uint8_t master_powerStep01_get_para(get_para_type_t data,powerstep01_Registers_t*para){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = GET_PARAM_TYPE;
		master_motorCommand.CommandPowerStep1.get_para=data;
		printf("start master_powerStep01_get_para \r\n");	
		
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.get_para.response.ret;
				}else{
						ret = 1;//fail
				}
		}
		*para=master_motorCommand.CommandPowerStep1.get_para.response.result_para;
		printf("end master_powerStep01_get_para ,ret:%d \r\n",ret);
		return ret;
}

uint8_t master_powerStep01_set_para(set_para_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = SET_PARAM_TYPE;
		master_motorCommand.CommandPowerStep1.set_para=data;
		printf("start master_powerStep01_set_para \r\n");	
		
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.set_para.response.ret;
				}else{
						ret = 1;//fail
				}
		}
		printf("end master_powerStep01_set_para ,ret:%d \r\n",ret);
		return ret;
}

uint8_t master_powerStep01_select_step_mode(select_step_mode_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = SElECT_STEP_MODE_TYPE;
		master_motorCommand.CommandPowerStep1.select_step_mode=data;
		printf("start master_powerStep01_select_step_mode \r\n");	
		
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.select_step_mode.response.ret;
				}else{
						ret = 1;//fail
				}
		}
		printf("end master_powerStep01_select_step_mode ,ret:%d \r\n",ret);
		return ret;
}

uint8_t master_get_light_sensor_level(get_light_sensor_level_t data,uint8_t*value){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = GET_LIGHT_LEVEL_TYPE;
		master_motorCommand.CommandPowerStep1.get_light_sensor_level=data;
		printf("start master_get_light_sensor_level \r\n");	
		
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.get_light_sensor_level.response.ret;
				}else{
						ret = 1;//fail
				}
				*value=master_motorCommand.CommandPowerStep1.get_light_sensor_level.response.value;
		}
	
		printf("end master_get_light_sensor_level ,ret:%d \r\n",ret);
		return ret;
}

//cheminert_c52_type_t
uint8_t master_cheminert_c52_c55(cheminert_c52_c55_type_t data,uint8_t*buf,uint8_t*buf_size){
		uint8_t ret =0;
		u8 len =0;
		u8 size=0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = CHEMINERT_C52_C55_TYPE;
		master_motorCommand.CommandPowerStep1.cheminert_c52_c55=data;
		printf("start master_cheminert_c52_c55 \r\n");	
		
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.cheminert_c52_c55.response.ret;
				}else{
						ret = 1;//fail
				}
				size=master_motorCommand.CommandPowerStep1.cheminert_c52_c55.response.size;
				memcpy(buf,master_motorCommand.CommandPowerStep1.cheminert_c52_c55.response.buf,size);
				*buf_size=size;
		}
	
		printf("end master_cheminert_c52_c55 ,ret:%d \r\n",ret);
		return ret;
}


uint8_t master_pump_s100_interface(pump_s100_command_type_t data,pump_s100_reply_type_t* reply,PUMP_S100_REPLY_type_t* type){
		uint8_t ret =0;
		u8 len =0;
		u8 size=0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = PUMP_S100_TYPE;
		master_motorCommand.CommandPowerStep1.pump_s100_command=data;
		printf("start master_pump_s100_interface \r\n");	
		
		ret=Master_PowerStep01_Transfer_Interface(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t),&len);
		printf("wait out \r\n");
		//len值为0的时候，从设备没有反馈
		if(!ret){
				if(len!=0){
						ret = master_motorCommand.CommandPowerStep1.pump_s100_command.response.ret;
				}else{
						ret = 1;//fail
				}
				*type=master_motorCommand.CommandPowerStep1.pump_s100_command.response.PUMP_S100_REPLY_type;
				
				//two way  output result to extern
				//memcpy(reply,&(master_motorCommand.CommandPowerStep1.pump_s100_command.response.s100_reply),sizeof(pump_s100_reply_type_t));
				if(*type==NORMAL_ANSWER_S100){
							reply->NormalAnswer.S100_AI=master_motorCommand.CommandPowerStep1.pump_s100_command.response.s100_reply.NormalAnswer.S100_AI;
							memcpy(reply->NormalAnswer.S100_VALUE,master_motorCommand.CommandPowerStep1.pump_s100_command.response.s100_reply.NormalAnswer.S100_VALUE,sizeof(reply->NormalAnswer.S100_VALUE));
				}else if(*type==SPECIAL_ACK_S100){
							reply->SpecialACK.S100_RESULT=master_motorCommand.CommandPowerStep1.pump_s100_command.response.s100_reply.SpecialACK.S100_RESULT;
				}else if(*type==ACTIVE_EVENT_S100){
							reply->ActiveEvent.S100_AI=master_motorCommand.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_AI;
							memcpy(reply->ActiveEvent.S100_PFC,master_motorCommand.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_PFC,sizeof(reply->ActiveEvent.S100_PFC));
							memcpy(reply->ActiveEvent.S100_VALUE,master_motorCommand.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_VALUE,sizeof(reply->ActiveEvent.S100_VALUE));
				}else if(*type==UNKNOWN_ANSWER){
							ret=1;//fuck result!
							printf("master_pump_s100, what a fuck result!\r\n");
				}
		}
	
		printf("end master_pump_s100_interface ,ret:%d \r\n",ret);
		return ret;
}



