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
		BSP_MotorControl_CmdResetPos(performer.request.devices);//此函数不连接硬件阻塞
		printf("slave devices2 %d \r\n",performer.request.devices);
		data->response.ret=0;
}

static void protocol_powerstep01_send_command_and_wait_no_busy(send_command_and_wait_no_busy_type_t* data){
		send_command_and_wait_no_busy_type_t performer;
		
		printf("slave wait  \r\n"); 
		BSP_MotorControl_SendQueuedCommands();
    /* Wait for all device ends moving */ 
    BSP_MotorControl_WaitForAllDevicesNotBusy();//此函数不连接硬件阻塞
		printf("slave busy  \r\n");
		data->response.ret=0;	
}

static void protocol_powerstep01_one_device_move(one_device_move_type_t* data){
		one_device_move_type_t performer;
		
		performer.request.devices = data->request.devices;
		performer.request.steps = data->request.steps;
	
		printf("slave devices %d \r\n",performer.request.devices);

		//此函数不连接电机会阻塞
		//BSP_MotorControl_GoTo(performer.request.devices,performer.request.steps);
		printf("slave steps %d \r\n",performer.request.steps);	
		data->response.ret=0;
		
}

static void protocol_powerstep01_one_device_wait(one_device_wait_type_t* data){
		one_device_wait_type_t performer;
		performer.request.devices=data->request.devices;
		printf("slave devices %d \r\n",performer.request.devices);
		BSP_MotorControl_WaitWhileActive(performer.request.devices);//此函数不连接硬件阻塞
		printf("slave devices2 %d \r\n",performer.request.devices);
		data->response.ret=0;
}

static void protocol_powerstep01_one_device_get_pos(one_device_get_pos_type_t*data){
			one_device_get_pos_type_t performer;
			performer.request.devices=data->request.devices;
			printf("slave devices %d \r\n",performer.request.devices);
			data->response.pos=BSP_MotorControl_GetPosition(performer.request.devices);//此函数不连接硬件阻塞
	
			data->response.pos=876;
			printf("slave devices2 %d \r\n",performer.request.devices);
		  data->response.ret=0;
}
static void protocol_powerstep01_one_device_set_mark(one_device_set_mark_type_t*data){
			one_device_set_mark_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.pos=data->request.pos;
	
			printf("slave devices %d \r\n",performer.request.devices);
			BSP_MotorControl_SetMark(performer.request.devices, performer.request.pos);//此函数不连接硬件阻塞
			printf("slave pos %d \r\n",performer.request.pos);
			data->response.ret=0;
}


static void protocol_powerstep01_get_para(get_para_type_t*data){
			get_para_type_t performer;
			performer.request.devices=data->request.devices;
			performer.request.para=data->request.para;
			
			printf("slave devices %d \r\n",performer.request.devices);
			//此函数不连接硬件阻塞
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
	//此函数不连接硬件阻塞
			BSP_MotorControl_CmdSetParam(performer.request.devices,performer.request.para,performer.request.value);
			printf("slave para %d \r\n",performer.request.para);
			data->response.ret=0;
}
static void protocol_powerstep01_select_step_mode(select_step_mode_t*data){
			select_step_mode_t performer;
			performer.request.devices=data->request.devices;
			performer.request.StepMode=data->request.StepMode;
			
			printf("slave devices %d \r\n",performer.request.devices);
	//此函数不连接硬件阻塞
			BSP_MotorControl_SelectStepMode(performer.request.devices, performer.request.StepMode);
			printf("slave StepMode %d \r\n",performer.request.StepMode);
			data->response.ret=0;
}

//select_step_mode_t
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
			case SElECT_STEP_MODE:
						protocol_powerstep01_select_step_mode(&slave_motorCommand.CommandPowerStep1.select_step_mode);
						break;
			default:
					printf("no found this cmd ! \r\n");
		}
		UART3_Send_Data((u8*)(&slave_motorCommand),sizeof(Powerstep1_contorl_motor_command_t));
		printf("end slave uart\r\n");
}




