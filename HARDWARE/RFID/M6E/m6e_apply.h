#ifndef __M6E_APPLY_H
#define __M6E_APPLY_H

#include <tm_reader.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

typedef enum Chemical_Produce_Id
{
	Produce_SEPAX=0xB1,
	Produce_B,
}Chemical_Produce_Id_type;

typedef enum Chemical_Produce_Adress
{
	CHINA_SUZHOU=0xC1,
	CHINA_YANGZHOU=0xC2,
	USA_TALA=0xA1,
}Chemical_Produce_Adress_type;

typedef enum Chemical_Produce_Name
{
	BUFFER_A=0xB1,
	BUFFER_B=0xB2,
	BUFFER_C=0xB3,
	DILUENT=0xD1,
}Chemical_Produce_Name_type;

typedef enum Chemical_Produce_Used
{
	USED_EPC=((uint8_t)0x00),
	UNUSED_EPC=((uint8_t)0xAA),
}Chemical_Produce_Used_type;

/*
 *two type just for disguish, no differ
 */
typedef struct
{
	uint8_t year[2];
	uint8_t month;
	uint8_t day;
}Chemical_Produce_Period_Hex_type;

typedef struct
{
	uint8_t year[2];
	uint8_t month;
	uint8_t day;
}Chemical_Produce_Period_int_type;

typedef struct
{
	uint8_t produce_id;
	uint8_t produce_adress;
	uint8_t produce_name;
}Chemical_Produce_Info_type;


typedef struct Chemical_reagent_EPC{
	Chemical_Produce_Info_type Info;
	uint8_t validity_period[8];
	uint8_t random_sequence[6];
	uint8_t check_sequence[4];
	uint8_t used;
}Chemical_reagent_EPC_type;


#define SN_A "LSP23412"
#define SN_B "LSP18023"

#define BN_VOLUM_A 5000
#define BN_VOLUM_B 4000

#define RN_COUNT_A 10000
#define RN_COUNT_B 1000

typedef struct {
	uint16_t serial_number[4];
	uint16_t total_volume;
	uint16_t total_count;
}Chemical_reagent_user_Hex_type;
/*
 * double
 */
typedef struct {
	uint8_t serial_number[8];
	uint8_t total_volume[2];
	uint8_t total_count[2];
}Chemical_reagent_user_Char_type;


typedef struct Chemical_reagent{
	uint8_t used;//???????
	uint8_t error;//??????
	Chemical_reagent_EPC_type EPC;
	Chemical_reagent_user_Char_type USR;
}Chemical_reagent_type;


typedef struct {
	uint8_t used;
	uint8_t epcString[48];
	uint8_t epcStringCount;
	uint8_t userBlankData[16];
	uint8_t userBlankCount;
}Chemical_reagent_Info_type;

/*
*单片机只负责读取原始数据，之后分多次传给hal层，hal层再进行格式提取，涉及加密解密都放在hal层，这样做为了以后方便管理
*/

uint8_t Init_M6e_Config(TMR_Region region, int Rpowerdbm,int Wpowerdbm);

void Destory_M6e_Config(void);

//读RFID信息
uint8_t M6e_Magic_Read_Rfid_Info(int* length);
//读之后，一个个返回HAL
Chemical_reagent_Info_type M6e_Magic_Get_One_Rfid_Info(uint8_t index);
//获取EPC字符串用于验证连接性
uint8_t Get_EPC_String(int*length, char* epc);
//写EPC信息	
uint8_t M6e_Magic_Write_Rfid_EPC(uint8_t* epcData,uint8_t epcByteCount);
//写BLANK信息
uint8_t M6e_Magic_Write_Rfid_Blank(uint8_t wordCount,uint16_t* writeData);


typedef union{
struct{
		uint8_t region;
		int ReadPowerdbm;
		int WritePowerdbm;
}request;
struct{
		uint8_t ret;
}response;
}rfid_init_config_type_t;

typedef union{
struct{
	  uint8_t nor;
}request;
struct{
		uint8_t ret;
}response;
}rfid_destory_config_type_t;

typedef union{
struct{
	  uint8_t nor;
}request;
struct{
		int length;
		uint8_t ret;
}response;
}rfid_send_read_command_type_t;

typedef union{
struct{
	  uint8_t index;
}request;
struct{
		uint8_t ret;
		Chemical_reagent_Info_type tag;
}response;
}rfid_receive_index_tag_type_t;

typedef union{
struct{
	  uint8_t nor;
}request;
struct{
		uint8_t ret;
		int  length; 
		char epc[60];
}response;
}rfid_get_epc_string_type_t;

typedef union{
struct{
	  uint8_t* epcData;
	  uint8_t epcByteCount;
}request;
struct{
		uint8_t ret;
}response;
}rfid_write_epc_type_t;

typedef union{
struct{
		uint8_t wordCount;
		uint16_t* writeData;
}request;
struct{
		uint8_t ret;
}response;
}rfid_write_blank_type_t;

#endif



