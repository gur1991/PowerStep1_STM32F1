#include "slave_uart_control_interface.h"

//discard 
static void protocol_powerstep01_move(move_type_t* data){
		
		move_type_t performer;
		performer.request.devices=data->request.devices;
		performer.request.steps=data->request.steps;
		performer.request.types=data->request.types;
		

		//Powerstep01_QueueCommands(performer.request.devices,performer.request.types,performer.request.steps);
		data->response.ret=0;	
		
}

//Ö¸ÁîÖØÐÂ½âÊÍÎªÍ£Ö¹stop 
static void protocol_powerstep01_power(power_type_t* data){
			
		power_type_t performer;
		performer.request.devices=data->request.devices;
		performer.request.power=data->request.power;
	printf("stop value:%d\r\n",performer.request.devices);	
		//0xffÎªÍ£Ö¹ËùÓÐµç»ú
		if(performer.request.devices==0xff)
		{
				StopALLMotorMotion();
		}else{
					PowerStep_Select_Motor_Baby(performer.request.devices);
					BSP_MotorControl_HardStop(0);
		}
	
		data->response.ret=0;
	
}
//¸´Î»Ô­µã
static void protocol_powerstep01_rest_pos(rest_pos_type_t* data){
		rest_pos_type_t performer;
		performer.request.devices=data->request.devices;
	
		PowerStep_Select_Motor_Baby(performer.request.devices);
		BSP_MotorControl_CmdResetPos(performer.request.devices);//´Ëº¯Êý²»Á¬½ÓÓ²¼þ×èÈû
		data->response.ret=0;
}
//discard 
static void protocol_powerstep01_send_command_and_wait_no_busy(send_command_and_wait_no_busy_type_t* data){
		send_command_and_wait_no_busy_type_t performer;
		
		//BSP_MotorControl_SendQueuedCommands();
    /* Wait for all device ends moving */ 
    //BSP_MotorControl_WaitForAllDevicesNotBusy();//´Ëº¯Êý²»Á¬½ÓÓ²¼þ×èÈû
		data->response.ret=0;	
}

//µ¥¸öµç»úÔË¶
//·½Ïò²»ÄÜÓÃÃ¶Åe£¬·ñÔòÊý¾Ý»ìÂÒ
static void protocol_powerstep01_one_device_move(one_device_move_type_t* data){
		one_device_move_type_t performer;
		
		performer.request.devices = data->request.devices;
		performer.request.dir=data->request.dir;
		performer.request.steps = data->request.steps;
	
		PowerStep_Select_Motor_Baby(performer.request.devices);
		BSP_MotorControl_Move(0, (motorDir_t)performer.request.dir, performer.request.steps);

	  data->response.ret=0;
		
}
//µÈ´ýµ¥¸öµç»úÔË¶¯Í£Ö¹
static void protocol_powerstep01_one_device_wait(one_device_wait_type_t* data){
		one_device_wait_type_t performer;
		performer.request.devices=data->request.devices;
	printf("wait stop \r\n");
		PowerStep_Select_Motor_Baby(performer.request.devices);
		BSP_MotorControl_WaitWhileActive(0);	
		data->response.ret=0;
}

//»ñÈ¡µ±Ç°Î»ÖÃ
static void protocol_powerstep01_one_device_get_pos(one_device_get_pos_type_t*data){
			one_device_get_pos_type_t performer;
			performer.request.devices=data->request.devices;
	
			PowerStep_Select_Motor_Baby(performer.request.devices);
			data->response.pos=BSP_MotorControl_GetPosition(performer.request.devices);//´Ëº¯Êý²»Á¬½ÓÓ²¼þ×èÈû
	
		  data->response.ret=0;
}
static void protocol_powerstep01_one_device_set_mark(one_device_set_mark_type_t*data){
			one_device_set_mark_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.pos=data->request.pos;
		
			PowerStep_Select_Motor_Baby(performer.request.devices);
			BSP_MotorControl_SetMark(0, performer.request.pos);//´Ëº¯Êý²»Á¬½ÓÓ²¼þ×èÈû
			data->response.ret=0;
}


