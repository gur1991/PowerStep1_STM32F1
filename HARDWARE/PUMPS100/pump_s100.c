#include "pump_s100.h"
#include "uart_command_control_protocol.h"
#include "config.h"

void Big_Little_Endian_Convert(u8*array,u8 array_size){
		u8 tmp_data[128];
		u8 i;
#if	NEED_BIG_LITTLE_ENDIAN_CONVERT
		for(i=0;i<array_size;i++){
				tmp_data[i]=array[array_size-1-i];
		}
		memcpy(array,tmp_data,array_size);
#endif
}

void CRC_Digital_Convert_Get(s100_command_t*s100_para){
		u8 gewei, shiwei ,baiwei;
		u16 sum;
		u8 mod;
	
	  sum=s100_para->S100_STX+s100_para->S100_ID[0]+s100_para->S100_ID[1]+s100_para->S100_AI+s100_para->S100_PFC[0]+s100_para->S100_PFC[1]+
				s100_para->S100_VALUE[0]+s100_para->S100_VALUE[1]+s100_para->S100_VALUE[2]+s100_para->S100_VALUE[3]+s100_para->S100_VALUE[4]+s100_para->S100_VALUE[5];
		mod=sum%256;
	
		gewei=mod%10;
	  shiwei=(mod%100)/10;
	  baiwei= mod/100;
		s100_para->S100_CRC[0]=gewei+48;
	  s100_para->S100_CRC[1]=shiwei+48;
		s100_para->S100_CRC[2]=baiwei+48;
		
}
/*
typedef struct{
		u8 S100_STX;
		u8 S100_ID[2];
		u8 S100_AI;
		u8 S100_PFC[2];
		u8 S100_VALUE[6];
		u8 S100_CRC[3];
		u8 S100_ETX;
}s100_command_t;

*/
//此处要注意的是，泵随时向STM32发送数据，如果收到的是泵主动发送的事件，恰好此时，被等待正常命令的动作接受到
//此种情况机率非常小，如果出现这种情况，STM32会把主动事件回传给ARM端，ARM端收是这种情况会把此命令再执行一次
//接收到的主动事件，根据情况选择是否放弃或者对应处理，因为ARM端也怕数据接收有误（主动事件）
uint8_t pump_s100_transfer(s100_command_t*data, PUMP_S100_REPLY_type_t*type, pump_s100_reply_type_t* reply,u16 timeout){
		uint8_t ret=0;
		u8 result;
    int len, i;
		s100_command_t S100_receive;
		u8 rs485buf[16];
		Uart_Receive_Data R232_Read = GetUartReceive( PUMP_UART_PORT, PUMP_UART_CS);
		Uart_Send_Data R232_Write = GetUartSend( PUMP_UART_PORT, PUMP_UART_CS);

	
	
		CRC_Digital_Convert_Get(data);
		Big_Little_Endian_Convert(data->S100_CRC,sizeof(data->S100_CRC));
		Big_Little_Endian_Convert(data->S100_PFC,sizeof(data->S100_PFC));
		Big_Little_Endian_Convert(data->S100_ID,sizeof(data->S100_ID));
		Big_Little_Endian_Convert(data->S100_VALUE,sizeof(data->S100_VALUE));
									
		R232_Write((u8*)data,sizeof(s100_command_t));
		while(1){
			 		if(!timeout){
						ret=1;
						*type=UNKNOWN_ANSWER;	
						LOGD(" pump_s100_transfer fuck timeout! \r\n");
						break;
					}
					if(FLAG_RECEIVE_ACK_PUMP100){
									delay_ms(10);
									R232_Read(&result,&len);
						
									*type=SPECIAL_ACK_S100;
									reply->SpecialACK.S100_RESULT=result;
						
									FLAG_RECEIVE_ACK_PUMP100=0;
									break;
					}
					if(FLAG_RECEIVE_ANSOWER_PUMP100){	
									delay_ms(10);
									R232_Read((u8*)(&S100_receive),&len);
									Big_Little_Endian_Convert(S100_receive.S100_PFC,sizeof(S100_receive.S100_PFC));	
									Big_Little_Endian_Convert(S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
									
									//s100 active event 
									if(S100_receive.S100_PFC[1]=='9'&&(S100_receive.S100_PFC[0]=='0'||S100_receive.S100_PFC[0]=='2'||S100_receive.S100_PFC[0]=='3'))
									{
											LOGD(" pump_s100_transfer ACTIVE_EVENT_S100! \r\n");
											*type=ACTIVE_EVENT_S100;
											reply->ActiveEvent.S100_AI=S100_receive.S100_AI;
											memcpy(reply->ActiveEvent.S100_VALUE,S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
										  memcpy(reply->ActiveEvent.S100_PFC,S100_receive.S100_PFC,sizeof(S100_receive.S100_PFC));
									}else{
											*type=NORMAL_ANSWER_S100;
											reply->NormalAnswer.S100_AI=S100_receive.S100_AI;
											memcpy(reply->NormalAnswer.S100_VALUE,S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
									}
									FLAG_RECEIVE_ANSOWER_PUMP100=0;
									break;
								}
								timeout--;
								delay_ms(1);
						}
	
		return ret;
}

//以下函数需要arm和stm32都要进行处理才可以，以下是示例
/*
//stm32
int main(void){
    uart_init(9600);
		......
		while(1){
				Wait_Ack_Pump_S100_Event_And_Send_Master();
				delay_ms(10);
				.......
		}
}
		
//arm

static pthread_t gWait_slave_msg;
pthread_create(&gWait_slave_msg,NULL,Wait_slave_msg_thread,NULL);

static void* Wait_slave_msg_thread(void*p){
			Powerstep1_contorl_motor_command_t RCommandData;
			u8 len;
			while(1){
						if(FLAG_UART_SLAVE_PUMP){
										RS485_Receive_Data((u8*)(&RCommandData),len);
										FLAG_UART_SLAVE_PUMP=0;
										.......
						}
				}
}



*/
//泵主动向STM32发送数据,STM32对数据进行储存，等待ARM读取，不要主动向ARM发送
//此主动行为，不需要回应泵
void Wait_Ack_Pump_S100_Event_And_Send_Master(void)
{
		uint8_t ret=0;
		u8 result;
    int len, i;
		s100_command_t S100_receive;
		Powerstep1_contorl_motor_command_t CommandData;
		Uart_Receive_Data R232_Read = GetUartReceive( PUMP_UART_PORT, PUMP_UART_CS);
		Uart_Send_Data R232_Write = GetUartSend( PUMP_UART_PORT, PUMP_UART_CS);
	
		CommandData.type = PUMP_S100_TYPE;
		CommandData.OverReceiveFlag[0]=OVER_UART_VALUE0;
		CommandData.OverReceiveFlag[1]=OVER_UART_VALUE1;
		
		//LOGD("start .\r\n");
		if(FLAG_RECEIVE_ANSOWER_PUMP100){
					R232_Read((u8*)(&S100_receive),&len);
					Big_Little_Endian_Convert(S100_receive.S100_PFC,sizeof(S100_receive.S100_PFC));	
					Big_Little_Endian_Convert(S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
									
					//s100 active event 
					if(S100_receive.S100_PFC[1]=='9'&&(S100_receive.S100_PFC[0]=='0'||S100_receive.S100_PFC[0]=='2'||S100_receive.S100_PFC[0]=='3'))
					{
								LOGD(" Wait_Ack_Pump_S100_Event_And_Send_Master ACTIVE_EVENT_S100! \r\n");
								CommandData.CommandPowerStep1.pump_s100_command.response.PUMP_S100_REPLY_type=ACTIVE_EVENT_S100;
								CommandData.CommandPowerStep1.pump_s100_command.response.ret=0;
								CommandData.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_AI=S100_receive.S100_AI;
								memcpy(CommandData.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_PFC,S100_receive.S100_PFC,sizeof(S100_receive.S100_PFC));
								memcpy(CommandData.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_VALUE,S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
								
								//R232_Write((u8*)(&CommandData),sizeof(Powerstep1_contorl_motor_command_t));
					}
					
					FLAG_RECEIVE_ANSOWER_PUMP100=0;//其他事件不去处理
				}
				//LOGD("end .\r\n");
		
}
