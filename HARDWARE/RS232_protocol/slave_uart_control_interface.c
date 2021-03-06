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

//指令重新解释为停止stop 
static void protocol_powerstep01_power(power_type_t* data){
			
		power_type_t performer;
		performer.request.devices=data->request.devices;
		performer.request.power=data->request.power;
		//0xff为停止所有电机
		if(performer.request.devices==0xff)
		{
				StopALLMotorMotion();
		}else{
					PowerStep_Select_Motor_Baby(performer.request.devices);
					BSP_MotorControl_HardStop(0);
		}
	
		data->response.ret=0;
	
}
//复位原点
static void protocol_powerstep01_rest_pos(rest_pos_type_t* data){
		rest_pos_type_t performer;
		performer.request.devices=data->request.devices;
	
		PowerStep_Select_Motor_Baby(performer.request.devices);
		BSP_MotorControl_CmdResetPos(performer.request.devices);//此函数不连接硬件阻塞
		data->response.ret=0;
}
//discard 
static void protocol_powerstep01_send_command_and_wait_no_busy(send_command_and_wait_no_busy_type_t* data){
		send_command_and_wait_no_busy_type_t performer;
		
		//BSP_MotorControl_SendQueuedCommands();
    /* Wait for all device ends moving */ 
    //BSP_MotorControl_WaitForAllDevicesNotBusy();//此函数不连接硬件阻塞
		data->response.ret=0;	
}

//单个电机运�
//方向不能用枚舉，否则数据混乱
static void protocol_powerstep01_one_device_move(one_device_move_type_t* data){
		one_device_move_type_t performer;
		
		performer.request.devices = data->request.devices;
		performer.request.dir=data->request.dir;
		performer.request.steps = data->request.steps;
	
		PowerStep_Select_Motor_Baby(performer.request.devices);
		BSP_MotorControl_Move(0, (motorDir_t)performer.request.dir, performer.request.steps);

	  data->response.ret=0;
		
}
//等待单个电机运动停止
static void protocol_powerstep01_one_device_wait(one_device_wait_type_t* data){
		one_device_wait_type_t performer;
		performer.request.devices=data->request.devices;
		PowerStep_Select_Motor_Baby(performer.request.devices);
		BSP_MotorControl_WaitWhileActive(0);	
		data->response.ret=0;
}