static void protocol_powerstep01_get_para(get_para_type_t*data){
			get_para_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.registe=data->request.registe;
			
			PowerStep_Select_Motor_Baby(performer.request.devices);
			data->response.result_para=BSP_MotorControl_CmdGetParam(0, performer.request.registe);

			data->response.ret=0;
}
static void protocol_powerstep01_set_para(set_para_type_t*data){
			set_para_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.registe=data->request.registe;
			performer.request.para=data->request.para;
	
			PowerStep_Select_Motor_Baby(performer.request.devices);
			BSP_MotorControl_CmdSetParam(0,performer.request.para,performer.request.para);
			data->response.ret=0;
}
static void protocol_powerstep01_select_step_mode(select_step_mode_t*data){
			select_step_mode_t performer;
			performer.request.devices=data->request.devices;
			performer.request.StepMode=data->request.StepMode;
			
			PowerStep_Select_Motor_Baby(performer.request.devices);
			BSP_MotorControl_SelectStepMode(0, performer.request.StepMode);
			data->response.ret=0;
}

//discard
static void protocol_init_motor_speed_tension(init_motor_speed_tension_type_t*data){
			init_motor_speed_tension_type_t performer;

			//memcpy(&performer, data,sizeof(performer));
			//data->response.ret=init_motor_device(performer);
}	
//discard
static void protocol_move_many_motor(move_many_motor_type_t*data){
			move_many_motor_type_t performer;
			
			memcpy(&performer, data,sizeof(performer));
			move_many_motor(performer);
			data->response.ret=0;
}
//discard
static void protocol_wait_many_motor(wait_many_motor_type_t*data){
			wait_many_motor_type_t performer;
			
			memcpy(&performer, data,sizeof(performer));
			wait_many_motor(performer);
			data->response.ret=0;
}

static void protocol_command_package_motor(motor_command_package_type_t*data)
{
		motor_command_package_type_t performer;
		performer.request.command = data->request.command;
		process_motor_command_receive(performer.request.command);
		data->response.ret=0;
}	


static void protocol_get_light_sensor_level(get_light_sensor_level_t*data){
			get_light_sensor_level_t performer;
			performer.request.number=data->request.number;
			printf("gggg slave devices %d \r\n",performer.request.number);
			data->response.value=Light_Sensor_Get(performer.request.number);
			data->response.ret=0;
}

static void protocol_get_all_light_sensor_level(get_all_light_sensor_level_t*data){
			get_all_light_sensor_level_t performer;
			
			Light_Sensor_Get_All();
			memcpy(data->response.value, gStatusLight, sizeof(gStatusLight));		
			printf("all light\r\n ");
			data->response.ret=0;
}




