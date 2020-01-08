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
void Init_Scan_FM100(bool status);
/*
*TimeOut_S ɨ��ʱ��
�����ַ���ָ��ͳ���
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
	  uint8_t nor[2];
}response;
}scan_barcode_t;

#endif


