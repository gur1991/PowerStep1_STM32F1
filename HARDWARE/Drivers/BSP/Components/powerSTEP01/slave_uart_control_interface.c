#include "slave_uart_control_interface.h"

static void protocol_powerstep01_move(move_type_t* data){
		
		move_type_t performer;
		performer.request.devices=data->request.devices;
		performer.request.steps=data->request.steps;
		performer.request.types=data->request.types;
		
		printf("slave devices %d \r\n",performer.request.devices);
		printf("slave steps %d \r\n",performer.request.steps);
		printf("slave types %d \r\n",performer.request.types);
	
		Powerstep01_QueueCommands(performer.request.devices,performer.request.types,performer.request.steps);
		data->response.ret=0;	
		
}

static void protocol_powerstep01_power(power_type_t* data){
			
		power_type_t performer;
		performer.request.devices=data->request.devices;
		performer.request.power=data->request.power;
		
		printf("slave devices %d \r\n",performer.request.devices);
		if(performer.request.power==0)BSP_MotorControl_CmdSoftStop(performer.request.devices);
		printf("slave power %d \r\n",performer.request.power);
		data->response.ret=0;
	
}

static void protocol_powerstep01_rest_pos(rest_pos_type_t* data){
		rest_pos_type_t performer;
		performer.request.devices=data->request.devices;
		
		printf("slave devices %d \r\n",performer.request.devices);
		BSP_MotorControl_CmdResetPos(performer.request.devices);//�˺���������Ӳ������
		printf("slave devices2 %d \r\n",performer.request.devices);
		data->response.ret=0;
}

static void protocol_powerstep01_send_command_and_wait_no_busy(send_command_and_wait_no_busy_type_t* data){
		send_command_and_wait_no_busy_type_t performer;
		
		printf("slave wait  \r\n"); 
		BSP_MotorControl_SendQueuedCommands();
    /* Wait for all device ends moving */ 
    BSP_MotorControl_WaitForAllDevicesNotBusy();//�˺���������Ӳ������
		printf("slave busy  \r\n");
		data->response.ret=0;	
}

static void protocol_powerstep01_one_device_move(one_device_move_type_t* data){
		one_device_move_type_t performer;
		
		performer.request.devices = data->request.devices;
		performer.request.steps = data->request.steps;
	
		printf("slave devices %d \r\n",performer.request.devices);

		//�˺��������ӵ��������
		//BSP_MotorControl_GoTo(performer.request.devices,performer.request.steps);
		printf("slave steps %d \r\n",performer.request.steps);	
		data->response.ret=0;
		
}

static void protocol_powerstep01_one_device_wait(one_device_wait_type_t* data){
		one_device_wait_type_t performer;
		performer.request.devices=data->request.devices;
		printf("slave devices %d \r\n",performer.request.devices);
		BSP_MotorControl_WaitWhileActive(performer.request.devices);//�˺���������Ӳ������
		printf("slave devices2 %d \r\n",performer.request.devices);
		data->response.ret=0;
}

static void protocol_powerstep01_one_device_get_pos(one_device_get_pos_type_t*data){
			one_device_get_pos_type_t performer;
			performer.request.devices=data->request.devices;
			printf("slave devices %d \r\n",performer.request.devices);
			data->response.pos=BSP_MotorControl_GetPosition(performer.request.devices);//�˺���������Ӳ������
	
			data->response.pos=876;
			printf("slave devices2 %d \r\n",performer.request.devices);
		  data->response.ret=0;
}
static void protocol_powerstep01_one_device_set_mark(one_device_set_mark_type_t*data){
			one_device_set_mark_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.pos=data->request.pos;
	
			printf("slave devices %d \r\n",performer.request.devices);
			BSP_MotorControl_SetMark(performer.request.devices, performer.request.pos);//�˺���������Ӳ������
			printf("slave pos %d \r\n",performer.request.pos);
			data->response.ret=0;
}


static void protocol_powerstep01_get_para(get_para_type_t*data){
			get_para_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.para=data->request.para;
			
			printf("slave devices %d \r\n",performer.request.devices);
			//�˺���������Ӳ������
			data->response.result_para=BSP_MotorControl_CmdGetParam(performer.request.devices, performer.request.para);
			printf("slave para %d \r\n",performer.request.para);
			data->response.ret=0;
}
static void protocol_powerstep01_set_para(set_para_type_t*data){
			set_para_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.para=data->request.para;
			performer.request.value=data->request.value;
			printf("slave devices %d \r\n",performer.request.devices);
	//�˺���������Ӳ������
			BSP_MotorControl_CmdSetParam(performer.request.devices,performer.request.para,performer.request.value);
			printf("slave para %d \r\n",performer.request.para);
			data->response.ret=0;
}
static void protocol_powerstep01_select_step_mode(select_step_mode_t*data){
			select_step_mode_t performer;
			performer.request.devices=data->request.devices;
			performer.request.StepMode=data->request.StepMode;
			
			printf("slave devices %d \r\n",performer.request.devices);
	//�˺���������Ӳ������
			BSP_MotorControl_SelectStepMode(performer.request.devices, performer.request.StepMode);
			printf("slave StepMode %d \r\n",performer.request.StepMode);
			data->response.ret=0;
}