//获取当前位置
static void protocol_powerstep01_one_device_get_pos(one_device_get_pos_type_t*data){
			one_device_get_pos_type_t performer;
			performer.request.devices=data->request.devices;
	
			PowerStep_Select_Motor_Baby(performer.request.devices);
			data->response.pos=BSP_MotorControl_GetPosition(performer.request.devices);//此函数不连接硬件阻塞
	
		  data->response.ret=0;
}
static void protocol_powerstep01_one_device_set_mark(one_device_set_mark_type_t*data){
			one_device_set_mark_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.pos=data->request.pos;
		
			PowerStep_Select_Motor_Baby(performer.request.devices);
			BSP_MotorControl_SetMark(0, performer.request.pos);//此函数不连接硬件阻塞
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

static void protocol_init_motor_speed_tension(init_motor_speed_tension_type_t*data){
			init_motor_speed_tension_type_t performer;
	
			memcpy(&performer, data,sizeof(performer));
	
			Set_Single_Motor_Config(*data);
			data->response.ret=0;	
}	
//discard
static void protocol_move_many_motor(move_many_motor_type_t*data){
			move_many_motor_type_t performer;
			
			memcpy(&performer, data,sizeof(performer));
			//move_many_motor(performer);
			data->response.ret=0;
}
//discard
static void protocol_wait_many_motor(wait_many_motor_type_t*data){
			wait_many_motor_type_t performer;
			
			memcpy(&performer, data,sizeof(performer));
			//wait_many_motor(performer);
			data->response.ret=0;
}

static void protocol_command_package_motor(motor_command_package_type_t*data)
{
		motor_command_package_type_t performer;
		performer.request.command = data->request.command;
		data->response.value=process_motor_command_receive(performer.request.command);
		data->response.ret=0;
}	


static void protocol_get_light_sensor_level(get_light_sensor_level_t*data){
			get_light_sensor_level_t performer;
			performer.request.number=data->request.number;
			data->response.value=Light_Sensor_Get(performer.request.number);
			data->response.ret=0;
}

static void protocol_get_all_light_sensor_level(get_all_light_sensor_level_t*data){
			get_all_light_sensor_level_t performer;
			
			Light_Sensor_Get_All();
			memcpy(data->response.value, gStatusLight, sizeof(gStatusLight));		
			data->response.ret=0;
}




static void protocol_cheminert_c52_c55(cheminert_c52_c55_type_t*data){
			static const u8 tx_buf_cp[4]={'C','P',0x0d,0x0a};//41 0D 0A
			static const u8 tx_buf_cca[5]={'C','C','A',0x0d,0x0a};//41 0D 0A 
			static const u8 tx_buf_ccb[5]={'C','C','B',0x0d,0x0a};//42 0D 0A 
			static const u8 tx_buf_cwa[5]={'C','W','A',0x0d,0x0a};//41 0D 0A 
			static const u8 tx_buf_cwb[5]={'C','W','B',0x0d,0x0a};//42 0D 0A 
			static const u8 tx_buf_dt[5]={'D','T','1',0x0d,0x0a};//4F 4B 0D 0A 
			static const u8 tx_buf_goa[5]={'G','O','A',0x0d,0x0a};//41 0D 0A 
			static const u8 tx_buf_gob[5]={'G','O','B',0x0d,0x0a};//42 0D 0A 
			static const u8 tx_buf_md[4]={'M','D',0x0d,0x0a};//4E 6F 6E 65 0D 0A 
			//u8 tx_buf_rc[]={'R','C',0x0d,0x0a};
			static const u8 tx_buf_sb[4]={'S','B',0x0d,0x0a};
			//u8 tx_buf_sb4K80[]={'S','B','4','K','8','0',0x0d,0x0a};
			static const u8 tx_buf_sn[4]={'S','N',0x0d,0x0a};//4E 6F 6E 65 0D 0A 
			static const u8 tx_buf_to[4]={'T','O',0x0d,0x0a};//42 0D 0A 41 0D 0A 
			//u8 tx_buf_tt[]={'T','T',0x0d,0x0a};
			static const u8 tx_buf_vr[4]={'V','R',0x0d,0x0a};//43 35 78 5F 20 43 36 78 5F 20 32 36 30 20 46 65 62 20 30 39 20 32 30 31 36 0D 0A 
			
			static const u8 tx_buf_cc1[5]={'C','C','1',0x0d,0x0a};
			static const u8 tx_buf_cc2[5]={'C','C','2',0x0d,0x0a};
			static const u8 tx_buf_cc3[5]={'C','C','3',0x0d,0x0a};
      static const u8 tx_buf_cc4[5]={'C','C','4',0x0d,0x0a};
      static const u8 tx_buf_cc5[5]={'C','C','5',0x0d,0x0a};
      static const u8 tx_buf_cc6[5]={'C','C','6',0x0d,0x0a};
      static const u8 tx_buf_cw1[5]={'C','W','1',0x0d,0x0a};
			static const u8 tx_buf_cw2[5]={'C','W','2',0x0d,0x0a};
			static const u8 tx_buf_cw3[5]={'C','W','3',0x0d,0x0a};
			static const u8 tx_buf_cw4[5]={'C','W','4',0x0d,0x0a};
			static const u8 tx_buf_cw5[5]={'C','W','5',0x0d,0x0a};
      static const u8 tx_buf_cw6[5]={'C','W','6',0x0d,0x0a};
			static const u8 tx_buf_gh[4]={'G','H',0x0d,0x0a};
			static const u8 tx_buf_go1[5]={'G','O','1',0x0d,0x0a};
			static const u8 tx_buf_go2[5]={'G','O','2',0x0d,0x0a};
			static const u8 tx_buf_go3[5]={'G','O','3',0x0d,0x0a};
			static const u8 tx_buf_go4[5]={'G','O','4',0x0d,0x0a};
		  static const u8 tx_buf_go5[5]={'G','O','5',0x0d,0x0a};
			static const u8 tx_buf_go6[5]={'G','O','6',0x0d,0x0a};
			static const u8 tx_buf_mn[5]={'M','N',0x0d,0x0a};
			static const u8 tx_buf_sd[5]={'S','D',0x0d,0x0a};
			static const u8 tx_buf_sdcc[6]={'S','D','C','C',0x0d,0x0a};
			static const u8 tx_buf_sdcw[6]={'S','D','C','W',0x0d,0x0a};
	
			int type_flag=0;//type_flag =1 c52    type_flag=0 c55
			
			u8 rx_buf[64];
			u8 tx_size=0;
			u8 tx_buf[10];
			u8 ret,rx_size,i,j=3;
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
while(j--){
			ret= cheminert_c52_c55_transfer(tx_buf,tx_size,rx_buf,&rx_size,timeout,wait_flag, type_flag);
			if(!ret){
						memcpy(data->response.buf,rx_buf,rx_size);
						data->response.size=rx_size;
						break;
			}
}			
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
			
			Set_Weight_Sensor_Warnning_Line(performer.request.weight,performer.request.gram);
			
			data->response.ret=0;

}
static void protocol_get_single_weight_warning_result_interface(get_single_weight_warning_result_type_t *data)
{
			get_single_weight_warning_result_type_t performer;
	
			performer.request.weight=data->request.weight;

			data->response.result = Get_Single_Weight_Sensor_Warnning_Result(performer.request.weight);
			data->response.ret=0;
}


static void protocol_get_all_weight_warning_result_interface(get_all_weight_warning_result_type_t *data)
{
			get_all_weight_warning_result_type_t  performer;

			data->response.result = Get_All_Weight_Sensor_Warnning_Result();	
			data->response.ret=0;
}

static void protocol_get_single_temperature_degree_interface(get_single_temperature_degree_type_t *data)
{
			get_single_temperature_degree_type_t  performer;
			
			performer.request.devices=data->request.devices;
			

			data->response.degree = ThermometerHandle->get_degree(performer.request.devices);
			data->response.ret = 0;
}

static void protocol_set_single_temperature_degree_interface(set_single_temperature_degree_type_t *data)
{
			set_single_temperature_degree_type_t  performer;
		
			performer.request.devices=data->request.devices;
			performer.request.degree=data->request.degree;
			

			ThermometerHandle->set_degree(performer.request.degree, performer.request.devices);
			data->response.ret=0;
}

static void protocol_electromagnetic_interface(electromagnetic_type_t* data)
{
			electromagnetic_type_t  performer;
	
			performer.request.devices=data->request.devices;
			performer.request.status=data->request.status;
	
	
			electromagnetic_control(performer.request.devices, performer.request.status);
			data->response.ret=0;
}	
static void protocol_connect_test(connect_test_type_t* data)
{
	
	data->response.ret =0;

}	

static void protocol_get_weight_current_gram(get_weight_current_gram_type_t* data)
{
	get_weight_current_gram_type_t  performer;
	
	performer.request.weight=data->request.weight;
	data->response.gram=Get_weight_current_gram(performer.request.weight);
	data->response.ret=0;
}

static void protocol_rest_select_motor_orgin(rest_select_motor_orgin_type_t* data)
{
	rest_select_motor_orgin_type_t  performer;
	performer.request.motorNum = data->request.motorNum;
	performer.request.lightNum = data->request.lightNum;
	performer.request.motorDir = data->request.motorDir;
	performer.request.steps = data->request.steps;
	performer.request.flag_wait = data->request.flag_wait;
	RestSelectMotorOrgin(performer.request.motorNum,performer.request.lightNum, (motorDir_t)performer.request.motorDir,performer.request.steps);
	data->response.ret=0;
}	

static void protocol_move_wait_motor(move_wait_motor_type_t* data)
{
	move_wait_motor_type_t performer;
	performer.request.data.array = data->request.data.array;
	performer.request.data.stepCount = data->request.data.stepCount;
	performer.request.data.direction = data->request.data.direction;
	Motor_Move_And_Wait(performer.request.data.array, performer.request.data.direction , performer.request.data.stepCount);
	data->response.ret=0;
}	

static void protocol_set_pumps100_press(Set_Pumps100_Press_type_t* data)
{
	Set_Pumps100_Press_type_t performer;	
	
	performer.request.MaxPress=data->request.MaxPress;
	performer.request.MinPress=data->request.MinPress;
	
	data->response.ret=Set_Pumps100_Press(performer.request.MaxPress, performer.request.MinPress);
	//data->response.ret=PumpHandle->setPress(performer.request.MaxPress, performer.request.MinPress);
}	

static void protocol_balance_chromatographic_column(Balance_Chromatographic_Column_type_t* data)
{
	Balance_Chromatographic_Column_type_t performer;	
	
	performer.request.IdleFlowSpeed=data->request.IdleFlowSpeed;
	
	data->response.ret=Balance_Chromatographic_Column(performer.request.IdleFlowSpeed);
}	

static void protocol_gradient_control_buffer(Gradient_control_buffer_type_t* data)
{
	Gradient_control_buffer_type_t performer;	
	
	performer.request.Work_Flow_Speed=data->request.Work_Flow_Speed;
	performer.request.A_timeS=data->request.A_timeS;
	performer.request.B_timeS=data->request.B_timeS;
	performer.request.C_timeS=data->request.C_timeS;
	performer.request.A2_timeS=data->request.A2_timeS;
	
	data->response.ret=Gradient_control_buffer(performer.request.Work_Flow_Speed, performer.request.A_timeS, performer.request.B_timeS,performer.request.C_timeS,performer.request.A2_timeS);
}

static void protocol_gravity_sensor_setting(Gravity_Sensor_Setting_type_t* data)
{
	Gravity_Sensor_Setting_type_t performer;
	
	performer.request.weightA=data->request.weightA;
	performer.request.weightB=data->request.weightB;
	performer.request.weightC=data->request.weightC;
	performer.request.weightD=data->request.weightD;
	
	Gravity_Sensor_Setting(performer.request.weightA,performer.request.weightB,performer.request.weightC,performer.request.weightD);
	
	data->response.ret=0;
}

static void protocol_gravity_sensor_getting(Gravity_Sensor_Getting_type_t* data)
{
		data->response.weightA=Get_weight_current_gram(WEIGHT_ONE);
		data->response.weightB=Get_weight_current_gram(WEIGHT_TWO);	
		data->response.weightC=Get_weight_current_gram(WEIGHT_THREE);
		data->response.weightD=Get_weight_current_gram(WEIGHT_FOUR);
		
		data->response.ret=0;
}

static void protocol_scan_barcode(scan_barcode_t* data)
{
		scan_barcode_t performer;
		performer.request.timeout = data->request.timeout;	
		int length=0;
		//Obtain_Barcode_String(data->response.string, &length, performer.request.timeout,false);
		//Scan_Bar_Action(data->response.string, &length, performer.request.timeout,false);
		ScanHandle->scan(data->response.string, &length, performer.request.timeout,false);
		data->response.ret=0;
	  data->response.length=length;
}
static void protocol_pump_interface(pump_type_t* data)
{
		pump_type_t performer;
		performer.request.type = data->request.type;
		performer.request.para = data->request.para;
		performer.request.para_nor = data->request.para_nor;
		data->response.value=pump_process_cmd(performer);
		data->response.ret=0;
}

static void  protocol_rest_injection_module_motor(rest_injection_module_motor_type_t* data)
{
	rest_injection_module_motor_type_t performer;
	performer.request.up_Steps=data->request.up_Steps;
	performer.request.big_Steps=data->request.big_Steps;
	performer.request.time=data->request.time;
	
	Rest_Injection_Module_Motor(performer.request.up_Steps,performer.request.big_Steps,performer.request.time);
	data->response.ret=0;
}

static void  protocol_get_liquid_sensor_level(get_liquid_sensor_level_t* data)
{
	get_liquid_sensor_level_t performer;
	data->response.value=Liquid_Sensor_Get();
	data->response.ret=0;
}
static void  protocol_pump_s100_set_flowSpeed(pump_s100_set_flowSpeed_type_t* data)
{
	pump_s100_set_flowSpeed_type_t performer;
	Set_Pumps100_FlowSpeed(data->request.flowSpeed);
	data->response.ret=0;
}

static void  protocol_rfid_init_config(rfid_init_config_type_t* data)
{
	rfid_init_config_type_t performer;
	performer.request.region=data->request.region;
	performer.request.ReadPowerdbm=data->request.ReadPowerdbm;
	performer.request.WritePowerdbm=data->request.WritePowerdbm;
	
	data->response.ret=Init_M6e_Config( (TMR_Region)(performer.request.region), performer.request.ReadPowerdbm, performer.request.WritePowerdbm);

	//data->response.ret=0;
}

static void  protocol_rfid_destory_config(rfid_destory_config_type_t* data)
{
	rfid_destory_config_type_t performer;
	Destory_M6e_Config();
	data->response.ret=0;
}

static void  protocol_rfid_send_read_command(rfid_send_read_command_type_t* data)
{
	rfid_send_read_command_type_t performer;
	data->response.ret=M6e_Magic_Read_Rfid_Info(&performer.response.length);
	data->response.length=performer.response.length;
}

static void  protocol_rfid_receive_index_tag(rfid_receive_index_tag_type_t* data)
{
	rfid_receive_index_tag_type_t performer;
	performer.request.index=data->request.index;
	
	if(performer.request.index>=10)data->response.ret=1;
	else {
		data->response.ret=0;
		data->response.tag=M6e_Magic_Get_One_Rfid_Info(performer.request.index);
	}
}

static void  protocol_rfid_get_epc_string(rfid_get_epc_string_type_t* data)
{
	  rfid_get_epc_string_type_t performer;
	
		static	char stringEPC[128];
		 int lengthEPC=0;
		Get_EPC_String(&lengthEPC, stringEPC);
		data->response.ret=0;
   	printf("str:%s\r\n",stringEPC);
	  memcpy(data->response.epc, stringEPC, lengthEPC);
		data->response.length=lengthEPC;
}

static void  protocol_rfid_write_epc(rfid_write_epc_type_t* data)
{
	  rfid_write_epc_type_t performer;
		printf("1111\r\n");
		performer.request.epcByteCount=data->request.epcByteCount;
		memcpy(performer.request.epcData, data->request.epcData, performer.request.epcByteCount);
		printf("1112\r\n");
	  data->response.ret=0;
		M6e_Magic_Write_Rfid_EPC(performer.request.epcData, performer.request.epcByteCount);
		printf("11113\r\n");
}

static void  protocol_rfid_write_blank(rfid_write_blank_type_t* data)
{
	  rfid_write_blank_type_t performer;
	
		performer.response.ret=M6e_Magic_Write_Rfid_Blank(data->request.wordCount,data->request.writeData);
		data->response.ret=performer.response.ret;
}


void protocol_handle_uart_powerstep01_plain_slave_cmd(void){
		uint8_t ret =0;
		int len=0;
		Powerstep1_contorl_motor_command_t slave_motorCommand;
		memset(&slave_motorCommand,0,sizeof(Powerstep1_contorl_motor_command_t));
		UART4_Receive_Data((u8*)(&slave_motorCommand),&len);
		
	  printf("*******%d \r\n",len);
	
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
			case GET_WEIGHT_CURRENT_GRAM:
						protocol_get_weight_current_gram(&slave_motorCommand.CommandPowerStep1.get_weight_current_gram);
						break;
			case REST_MOTOR_ORGIN:
						protocol_rest_select_motor_orgin(&slave_motorCommand.CommandPowerStep1.rest_select_motor_orgin);
						break;
			case MOTOR_MOVE_AND_WAIT:
						protocol_move_wait_motor(&slave_motorCommand.CommandPowerStep1.move_wait_motor);
						break;
			case SET_PUMPS100_PRESS:
						protocol_set_pumps100_press(&slave_motorCommand.CommandPowerStep1.Set_Pumps100_Press);
						break;
			case BALANCE_CHROMATOGRAPHIC_COLUMN:
						protocol_balance_chromatographic_column(&slave_motorCommand.CommandPowerStep1.Balance_Chromatographic_Column);
						break;
			case GRADIENT_CONTROL_BUFFER:
						protocol_gradient_control_buffer(&slave_motorCommand.CommandPowerStep1.Gradient_control_buffer);
						break;
			case GRAVITY_SENSOR_SETTING:
						protocol_gravity_sensor_setting(&slave_motorCommand.CommandPowerStep1.Gravity_Sensor_Setting);
						break;	
			case GRAVITY_SENSOR_GETTING:
						protocol_gravity_sensor_getting(&slave_motorCommand.CommandPowerStep1.Gravity_Sensor_Getting);
						break;	
			case SCAN_BARCODE:
						protocol_scan_barcode(&slave_motorCommand.CommandPowerStep1.scan);
						break;
			case PUMP_INTERFACE:
					protocol_pump_interface(&slave_motorCommand.CommandPowerStep1.pump);
						break;
			case REST_INJECTION_MODLUE_MOTOR:
					protocol_rest_injection_module_motor(&slave_motorCommand.CommandPowerStep1.rest_injection_module_motor);
				break;
			case LIQUID_SENSOR:
					protocol_get_liquid_sensor_level(&slave_motorCommand.CommandPowerStep1.get_liquid_sensor);
				break;
			case PUMPS100_SET_FLOWSPEED:
					protocol_pump_s100_set_flowSpeed(&slave_motorCommand.CommandPowerStep1.pump_s100_set_flowSpeed);
				break;
			case RFID_INIT:
					protocol_rfid_init_config(&slave_motorCommand.CommandPowerStep1.rfid_init_config);
					break;
			case RFID_DESTORY:
					protocol_rfid_destory_config(&slave_motorCommand.CommandPowerStep1.rfid_destory_config);
					break;
			case RFID_SEND_READ_COMMAND:
					protocol_rfid_send_read_command(&slave_motorCommand.CommandPowerStep1.rfid_send_read_command);
				break;
			case RFID_RECEIVE_INDEX_TAG:
					protocol_rfid_receive_index_tag(&slave_motorCommand.CommandPowerStep1.rfid_receive_index_tag);
				break;
			case RFID_GET_EPC_STRING:
					protocol_rfid_get_epc_string(&slave_motorCommand.CommandPowerStep1.rfid_get_epc_string);
				break;
			case RFID_WRITE_EPC:
					protocol_rfid_write_epc(&slave_motorCommand.CommandPowerStep1.rfid_write_epc);
				break;
			case RFID_WRITE_BLANK:
					protocol_rfid_write_blank(&slave_motorCommand.CommandPowerStep1.rfid_write_blank);
				break;

			
			default:
					printf("no found this cmd ! %d \r\n",slave_motorCommand.type);
		}
OVER:		
		UART4_Send_Data((u8*)(&slave_motorCommand),sizeof(Powerstep1_contorl_motor_command_t));
}



