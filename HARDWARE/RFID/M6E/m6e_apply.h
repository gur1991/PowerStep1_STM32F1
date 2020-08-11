#ifndef __M6E_APPLY_H
#define __M6E_APPLY_H

#include "tm_reader.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include "rfid.h"


typedef struct {
	uint8_t used;
	uint8_t epcString[48];
	uint8_t epcStringCount;
	//uint8_t userBlankData[16];
	//uint8_t userBlankCount;
}Chemical_reagent_Info_type;

/*
*单片机只负责读取原始数据，之后分多次传给hal层，hal层再进行格式提取，涉及加密解密都放在hal层，这样做为了以后方便管理
*/

uint32_t Init_M6e_Config(TMR_Region region, uint32_t Rpowerdbm,uint32_t Wpowerdbm, uint8_t type);

void Destory_M6e_Config(void);

//读RFID信息
uint32_t M6e_Magic_Read_Rfid_Info(uint8_t* length);
//读之后，一个个返回HAL
Chemical_reagent_Info_type M6e_Magic_Get_One_Rfid_Info(uint8_t index);
//获取EPC字符串用于验证连接性
uint32_t Get_EPC_String(uint8_t*length, uint8_t* epc);
//写EPC信息	
uint32_t M6e_Magic_Write_Rfid_EPC(uint8_t* epcData,uint8_t epcByteCount);
//写BLANK信息
uint32_t M6e_Magic_Write_Rfid_Blank(uint8_t wordCount,uint16_t* writeData);

uint32_t M6e_Read_Info(void);


void Protocol_Output_Report(Chemical_reagent_Info_type info);
void Process_Regent_Info(Sepax_EPC_Type_T Sepc);
void Process_Filiter_Info(Sepax_EPC_Type_T Sepc);
void Process_Colunm_Info(Sepax_EPC_Type_T Sepc);
void Process_Date_Info(Sepax_EPC_Type_T Sepc);
	
enum{
	ONLY_FIRST=1,
	ONLY_SECOND,
	TWO_ALL,
	TWO_NONE
};


typedef union{
struct{
		uint8_t region;
		uint32_t ReadPowerdbm;
		uint32_t WritePowerdbm;
	  uint8_t RfidType;
}request;
struct{
		uint32_t RetM6e;
		uint8_t ret;
		uint8_t nor[11];
}response;
}rfid_init_config_type_t;

typedef union{
struct{
	  uint8_t nor[4];
}request;
struct{
		uint8_t ret;
}response;
}rfid_destory_config_type_t;

typedef union{
struct{
	  uint8_t nor[60];
}request;
struct{
		uint32_t RetM6e;
		uint8_t length;
		uint8_t ret;
}response;
}rfid_send_read_command_type_t;

typedef union{
struct{
	  uint8_t index;
		uint8_t nor[59];
}request;
struct{
		uint32_t RetM6e;
		uint8_t ret;
		Chemical_reagent_Info_type tag;
}response;
}rfid_receive_index_tag_type_t;

typedef union{
struct{
	  uint8_t nor[60];
}request;
struct{
		uint32_t RetM6e;
		uint8_t ret;
		uint8_t  length; 
		uint8_t epc[48];
}response;
}rfid_get_epc_string_type_t;

typedef union{
struct{
	  uint8_t epcData[48];
	  uint8_t epcByteCount;
		uint8_t nor[3];
}request;
struct{
		uint32_t RetM6e;
		uint8_t ret;
}response;
}rfid_write_epc_type_t;

typedef union{
struct{
	uint16_t writeData[8];	
	uint8_t wordCount;
	uint8_t nor[3];
}request;
struct{
		uint32_t RetM6e;
		uint8_t ret;
}response;
}rfid_write_blank_type_t;

#endif