static void protocol_get_light_sensor_level(get_light_sensor_level_t*data){
			get_light_sensor_level_t performer;
			performer.request.number=data->request.number;
			
			printf("slave devices %d \r\n",performer.request.number);
			data->response.value=Light_Sensor_Get(performer.request.number);
			data->response.ret=0;
}

static void protocol_cheminert_c52(cheminert_c52_type_t*data){
			static const u8 tx_buf_cp[]={'C','P',0x0d,0x0a};//41 0D 0A
			static const u8 tx_buf_cca[]={'C','C','A',0x0d,0x0a};//41 0D 0A 
			static const u8 tx_buf_ccb[]={'C','C','B',0x0d,0x0a};//42 0D 0A 
			static const u8 tx_buf_cwa[]={'C','W','A',0x0d,0x0a};//41 0D 0A 
			static const u8 tx_buf_cwb[]={'C','W','B',0x0d,0x0a};//42 0D 0A 
			static const u8 tx_buf_dt[]={'D','T','1',0x0d,0x0a};//4F 4B 0D 0A 
			static const u8 tx_buf_goa[]={'G','O','A',0x0d,0x0a};//41 0D 0A 
			static const u8 tx_buf_gob[]={'G','O','B',0x0d,0x0a};//42 0D 0A 
			static const u8 tx_buf_md[]={'M','D',0x0d,0x0a};//4E 6F 6E 65 0D 0A 
			//u8 tx_buf_rc[]={'R','C',0x0d,0x0a};
			static const u8 tx_buf_sb[]={'S','B',0x0d,0x0a};
			//u8 tx_buf_sb4K80[]={'S','B','4','K','8','0',0x0d,0x0a};
			static const u8 tx_buf_sn[]={'S','N',0x0d,0x0a};//4E 6F 6E 65 0D 0A 
			static const u8 tx_buf_to[]={'T','O',0x0d,0x0a};//42 0D 0A 41 0D 0A 
			//u8 tx_buf_tt[]={'T','T',0x0d,0x0a};
			static const u8 tx_buf_vr[]={'V','R',0x0d,0x0a};//43 35 78 5F 20 43 36 78 5F 20 32 36 30 20 46 65 62 20 30 39 20 32 30 31 36 0D 0A 
	
			u8 rx_buf[64];
			u8 tx_size=0;
			u8 tx_buf[10];
			
			cheminert_c52_type_t performer;
			u8 ret,rx_size,i;
			u16 timeout;
			timeout=data->request.timeout;
			performer.request.para=data->request.para;
			switch(performer.request.para){
					case CHEMINERT_C52_CP:
								tx_size=sizeof(tx_buf_cp);	
								memcpy(tx_buf,tx_buf_cp,tx_size);
								break;
				  case CHEMINERT_C52_CCA: 
								tx_size=sizeof(tx_buf_cca);	
								memcpy(tx_buf,tx_buf_cca,tx_size);
								break;
					case CHEMINERT_C52_CCB:
								tx_size=sizeof(tx_buf_ccb);	
								memcpy(tx_buf,tx_buf_ccb,tx_size);
								break;
					case CHEMINERT_C52_CWA:
								tx_size=sizeof(tx_buf_cwa);	
								memcpy(tx_buf,tx_buf_cwa,tx_size);
								break;
					case CHEMINERT_C52_CWB:		
								tx_size=sizeof(tx_buf_cwb);	
								memcpy(tx_buf,tx_buf_cwb,tx_size);
								break;
					case CHEMINERT_C52_DT:
								tx_size=sizeof(tx_buf_dt);	
								memcpy(tx_buf,tx_buf_dt,tx_size);
								break;								
					case CHEMINERT_C52_GOA:
								tx_size=sizeof(tx_buf_goa);	
								memcpy(tx_buf,tx_buf_goa,tx_size);
								break;
					case CHEMINERT_C52_GOB:
								tx_size=sizeof(tx_buf_gob);	
								memcpy(tx_buf,tx_buf_gob,tx_size);
								break;
					case CHEMINERT_C52_MD:		
								tx_size=sizeof(tx_buf_md);	
								memcpy(tx_buf,tx_buf_md,tx_size);
								break;
					case CHEMINERT_C52_SB:
								tx_size=sizeof(tx_buf_sb);	
								memcpy(tx_buf,tx_buf_sb,tx_size);
								break;
					case CHEMINERT_C52_SN:
								tx_size=sizeof(tx_buf_sn);	
								memcpy(tx_buf,tx_buf_sn,tx_size);
								break;
					case CHEMINERT_C52_TO:
								tx_size=sizeof(tx_buf_to);	
								memcpy(tx_buf,tx_buf_to,tx_size);
								break;
					case CHEMINERT_C52_VR:		
								tx_size=sizeof(tx_buf_vr);	
								memcpy(tx_buf,tx_buf_vr,tx_size);
								break;
					default:
								printf("no found this cmd ! \r\n");
			
			}
			ret=cheminert_c52_transfer(tx_buf,tx_size,rx_buf,&rx_size,timeout);
		
			if(!ret){
						memcpy(data->response.buf,rx_buf,rx_size);
						data->response.size=rx_size;
			}
			
			data->response.ret=ret;		

}