uint8_t Set_Pumps100_FlowSpeed(int speed)
{
	uint8_t ret=0;
	pump_s100_command_type_t data;
	
	uint8_t ge = speed%10;
	speed=speed/10;
	uint8_t shi = speed%10;
	speed=speed/10;
	uint8_t bai = speed%10;
	speed=speed/10;
	uint8_t qian = speed%10;

	data.request.timeout=1000;
	data.request.para.S100_STX=0x21;
	data.request.para.S100_ETX=0x0a;
	data.request.para.S100_ID[1]='1'; 
	data.request.para.S100_ID[0]='0';
	data.request.para.S100_AI='0';
	
	data.request.para.S100_PFC[1]='1';
	data.request.para.S100_PFC[0]='0';
	
	data.request.para.S100_VALUE[5]=0x20;//0x20
	data.request.para.S100_VALUE[4]=0x20;//0x20
	data.request.para.S100_VALUE[3]=0x30+qian;
	data.request.para.S100_VALUE[2]=0x30+bai;
	data.request.para.S100_VALUE[1]=0x30+shi;
	data.request.para.S100_VALUE[0]=0x30+ge;
	
	protocol_pump_s100_interface(&data);

	if(data.response.s100_reply.SpecialACK.S100_RESULT!='#')ret=1;

	return ret;
}


