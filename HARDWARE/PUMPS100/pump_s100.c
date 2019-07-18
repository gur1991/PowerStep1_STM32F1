#include "pump_s100.h"
#include "uart_command_control_protocol.h"

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
//�˴�Ҫע����ǣ�����ʱ��STM32�������ݣ�����յ����Ǳ��������͵��¼���ǡ�ô�ʱ�����ȴ���������Ķ������ܵ�
//����������ʷǳ�С������������������STM32��������¼��ش���ARM�ˣ�ARM���������������Ѵ�������ִ��һ��
//���յ��������¼����������ѡ���Ƿ�������߶�Ӧ������ΪARM��Ҳ�����ݽ������������¼���
uint8_t pump_s100_transfer(s100_command_t*data, PUMP_S100_REPLY_type_t*type, pump_s100_reply_type_t* reply,u16 timeout){
		uint8_t ret=0;
		u8 result;
    int len, i;
		s100_command_t S100_receive;
		u8 rs485buf[16];
	
		CRC_Digital_Convert_Get(data);
		Big_Little_Endian_Convert(data->S100_CRC,sizeof(data->S100_CRC));
		Big_Little_Endian_Convert(data->S100_PFC,sizeof(data->S100_PFC));
		Big_Little_Endian_Convert(data->S100_ID,sizeof(data->S100_ID));
		Big_Little_Endian_Convert(data->S100_VALUE,sizeof(data->S100_VALUE));
	/*
		printf("S100_STX: %x\r\n",data->S100_STX);
		printf("S100_ETX: %x\r\n",data->S100_ETX);
		printf("ID[1]: %c\r\n",data->S100_ID[1]);
		printf("ID[0]: %c\r\n",data->S100_ID[0]);	
		printf("AI: %c\r\n",data->S100_AI);
		printf("PFC[1]: %c\r\n",data->S100_PFC[1]);
		printf("PFC[0]: %c\r\n",data->S100_PFC[0]);
		printf("CRC[2]: %c\r\n",data->S100_CRC[2]);	
		printf("CRC[1]: %c\r\n",data->S100_CRC[1]);	
		printf("CRC[0]: %c\r\n",data->S100_CRC[0]);
									
		for(i=0;i<6;i++){
			printf("value:%x\r\n",data->S100_VALUE[i]);
		}
		*/
									
		UART2_Send_Data((u8*)data,sizeof(s100_command_t));
		while(1){
			 		if(!timeout){
						ret=1;
						*type=UNKNOWN_ANSWER;	
						printf(" pump_s100_transfer fuck timeout! \r\n");
						break;
					}
					/*
					delay_ms(200);
					RS485_Receive_Data(rs485buf,&len);
					for(i=0;i<len;i++){
							printf("%c",rs485buf[i]);
					}
					*/
					if(FLAG_RECEIVE_ACK){
									UART2_Receive_Data(&result,&len);
						
									*type=SPECIAL_ACK_S100;
									reply->SpecialACK.S100_RESULT=result;
						
									FLAG_RECEIVE_ACK=0;
									break;
					}
					if(FLAG_UART_MASTER){	
									UART2_Receive_Data((u8*)(&S100_receive),&len);
									Big_Little_Endian_Convert(S100_receive.S100_PFC,sizeof(S100_receive.S100_PFC));	
									Big_Little_Endian_Convert(S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
									
									//s100 active event 
									if(S100_receive.S100_PFC[1]=='9'&&(S100_receive.S100_PFC[0]=='0'||S100_receive.S100_PFC[0]=='2'||S100_receive.S100_PFC[0]=='3'))
									{
											printf(" pump_s100_transfer ACTIVE_EVENT_S100! \r\n");
											*type=ACTIVE_EVENT_S100;
											reply->ActiveEvent.S100_AI=S100_receive.S100_AI;
											memcpy(reply->ActiveEvent.S100_VALUE,S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
										  memcpy(reply->ActiveEvent.S100_PFC,S100_receive.S100_PFC,sizeof(S100_receive.S100_PFC));
									}else{
											*type=NORMAL_ANSWER_S100;
											reply->NormalAnswer.S100_AI=S100_receive.S100_AI;
											memcpy(reply->NormalAnswer.S100_VALUE,S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
									}
									FLAG_UART_MASTER=0;
									break;
								}
								timeout--;
								delay_ms(1);
						}
	
		return ret;
}

//���º�����Ҫarm��stm32��Ҫ���д���ſ��ԣ�������ʾ��
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
//��������STM32�������ݣ�STM32������ȡ��Ҫ���ݷ��͸�ARM��ARM�յ����ݽ��ж�Ӧ����
//��������Ϊ������Ҫ��Ӧ��
void Wait_Ack_Pump_S100_Event_And_Send_Master(void){
		uint8_t ret=0;
		u8 result;
    int len, i;
		s100_command_t S100_receive;
		Powerstep1_contorl_motor_command_t CommandData;
			
		CommandData.type = PUMP_S100_TYPE;
		CommandData.OverReceiveFlag[0]=OVER_UART_VALUE0;
		CommandData.OverReceiveFlag[1]=OVER_UART_VALUE1;
		
		if(FLAG_UART_MASTER){
					UART2_Receive_Data((u8*)(&S100_receive),&len);
					Big_Little_Endian_Convert(S100_receive.S100_PFC,sizeof(S100_receive.S100_PFC));	
					Big_Little_Endian_Convert(S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
									
					//s100 active event 
					if(S100_receive.S100_PFC[1]=='9'&&(S100_receive.S100_PFC[0]=='0'||S100_receive.S100_PFC[0]=='2'||S100_receive.S100_PFC[0]=='3'))
					{
								printf(" Wait_Ack_Pump_S100_Event_And_Send_Master ACTIVE_EVENT_S100! \r\n");
								CommandData.CommandPowerStep1.pump_s100_command.response.PUMP_S100_REPLY_type=ACTIVE_EVENT_S100;
								CommandData.CommandPowerStep1.pump_s100_command.response.ret=0;
								CommandData.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_AI=S100_receive.S100_AI;
								memcpy(CommandData.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_PFC,S100_receive.S100_PFC,sizeof(S100_receive.S100_PFC));
								memcpy(CommandData.CommandPowerStep1.pump_s100_command.response.s100_reply.ActiveEvent.S100_VALUE,S100_receive.S100_VALUE,sizeof(S100_receive.S100_VALUE));
								
								UART2_Send_Data((u8*)(&CommandData),sizeof(Powerstep1_contorl_motor_command_t));
					}
					
					FLAG_UART_MASTER=0;//�����¼���ȥ����
				}
		
}
