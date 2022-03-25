#ifndef __FM100_H
#define __FM100_H
#include <stdio.h>
#include <string.h>
#include "uart2.h"
#include "delay.h"
#include <stdbool.h>
#include "uart_choose.h"
#include "config.h"
/*
*status true/false  beeper open/close
*/
uint32_t scan_connect_test(void);


void Init_Scan_FM100(bool status);
/*
*TimeOut_S 扫描时间
返回字符串指针和长度
check true/false    one/two
*/
int Obtain_Barcode_String(u8* string,int* length, int TimeOut_S	,bool check);


typedef union{ 
struct{
		uint32_t timeout;
}request;
struct{
		uint8_t length;
		uint8_t string[32];
		uint8_t ret;
	  uint8_t state;
	  uint8_t nor;
}response;
}scan_barcode_t;

#endif


