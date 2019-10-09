#ifndef __BL180_H
#define __BL180_H
#include <stdio.h>
#include <string.h>
#include "uart2.h"
//#include "uart3.h"
#include "delay.h"
#include <stdbool.h>
#include "uart_choose.h"
#include "config.h"
#include "fm100.h"

void Init_BL180(bool status);

int Scan_Bar_Action(u8* string,int* length, int TimeOut_S,bool check);


typedef struct
{
	void (*init)(bool status);
  int (*scan)(u8* string,int* length, int TimeOut_S,bool check);
}Scan_t;	

typedef enum{
	FM100=0,
	BL180,
}SCAN_type;

extern Scan_t* ScanHandle;

void ScanChooseHandle(SCAN_type id);
#endif