uint8_t Set_Pumps100_MaxPress(int press)
{
	uint8_t ret=0;
	pump_s100_command_type_t data;

	uint8_t ge = press%10;
	press=press/10;
	uint8_t shi = press%10;
	press=press/10;
	uint8_t bai = press%10;
	press=press/10;
	uint8_t qian = press%10;
	
	data.request.timeout=1000;
	data.request.para.S100_STX=0x21;
	data.request.para.S100_ETX=0x0a;
	data.request.para.S100_ID[1]='1'; 
	data.request.para.S100_ID[0]='0';
	data.request.para.S100_AI='0';
	
	data.request.para.S100_PFC[1]='1';
	data.request.para.S100_PFC[0]='3';
	
	data.request.para.S100_VALUE[5]=0x20;//0x20
	data.request.para.S100_VALUE[4]=0x20;//0x20
	data.request.para.S100_VALUE[3]=0x30+qian;
	data.request.para.S100_VALUE[2]=0x30+bai;
	data.request.para.S100_VALUE[1]=0x30+shi;
	data.request.para.S100_VALUE[0]=0x30+ge;
	
	protocol_pump_s100_interface(&data);
	if(data.response.s100_reply.SpecialACK.S100_RESULT!='#')ret=1;

	return ret;
}


uint8_t Set_Pumps100_MinPress(int press)
{
	uint8_t ret=0;
	pump_s100_command_type_t data;

	uint8_t ge = press%10;
	press=press/10;
	uint8_t shi = press%10;
	press=press/10;
	uint8_t bai = press%10;
	press=press/10;
	uint8_t qian = press%10;
	
	data.request.timeout=1000;
	data.request.para.S100_STX=0x21;
	data.request.para.S100_ETX=0x0a;
	data.request.para.S100_ID[1]='1'; 
	data.request.para.S100_ID[0]='0';
	data.request.para.S100_AI='0';
	
	data.request.para.S100_PFC[1]='1';
	data.request.para.S100_PFC[0]='4';
	
	data.request.para.S100_VALUE[5]=0x20;//0x20
	data.request.para.S100_VALUE[4]=0x20;//0x20
	data.request.para.S100_VALUE[3]=0x30+qian;
	data.request.para.S100_VALUE[2]=0x30+bai;
	data.request.para.S100_VALUE[1]=0x30+shi;
	data.request.para.S100_VALUE[0]=0x30+ge;
	
	protocol_pump_s100_interface(&data);
	if(data.response.s100_reply.SpecialACK.S100_RESULT!='#')ret=1;

	return ret;
}