static void protocol_cheminert_c52_c55(cheminert_c52_c55_type_t*data){
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
			
			static const u8 tx_buf_cc1[]={'C','C','1',0x0d,0x0a};
			static const u8 tx_buf_cc2[]={'C','C','2',0x0d,0x0a};
			static const u8 tx_buf_cc3[]={'C','C','3',0x0d,0x0a};
      static const u8 tx_buf_cc4[]={'C','C','4',0x0d,0x0a};
      static const u8 tx_buf_cc5[]={'C','C','5',0x0d,0x0a};
      static const u8 tx_buf_cc6[]={'C','C','6',0x0d,0x0a};
      static const u8 tx_buf_cw1[]={'C','W','1',0x0d,0x0a};
			static const u8 tx_buf_cw2[]={'C','W','2',0x0d,0x0a};
			static const u8 tx_buf_cw3[]={'C','W','3',0x0d,0x0a};
			static const u8 tx_buf_cw4[]={'C','W','4',0x0d,0x0a};
			static const u8 tx_buf_cw5[]={'C','W','5',0x0d,0x0a};
      static const u8 tx_buf_cw6[]={'C','W','6',0x0d,0x0a};
			static const u8 tx_buf_gh[]={'G','H',0x0d,0x0a};
			static const u8 tx_buf_go1[]={'G','O','1',0x0d,0x0a};
			static const u8 tx_buf_go2[]={'G','O','2',0x0d,0x0a};
			static const u8 tx_buf_go3[]={'G','O','3',0x0d,0x0a};
			static const u8 tx_buf_go4[]={'G','O','4',0x0d,0x0a};
		  static const u8 tx_buf_go5[]={'G','O','5',0x0d,0x0a};
			static const u8 tx_buf_go6[]={'G','O','6',0x0d,0x0a};
			static const u8 tx_buf_mn[]={'M','N',0x0d,0x0a};
			static const u8 tx_buf_sd[]={'S','D',0x0d,0x0a};
			static const u8 tx_buf_sdcc[]={'S','D','C','C',0x0d,0x0a};
			static const u8 tx_buf_sdcw[]={'S','D','C','W',0x0d,0x0a};
	
			int type_flag=0;//type_flag =1 c52    type_flag=0 c55
			
			u8 rx_buf[64];
			u8 tx_size=0;
			u8 tx_buf[10];
			u8 ret,rx_size,i;
			int timeout;
			cheminert_c52_c55_type_t performer;
			bool wait_flag=true;
			timeout=data->request.timeout;
			if(timeout<1000)timeout=1000;
			performer.request.para=data->request.para;
			switch(performer.request.para){
					case CHEMINERT_C52_CP:
								type_flag=1;
					case CHEMINERT_C55_CP:
								tx_size=sizeof(tx_buf_cp);	
								memcpy(tx_buf,tx_buf_cp,tx_size);
								break;
				  case CHEMINERT_C52_CCA:
								type_flag=1;
								tx_size=sizeof(tx_buf_cca);	
								memcpy(tx_buf,tx_buf_cca,tx_size);
								break;
					case CHEMINERT_C52_CCB:
								type_flag=1;
								tx_size=sizeof(tx_buf_ccb);	
								memcpy(tx_buf,tx_buf_ccb,tx_size);
								break;
					case CHEMINERT_C52_CWA:
								type_flag=1;
								tx_size=sizeof(tx_buf_cwa);	
								memcpy(tx_buf,tx_buf_cwa,tx_size);
								break;
					case CHEMINERT_C52_CWB:
								type_flag=1;
								tx_size=sizeof(tx_buf_cwb);	
								memcpy(tx_buf,tx_buf_cwb,tx_size);
								break;
					case CHEMINERT_C52_DT:
								type_flag=1;
								tx_size=sizeof(tx_buf_dt);	
								memcpy(tx_buf,tx_buf_dt,tx_size);
								break;								
					case CHEMINERT_C52_GOA:
								type_flag=1;
								tx_size=sizeof(tx_buf_goa);	
								memcpy(tx_buf,tx_buf_goa,tx_size);
								break;
					case CHEMINERT_C52_GOB:
								type_flag=1;
								tx_size=sizeof(tx_buf_gob);	
								memcpy(tx_buf,tx_buf_gob,tx_size);
								break;
					case CHEMINERT_C52_MD:
								type_flag=1;
					case CHEMINERT_C55_MD:		
								tx_size=sizeof(tx_buf_md);	
								memcpy(tx_buf,tx_buf_md,tx_size);
								break;
					case CHEMINERT_C52_SB:
								type_flag=1;
					case CHEMINERT_C55_SB:
								tx_size=sizeof(tx_buf_sb);	
								memcpy(tx_buf,tx_buf_sb,tx_size);
								break;
					case CHEMINERT_C52_SN:
								type_flag=1;
					case CHEMINERT_C55_SN:
								tx_size=sizeof(tx_buf_sn);	
								memcpy(tx_buf,tx_buf_sn,tx_size);
								break;
					case CHEMINERT_C52_TO:
								type_flag=1;
								tx_size=sizeof(tx_buf_to);	
								memcpy(tx_buf,tx_buf_to,tx_size);
								break;
					case CHEMINERT_C52_VR:
								type_flag=1;
					case CHEMINERT_C55_VR:						
								tx_size=sizeof(tx_buf_vr);	
								memcpy(tx_buf,tx_buf_vr,tx_size);
								break;
				  case CHEMINERT_C55_CC1:
								tx_size=sizeof(tx_buf_cc1);	
								memcpy(tx_buf,tx_buf_cc1,tx_size);
								break;
				  case CHEMINERT_C55_CC2:
								tx_size=sizeof(tx_buf_cc2);	
								memcpy(tx_buf,tx_buf_cc2,tx_size);
								break;
				  case CHEMINERT_C55_CC3:
								tx_size=sizeof(tx_buf_cc3);	
								memcpy(tx_buf,tx_buf_cc3,tx_size);
								break;
				  case CHEMINERT_C55_CC4:
								tx_size=sizeof(tx_buf_cc4);	
								memcpy(tx_buf,tx_buf_cc4,tx_size);
								break;
				  case CHEMINERT_C55_CC5:
								tx_size=sizeof(tx_buf_cc5);	
								memcpy(tx_buf,tx_buf_cc5,tx_size);
								break;
				  case CHEMINERT_C55_CC6:
								tx_size=sizeof(tx_buf_cc6);	
								memcpy(tx_buf,tx_buf_cc6,tx_size);
								break;					
				  case CHEMINERT_C55_CW1:
								tx_size=sizeof(tx_buf_cw1);	
								memcpy(tx_buf,tx_buf_cw1,tx_size);
								break;
				  case CHEMINERT_C55_CW2:
								tx_size=sizeof(tx_buf_cw2);	
								memcpy(tx_buf,tx_buf_cw2,tx_size);
								break;						
				  case CHEMINERT_C55_CW3:
								tx_size=sizeof(tx_buf_cw3);	
								memcpy(tx_buf,tx_buf_cw3,tx_size);
								break;						
				  case CHEMINERT_C55_CW4:
								tx_size=sizeof(tx_buf_cw4);	
								memcpy(tx_buf,tx_buf_cw4,tx_size);
								break;						
				  case CHEMINERT_C55_CW5:
								tx_size=sizeof(tx_buf_cw5);	
								memcpy(tx_buf,tx_buf_cw5,tx_size);
								break;						
				  case CHEMINERT_C55_CW6:
								tx_size=sizeof(tx_buf_cw6);	
								memcpy(tx_buf,tx_buf_cw6,tx_size);
								break;						
				  case CHEMINERT_C55_GH:
								tx_size=sizeof(tx_buf_gh);	
								memcpy(tx_buf,tx_buf_gh,tx_size);
								break;		
				  case CHEMINERT_C55_GO1:
								tx_size=sizeof(tx_buf_go1);	
								memcpy(tx_buf,tx_buf_go1,tx_size);
								break;
				  case CHEMINERT_C55_GO2:
								tx_size=sizeof(tx_buf_go2);	
								memcpy(tx_buf,tx_buf_go2,tx_size);
								break;		
				  case CHEMINERT_C55_GO3:
								tx_size=sizeof(tx_buf_go3);	
								memcpy(tx_buf,tx_buf_go3,tx_size);
								break;						
				  case CHEMINERT_C55_GO4:
								tx_size=sizeof(tx_buf_go4);	
								memcpy(tx_buf,tx_buf_go4,tx_size);
								break;
				  case CHEMINERT_C55_GO5:
								tx_size=sizeof(tx_buf_go5);	
								memcpy(tx_buf,tx_buf_go5,tx_size);
								break;
				  case CHEMINERT_C55_GO6:
								tx_size=sizeof(tx_buf_go6);	
								memcpy(tx_buf,tx_buf_go6,tx_size);
								break;
				  case CHEMINERT_C55_MN:
								tx_size=sizeof(tx_buf_mn);	
								memcpy(tx_buf,tx_buf_mn,tx_size);
								break;					
				  case CHEMINERT_C55_SD:
								tx_size=sizeof(tx_buf_sd);	
								memcpy(tx_buf,tx_buf_sd,tx_size);
								break;						
				  case CHEMINERT_C55_SDCC:
								tx_size=sizeof(tx_buf_sdcc);	
								memcpy(tx_buf,tx_buf_sdcc,tx_size);
								wait_flag=false;
								break;	
				  case CHEMINERT_C55_SDCW:
								tx_size=sizeof(tx_buf_sdcw);	
								memcpy(tx_buf,tx_buf_sdcw,tx_size);
								wait_flag=false;
								break;
					
					default:
								printf("no found this cmd ! \r\n");
			
			}
			
			ret= cheminert_c52_c55_transfer(tx_buf,tx_size,rx_buf,&rx_size,timeout,wait_flag, type_flag);
			if(!ret){
						memcpy(data->response.buf,rx_buf,rx_size);
						data->response.size=rx_size;
			}
			//printf("%c\r\n",rx_buf[0]);
			
			data->response.ret=ret;		
}
/*
		u8 S100_STX;
		u8 S100_ID[2];
		u8 S100_AI;
		u8 S100_PFC[2];
		u8 S100_VALUE[6];
		u8 S100_CRC[3];
		u8 S100_ETX;
*/
static void protocol_pump_s100_interface(pump_s100_command_type_t*data){
			pump_s100_command_type_t performer;
			PUMP_S100_REPLY_type_t type;
	    pump_s100_reply_type_t s100_reply;
			uint8_t ret=0,i;
	
	
			printf("size:%d \r\n",sizeof(data->request.para.S100_VALUE));	
			for(i=0;i<sizeof(data->request.para.S100_VALUE);i++){
					printf("value gggg:0x%x\r\n",data->request.para.S100_VALUE[i]);
			}	
	
			performer.request.para.S100_STX=data->request.para.S100_STX;
			memcpy(performer.request.para.S100_ID,data->request.para.S100_ID,sizeof(performer.request.para.S100_ID));
			performer.request.para.S100_AI=data->request.para.S100_AI;
			memcpy(performer.request.para.S100_PFC,data->request.para.S100_PFC,sizeof(performer.request.para.S100_PFC));
			memcpy(performer.request.para.S100_VALUE,data->request.para.S100_VALUE,sizeof(performer.request.para.S100_VALUE));
			memcpy(performer.request.para.S100_CRC,data->request.para.S100_CRC,sizeof(performer.request.para.S100_CRC));
			performer.request.para.S100_ETX=data->request.para.S100_ETX;
			
			performer.request.timeout=data->request.timeout;
			

			ret=pump_s100_transfer(&(performer.request.para), &type, &s100_reply, performer.request.timeout);
			
			data->response.ret=ret;	
			data->response.PUMP_S100_REPLY_type=type;
	
			printf("pump type:%d \r\n",data->response.PUMP_S100_REPLY_type);
			if(type==SPECIAL_ACK_S100){
						data->response.s100_reply.SpecialACK.S100_RESULT=s100_reply.SpecialACK.S100_RESULT;
			}else if(type==NORMAL_ANSWER_S100){
						data->response.s100_reply.NormalAnswer.S100_AI=s100_reply.NormalAnswer.S100_AI;
						memcpy(data->response.s100_reply.NormalAnswer.S100_VALUE,s100_reply.NormalAnswer.S100_VALUE,sizeof(s100_reply.NormalAnswer.S100_VALUE));
						for(i=0;i<sizeof(data->request.para.S100_VALUE);i++){
							printf("value zzz:%c\r\n",data->response.s100_reply.NormalAnswer.S100_VALUE[i]);
			}	
			}else if(type==ACTIVE_EVENT_S100){
						data->response.s100_reply.ActiveEvent.S100_AI=s100_reply.ActiveEvent.S100_AI;
						memcpy(data->response.s100_reply.ActiveEvent.S100_VALUE,s100_reply.ActiveEvent.S100_VALUE,sizeof(s100_reply.ActiveEvent.S100_VALUE));
						memcpy(data->response.s100_reply.ActiveEvent.S100_PFC,s100_reply.ActiveEvent.S100_PFC,sizeof(s100_reply.ActiveEvent.S100_PFC));
			}
}
/*
static void protocol_print_outcome_interface(print_outcome_type_t *data){
			print_outcome_type_t performer;
			HBA1C_DATA_type hba1c;
	
			hba1c.All_Area=data->request.DATA.All_Area;
			hba1c.Peak_Area=data->request.DATA.Peak_Area;
			hba1c.Percentage=data->request.DATA.Percentage;
			hba1c.eAg=data->request.DATA.eAg;
			Printer_outcome(hba1c);
}
*/

