#include "master_uart_control_interface.h"

uint8_t master_powerStep01_move_command(move_type_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = MOVE_TYPE;
		master_motorCommand.CommandPowerStep1.move=data;
		printf("start master_powerStep01_move_command \r\n");	
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.move.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_move_command \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.power.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_power_command \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.rest_post.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_rest_pos_command \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.send_and_wait.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_send_command_and_wait_no_busy \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.one_device_move.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_one_device_move \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.one_device_wait.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_one_device_wait \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.one_device_get_pos.response.ret;
		}else{
				ret = 1;//fail
		}
		*pos=master_motorCommand.CommandPowerStep1.one_device_get_pos.response.pos; 
		printf("end master_powerStep01_one_device_get_pos \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.one_device_set_mark.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_one_device_set_mark \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.get_para.response.ret;
		}else{
				ret = 1;//fail
		}
		*para=master_motorCommand.CommandPowerStep1.get_para.response.result_para;
		printf("end master_powerStep01_get_para \r\n");
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
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.set_para.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_set_para \r\n");
		return ret;
}

uint8_t master_powerStep01_select_step_mode(select_step_mode_t data){
		uint8_t ret =0;
		u8 len =0;
		u8 times =100;
		Powerstep1_contorl_motor_command_t master_motorCommand;
		master_motorCommand.type = SElECT_STEP_MODE;
		master_motorCommand.CommandPowerStep1.select_step_mode=data;
		printf("start master_powerStep01_select_step_mode \r\n");	
		
		RS485_Send_Data(&master_motorCommand,sizeof(Powerstep1_contorl_motor_command_t));
		//HAL_Delay(1000);
		delay_ms(10);//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束  ---slave 应该在10ms内向master反馈
		//最多等待1分钟
		while(1){
			RS485_Receive_Data(&master_motorCommand,&len);
			times--;
			if(len>=0||times ==0)break;
			delay_ms(10);
		}
		//len值为0的时候，从设备没有反馈
		if(len==0){
				ret = master_motorCommand.CommandPowerStep1.select_step_mode.response.ret;
		}else{
				ret = 1;//fail
		}
		printf("end master_powerStep01_select_step_mode \r\n");
		return ret;
}

