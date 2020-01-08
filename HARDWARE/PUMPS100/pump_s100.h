#ifndef _PUMP_S100_H_INCLUDED
#define _PUMP_S100_H_INCLUDED
#include "stdint.h"	 
#include "stdio.h"	 
#include "sys.h"
#include "uart2.h"
#include "delay.h"
#include "uart_choose.h"
#include "config.h"
//1 need ------ 0 no need
#define NEED_BIG_LITTLE_ENDIAN_CONVERT 1

typedef struct{
		u8 S100_STX;
		u8 S100_ID[2];
		u8 S100_AI;
		u8 S100_PFC[2];
		u8 S100_VALUE[6];
		u8 S100_CRC[3];
		u8 S100_ETX;
}s100_command_t;

typedef enum Result_S100_type{ 
		NORMAL_ANSWER_S100=11,
		SPECIAL_ACK_S100,
		ACTIVE_EVENT_S100,
		UNKNOWN_ANSWER,
}PUMP_S100_REPLY_type_t;

typedef	union{
struct{
		uint8_t S100_AI;
		uint8_t S100_VALUE[6];
	  uint8_t nor[5];
}NormalAnswer;
struct{
		uint8_t S100_RESULT; 	
}SpecialACK;
struct{
		uint8_t S100_AI;
		uint8_t S100_VALUE[6];
		uint8_t S100_PFC[2];
}ActiveEvent;
}pump_s100_reply_type_t;

//pump s100
typedef union{ 
struct{
		s100_command_t para;
	  uint16_t timeout;
		uint8_t nor[10];
}request;
struct{
		uint8_t ret;
	  PUMP_S100_REPLY_type_t PUMP_S100_REPLY_type; 
		pump_s100_reply_type_t s100_reply;		
}response;
}pump_s100_command_type_t;


typedef union{ 
struct{
	  uint32_t flowSpeed;
}request;
struct{
		uint8_t ret;
		uint8_t nor[3];	
}response;
}pump_s100_set_flowSpeed_type_t;



//泵主动向STM32发送数据,STM32对数据进行储存，等待ARM读取，不要主动向ARM发送
//此主动行为，不需要回应泵

void Wait_Ack_Pump_S100_Event_And_Send_Master(void);

uint8_t pump_s100_transfer(s100_command_t*data, PUMP_S100_REPLY_type_t*type, pump_s100_reply_type_t* reply,u16 timeout);

#endif