static void protocol_set_weight_warning_line_interface(set_weight_warning_line_type_t *data){
			set_weight_warning_line_type_t performer;
	
			performer.request.weight=data->request.weight;
			performer.request.gram=data->request.gram;
			
	printf("set warnig weight£º%d gram:%d\r\n",performer.request.weight,performer.request.gram);
			Set_Weight_Sensor_Warnning_Line(performer.request.weight,performer.request.gram);
			
			data->response.ret=0;

}
static void protocol_get_single_weight_warning_result_interface(get_single_weight_warning_result_type_t *data)
{
			get_single_weight_warning_result_type_t performer;
	
			performer.request.weight=data->request.weight;

			printf("get warnig weight£º%d\r\n",performer.request.weight);
			data->response.result = Get_Single_Weight_Sensor_Warnning_Result(performer.request.weight);
			data->response.ret=0;
}


static void protocol_get_all_weight_warning_result_interface(get_all_weight_warning_result_type_t *data)
{
			get_all_weight_warning_result_type_t  performer;
			printf("get all warnig\r\n");

			data->response.result = Get_All_Weight_Sensor_Warnning_Result();	
			data->response.ret=0;
}

static void protocol_get_single_temperature_degree_interface(get_single_temperature_degree_type_t *data)
{
			get_single_temperature_degree_type_t  performer;
			
			performer.request.devices=data->request.devices;
			
			printf("get tmp devices:%d \r\n",performer.request.devices);

			data->response.degree = ThermometerHandle->get_degree(performer.request.devices);
			data->response.ret = 0;
}