uint8_t Set_Pumps100_Press(int MinPress,int MaxPress)
{
	uint8_t ret=0;
	if(Set_Pumps100_MinPress(MinPress))return 1;
	if(Set_Pumps100_MaxPress(MaxPress))return 1;
	return ret;
}


uint8_t Balance_Chromatographic_Column(int IdleFlowSpeed)
{
	uint8_t ret=0;
	
	cheminert_c52_c55_type_t cheminert_c52_c55;
	cheminert_c52_c55.request.para=	CHEMINERT_C52_CCA;
	cheminert_c52_c55.request.timeout=1000;
	protocol_cheminert_c52_c55(&cheminert_c52_c55);
	
	if(cheminert_c52_c55.response.buf[0]!='A')return 1;

	//ret=Set_Pumps100_FlowSpeed(IdleFlowSpeed);
	ret=PumpHandle->setFlowSpeed(IdleFlowSpeed);
	return ret;
}


uint8_t Gradient_control_buffer(int Work_Flow_Speed,int A_timeS,int B_timeS,int C_timeS,int A2_timeS)
{
	uint8_t ret=0;
	
	
	cheminert_c52_c55_type_t cheminert_c52_c55;
	cheminert_c52_c55.request.para=	CHEMINERT_C52_CCA;
	cheminert_c52_c55.request.timeout=1000;
	protocol_cheminert_c52_c55(&cheminert_c52_c55);
	
	if(cheminert_c52_c55.response.buf[0]!='A')return 1;

	ret=PumpHandle->setFlowSpeed(Work_Flow_Speed);

	electromagnetic_control(ELECTROMAGNETIC_A, OPEN_FT);
	delay_ms(1000*A_timeS);
	electromagnetic_control(ELECTROMAGNETIC_A, CLOSE_FT);
	
	electromagnetic_control(ELECTROMAGNETIC_B, OPEN_FT);
	delay_ms(1000*B_timeS);
	electromagnetic_control(ELECTROMAGNETIC_B, CLOSE_FT);
	
	electromagnetic_control(ELECTROMAGNETIC_C, OPEN_FT);
	delay_ms(1000*C_timeS);
	electromagnetic_control(ELECTROMAGNETIC_C, CLOSE_FT);
	
	electromagnetic_control(ELECTROMAGNETIC_A, OPEN_FT);
	delay_ms(1000*A2_timeS);
	electromagnetic_control(ELECTROMAGNETIC_A, CLOSE_FT);
	
	return ret;
}


