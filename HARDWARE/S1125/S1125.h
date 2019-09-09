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
#include "config.h"
#include "spi1.h"
typedef struct S1125_protocl
{
	u8 start;
	u8 device[4];
	u8 address[4];
	u8 value[4];
	u8 check[2];
	u8 end[2];
}S1125_protocl_type;

int Run_S1125_Pump(void);

int Stop_S1125_Pump(void);

void S1125_Init(void);

int Read_Press_S1125_Pump(void);

int Write_MaxPress_s1125_pump(int MaxPress);

int Write_MinPress_s1125_pump(int MinPress);

int Write_FlowSpeed_s1125_pump(int SpeedFlow);

#endif