static void protocol_set_single_temperature_degree_interface(set_single_temperature_degree_type_t *data)
{
			set_single_temperature_degree_type_t  performer;
		
			performer.request.devices=data->request.devices;
			performer.request.degree=data->request.degree;
			
			printf("set tmp devices:%d degree:%d \r\n",performer.request.devices,performer.request.degree);

			ThermometerHandle->set_degree(performer.request.degree, performer.request.devices);
			data->response.ret=0;
}

static void protocol_electromagnetic_interface(electromagnetic_type_t* data)
{
			electromagnetic_type_t  performer;
	
			performer.request.devices=data->request.devices;
			performer.request.status=data->request.status;
	
			printf("electromagnetic devices:%d status:%d \r\n",performer.request.devices,performer.request.status);
	
			electromagnetic_control(performer.request.devices, performer.request.status);
			data->response.ret=0;
}	
static void protocol_connect_test(connect_test_type_t* data)
{
	
	#if USE_MOTOR_BOARD
			data->response.ret='M';
	#endif
	
	#if USE_SENSOR_BOARD
			data->response.ret='S';
	#endif
}	

//
void protocol_handle_uart_powerstep01_plain_slave_cmd(void){
		uint8_t ret =0;
		int len=0;
		Powerstep1_contorl_motor_command_t slave_motorCommand;
		printf("start slave uart\r\n");
		memset(&slave_motorCommand,0,sizeof(Powerstep1_contorl_motor_command_t));
		UART4_Receive_Data((u8*)(&slave_motorCommand),&len);
		
	
		if(sizeof(Powerstep1_contorl_motor_command_t)!=len)
		{
				printf("receive data len error !\r\n");
				goto OVER;
		}
		
		if(slave_motorCommand.OverReceiveFlag[0]!=OVER_UART_VALUE0||slave_motorCommand.OverReceiveFlag[1]!=OVER_UART_VALUE1){
					printf("check flag error!\r\n");
					goto OVER;
		}
			
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
			case INIT_MOTOR_SPEED_TENSION_TYPE:
						protocol_init_motor_speed_tension(&slave_motorCommand.CommandPowerStep1.init_motor_speed_tension);
						break;
			case MOVE_MANY_MOTOR:
						protocol_move_many_motor(&slave_motorCommand.CommandPowerStep1.move_many_motor);
						break;
			case WAIT_MANY_MOTOR:
						protocol_wait_many_motor(&slave_motorCommand.CommandPowerStep1.wait_many_motor);
						break;
			case COMMAND_PACKAGE_MOTOR:
					  protocol_command_package_motor(&slave_motorCommand.CommandPowerStep1.motor_command_package);
						break;
			case GET_LIGHT_LEVEL_TYPE:
						protocol_get_light_sensor_level(&slave_motorCommand.CommandPowerStep1.get_light_sensor_level);
						break;
			case GET_ALL_LIGHT_LEVEL_TYPE:
						protocol_get_all_light_sensor_level(&slave_motorCommand.CommandPowerStep1.get_all_light_sensor_level);	
						break;	
			case CHEMINERT_C52_C55_TYPE:
						protocol_cheminert_c52_c55(&slave_motorCommand.CommandPowerStep1.cheminert_c52_c55);
						break;
			case PUMP_S100_TYPE:
						protocol_pump_s100_interface(&slave_motorCommand.CommandPowerStep1.pump_s100_command);
						break;
		//	case PRINTER_F37C_OUTCOME_TYPE:
		//				protocol_print_outcome_interface(&slave_motorCommand.CommandPowerStep1.print_outcome);
		//				break;
			case WEIGHT_SENSOR_SET_LINE_TYPE:
						protocol_set_weight_warning_line_interface(&slave_motorCommand.CommandPowerStep1.set_weight_warning_line);
						break;				
			case WEIGHT_SENSOR_GET_SINGLE_RESULT_TYPE:
						protocol_get_single_weight_warning_result_interface(&slave_motorCommand.CommandPowerStep1.get_single_weight_warning_result);
						break;	
			case WEIGHT_SENSOR_GET_ALL_RESULT_TYPE:
						protocol_get_all_weight_warning_result_interface(&slave_motorCommand.CommandPowerStep1.get_all_weight_warning_result);						
						break;	
			case TEMPERATURE_SENSOR_GET_TYPE:
						protocol_get_single_temperature_degree_interface(&slave_motorCommand.CommandPowerStep1.get_single_temperature_degree);
						break;
			case TEMPERATURE_SENSOR_SET_TYPE:
						protocol_set_single_temperature_degree_interface(&slave_motorCommand.CommandPowerStep1.set_single_temperature_degree);
						break;
			case ELECTROMAGNETIC_TYPE:
						protocol_electromagnetic_interface(&slave_motorCommand.CommandPowerStep1.electromagnetic);
						break;
			case CONNECT_TEST_TYPE:
						protocol_connect_test(&slave_motorCommand.CommandPowerStep1.connect_test);
						break;
					
			default:
					printf("no found this cmd ! %d \r\n",slave_motorCommand.type);
		}