uint8_t Rest_C55_C52_Position(void)
{
	uint8_t ret=0;

	cheminert_c52_c55_type_t cheminert_c52_c55;
	cheminert_c52_c55.request.timeout=1000;
	
	cheminert_c52_c55.request.para=	CHEMINERT_C52_CCA;
	protocol_cheminert_c52_c55(&cheminert_c52_c55);
	if(cheminert_c52_c55.response.buf[0]!='A')return 1;

	cheminert_c52_c55.request.para=	CHEMINERT_C55_CC2;
	protocol_cheminert_c52_c55(&cheminert_c52_c55);
	if(cheminert_c52_c55.response.buf[0]!='4')return 1;
	
	return ret;
}	

void Gravity_Sensor_Setting(int weightA,int weightB,int weightC,int weightD)
{
	Set_Weight_Sensor_Warnning_Line(WEIGHT_ONE , weightA);
	Set_Weight_Sensor_Warnning_Line(WEIGHT_TWO , weightB);
	Set_Weight_Sensor_Warnning_Line(WEIGHT_THREE , weightC);
	Set_Weight_Sensor_Warnning_Line(WEIGHT_FOUR , weightD);
}







/*************************************************************************************/
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






uint8_t Run_S100_Pump(void)
{
	u8 ret=0,i;
	pump_s100_command_type_t data;
	
	data.request.timeout=1000;
	
	data.request.para.S100_STX=0x21;
	data.request.para.S100_ETX=0x0a;
	
	data.request.para.S100_ID[1]='1'; 
	data.request.para.S100_ID[0]='0';
	
	data.request.para.S100_AI='0';
	
	data.request.para.S100_PFC[1]='1';
	data.request.para.S100_PFC[0]='5';
	
	data.request.para.S100_VALUE[5]=0x20;//0x20
	data.request.para.S100_VALUE[4]=0x20;//0x20
	data.request.para.S100_VALUE[3]=0x20;
	data.request.para.S100_VALUE[2]='0';
	data.request.para.S100_VALUE[1]='1';
	data.request.para.S100_VALUE[0]='0';
	
	protocol_pump_s100_interface(&data);
	
/*
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
*/	
	return data.response.ret;
}




