#ifndef _S1125_H
#define _S1125_H
#include "stdint.h"	 
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include "uart2.h"
#include "delay.h"
#include <stdbool.h>
#include "uart_choose.h"
#include "spi1.h"
#include "uart_rts_control.h"
#include "real_time_polling.h"

typedef struct S1125_protocl
{
	u8 start;
	u8 device[4];
	u8 address[4];
	u8 value[4];
	u8 check[2];
	u8 end[2];
}S1125_protocl_type;



uint8_t Start_S1125_Purge(void);

uint8_t Stop_S1125_Purge(void);

uint8_t Run_S1125_Pump(void);

uint8_t Stop_S1125_Pump(void);

void S1125_Init(void);

int Read_Press_S1125_Pump(void);

uint8_t Write_MaxPress_s1125_pump(int MaxPress);

uint8_t Write_MinPress_s1125_pump(int MinPress);

uint8_t Write_FlowSpeed_s1125_pump(int SpeedFlow);

uint8_t Write_Press_s1125_pump(int MinPress, int MaxPress);

uint8_t Connect_S1125_Pump(void);
typedef union{ 
struct{
		uint8_t type;
		uint32_t  para;
		uint32_t  para_nor;
		uint8_t nor[3];
}request;
struct{
		uint8_t ret;
		uint32_t value;
}response;
}pump_type_t;

#endif