OVER:		
		UART4_Send_Data((u8*)(&slave_motorCommand),sizeof(Powerstep1_contorl_motor_command_t));
		printf("end slave uart\r\n");
}


u8 test_actuator(Command_Cheminert_type_t type){
	u8 ret=0,i;
	cheminert_c52_c55_type_t cheminert_c52_c55;
	cheminert_c52_c55.request.para=	type;
	cheminert_c52_c55.request.timeout=1000;
	protocol_cheminert_c52_c55(&cheminert_c52_c55);
	printf("ret:%d \r\n",cheminert_c52_c55.response.ret);
	printf("size:%d \r\n",cheminert_c52_c55.response.size);
	for(i=0;i<cheminert_c52_c55.response.size;i++){
				printf("%c",cheminert_c52_c55.response.buf[i]);
	}
	printf("\r\n");
	return ret;
}
/*
	S100_testCommand.S100_STX=0x21;
	S100_testCommand.S100_ETX=0x0a;
	S100_testCommand.S100_ID[1]='1'; //ID gu ding
	S100_testCommand.S100_ID[0]='0';
	S100_testCommand.S100_AI='1';//A B Beng
	S100_testCommand.S100_PFC[1]='0';//Command
	S100_testCommand.S100_PFC[0]='9';
	
	S100_testCommand.S100_VALUE[5]=0x20;
	S100_testCommand.S100_VALUE[4]=0x20;
	S100_testCommand.S100_VALUE[3]='0';
	S100_testCommand.S100_VALUE[2]='0';
	S100_testCommand.S100_VALUE[1]='0';
	S100_testCommand.S100_VALUE[0]='0';


*/
void test_pump_s100(void)
{
	u8 ret=0,i;
	pump_s100_command_type_t data;
	
	data.request.timeout=1000;
	
	data.request.para.S100_STX=0x21;
	data.request.para.S100_ETX=0x0a;
	
	data.request.para.S100_ID[1]='1'; 
	data.request.para.S100_ID[0]='0';
	
	data.request.para.S100_AI='0';
	
	data.request.para.S100_PFC[1]='0';
	data.request.para.S100_PFC[0]='1';
	

	data.request.para.S100_VALUE[5]=0x20;//0x20
	data.request.para.S100_VALUE[4]=0x20;//0x20
	data.request.para.S100_VALUE[3]='0';
	data.request.para.S100_VALUE[2]='0';
	data.request.para.S100_VALUE[1]='0';
	data.request.para.S100_VALUE[0]='9';
	
	protocol_pump_s100_interface(&data);
	
	printf("ret:%d \r\n",data.response.ret);
	printf("type:%d\r\n",data.response.PUMP_S100_REPLY_type);
	if(NORMAL_ANSWER_S100==data.response.PUMP_S100_REPLY_type){
			printf("AI:%d \r\n",data.response.s100_reply.NormalAnswer.S100_AI);
			for(i=0;i<6;i++){
					printf("%c",data.response.s100_reply.NormalAnswer.S100_VALUE[5-i]);
			}
			printf("\r\n");
	}else if(SPECIAL_ACK_S100==data.response.PUMP_S100_REPLY_type){
			printf("ACK:%c\n\r",data.response.s100_reply.SpecialACK.S100_RESULT);
	}else if(data.response.PUMP_S100_REPLY_type==UNKNOWN_ANSWER){
			printf("unkown answer! \r\n");
	}
	
}