uint8_t Stop_S100_Pump(void)
{
	u8 ret=0,i;
	pump_s100_command_type_t data;
	
	data.request.timeout=1000;
	
	data.request.para.S100_STX=0x21;
	data.request.para.S100_ETX=0x0a;
	
	data.request.para.S100_ID[1]='1'; 
	data.request.para.S100_ID[0]='0';
	
	data.request.para.S100_AI='0';
	
	data.request.para.S100_PFC[1]='1';
	data.request.para.S100_PFC[0]='6';
	
	data.request.para.S100_VALUE[5]=0x20;//0x20
	data.request.para.S100_VALUE[4]=0x20;//0x20
	data.request.para.S100_VALUE[3]=0x20;
	data.request.para.S100_VALUE[2]='0';
	data.request.para.S100_VALUE[1]='1';
	data.request.para.S100_VALUE[0]='0';
	
	protocol_pump_s100_interface(&data);
	
	/*
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
	*/
	return data.response.ret;
}


void Init_S100_Pump(void){}
int Read_S100_Pump_press(void){return 0;}	




void scan_test(void)
{
	int i;
	scan_barcode_t scan;
	scan.request.timeout=7;
	protocol_scan_barcode(&scan);
	
	printf("obtain len :%d \r\n",scan.response.length);
	for(i=0;i<scan.response.length;i++)
	{
		printf("%c",scan.response.string[i]);
	}
	printf("\r\n");

}	