//
void protocol_handle_uart_powerstep01_plain_slave_cmd(void){
		uint8_t ret =0;
		u8 len=0;
		Powerstep1_contorl_motor_command_t slave_motorCommand;
		printf("start slave uart\r\n");
		
		UART3_Receive_Data(&slave_motorCommand,&len);
		switch(slave_motorCommand.type){
			case MOVE_TYPE:
						protocol_powerstep01_move(&slave_motorCommand.CommandPowerStep1.move);
						break;
			case POWER_TYPE:
						protocol_powerstep01_power(&slave_motorCommand.CommandPowerStep1.power);
						break;
			case REST_TYPE:
						protocol_powerstep01_rest_pos(&slave_motorCommand.CommandPowerStep1.rest_post);
						break;
			case SEND_COMMAND_AND_WAIT_NOTBUSY_TYPE:
						protocol_powerstep01_send_command_and_wait_no_busy(&slave_motorCommand.CommandPowerStep1.send_and_wait);
						break;
			case ONE_DEVICE_MOVE_TYPE:
						protocol_powerstep01_one_device_move(&slave_motorCommand.CommandPowerStep1.one_device_move);
						break;
			case ONE_DEVICE_WAIT_TYPE:
						protocol_powerstep01_one_device_wait(&slave_motorCommand.CommandPowerStep1.one_device_wait);
						break;
			case ONE_DEVICE_GET_POS_TYPE:
						protocol_powerstep01_one_device_get_pos(&slave_motorCommand.CommandPowerStep1.one_device_get_pos);
						break;
			case ONE_DEVICE_SET_MARK_TYPE:
						protocol_powerstep01_one_device_set_mark(&slave_motorCommand.CommandPowerStep1.one_device_set_mark);
						break;
			case 	GET_PARAM_TYPE:
						protocol_powerstep01_get_para(&slave_motorCommand.CommandPowerStep1.get_para);
						break;
			case 	SET_PARAM_TYPE:
						protocol_powerstep01_set_para(&slave_motorCommand.CommandPowerStep1.set_para);
						break;	
			case SElECT_STEP_MODE_TYPE:
						protocol_powerstep01_select_step_mode(&slave_motorCommand.CommandPowerStep1.select_step_mode);
						break;
			case GET_LIGHT_LEVEL_TYPE:
						protocol_get_light_sensor_level(&slave_motorCommand.CommandPowerStep1.get_light_sensor_level);
						break;
			case CHEMINERT_C52_TYPE:
						protocol_cheminert_c52(&slave_motorCommand.CommandPowerStep1.cheminert_c52);
						break;
			default:
					printf("no found this cmd ! \r\n");
		}
		UART3_Send_Data((u8*)(&slave_motorCommand),sizeof(Powerstep1_contorl_motor_command_t));
		printf("end slave uart\r\n");
}


u8 test_actuator(void){
	u8 ret=0,i;
	cheminert_c52_type_t cheminert_c52;
	cheminert_c52.request.para=	CHEMINERT_C52_TO;
	cheminert_c52.request.timeout=200;
	protocol_cheminert_c52(&cheminert_c52);
	printf("ret:%d \r\n",cheminert_c52.response.ret);
	printf("size:%d \r\n",cheminert_c52.response.size);
	for(i=0;i<cheminert_c52.response.size;i++){
				printf("%c",cheminert_c52.response.buf[i]);
	}
	printf("\r\n");
	return ret;
}