void weight_test()
{
	Gravity_Sensor_Getting_type_t data;
	protocol_gravity_sensor_getting(&data);
	printf("A:%d /gram .\r\n",data.response.weightA);
	printf("B:%d /gram .\r\n",data.response.weightB);
	printf("C:%d /gram .\r\n",data.response.weightC);
	printf("D:%d /gram .\r\n",data.response.weightD);
}	
/**************************PUMP start************************************/

Pump_t* PumpHandle=0;

Pump_t s100=
{
	Init_S100_Pump,
	Run_S100_Pump,
	Stop_S100_Pump,
	Set_Pumps100_Press,
	Set_Pumps100_FlowSpeed,
	Read_S100_Pump_press,
	Set_Pumps100_MaxPress,
	Set_Pumps100_MinPress,
};

Pump_t s1125=
{
	S1125_Init,
	Run_S1125_Pump,
	Stop_S1125_Pump,
	Write_Press_s1125_pump,
	Write_FlowSpeed_s1125_pump,
	Read_Press_S1125_Pump,
	Write_MaxPress_s1125_pump,
	Write_MinPress_s1125_pump,
};

Pump_t* S100_GetHandle(void)
{
  return (&s100);
}

Pump_t* S1125_GetHandle(void)
{
  return (&s1125);
}

void PumpChooseHandle(PUMP_type id)
{

  if (id == S100)
  {
    PumpHandle = S100_GetHandle();
	}else if(id == S1125)
	{
		PumpHandle = S1125_GetHandle();
	}	
}



int pump_process_cmd(pump_type_t pump)
{
	int value=0;
	switch(pump.request.type)
	{
		case RUN_PUMP:
			PumpHandle->run();
			break;
	
		case STOP_PUMP:
			PumpHandle->stop();
			break;
		
		case READ_PRESS:
			value=PumpHandle->readPress();
			break;
		
		case WRITE_MAX_PRESS:
			PumpHandle->setMaxPress(pump.request.para);
			break;
		
		case WRITE_MIN_PRESS:
			PumpHandle->setMinPress(pump.request.para);
			break;
		
		case WRITE_FLOW_SPEED:
			PumpHandle->setFlowSpeed(pump.request.para);
			break;
		
		case WRITE_PRESS:
			PumpHandle->setPress(pump.request.para,pump.request.para_nor);
			break;
		
		default:
			break;
	}	

	return value;
}	
/**************************PUMP end************************************/



