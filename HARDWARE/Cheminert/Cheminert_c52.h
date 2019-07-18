#ifndef _CHEMINERT_C52_H_INCLUDED
#define _CHEMINERT_C52_H_INCLUDED
#include "stdint.h"	 
#include "stdio.h"	 
#include "sys.h"
#include "uart2.h"
#include "delay.h"
#include <stdbool.h>

u8 cheminert_c52_c55_transfer(u8*tx_buf,u8 tx_size,u8*rx_buf, u8*rx_size,uint16_t timeout,bool wait_flag);

typedef enum Command_Cheminert_type{ 
		CHEMINERT_C52_CP=25,//actuator 
		CHEMINERT_C52_CCA,
		CHEMINERT_C52_CCB,
		CHEMINERT_C52_CWA,
		CHEMINERT_C52_CWB,
		CHEMINERT_C52_DT,
		CHEMINERT_C52_GOA,
		CHEMINERT_C52_GOB,
		CHEMINERT_C52_MD,
		CHEMINERT_C52_SB,
		CHEMINERT_C52_SN,
		CHEMINERT_C52_TO,
		CHEMINERT_C52_VR,
	
		CHEMINERT_C55_CP,
		CHEMINERT_C55_CC1,
		CHEMINERT_C55_CC2,
		CHEMINERT_C55_CC3,
		CHEMINERT_C55_CC4,
		CHEMINERT_C55_CC5,
		CHEMINERT_C55_CC6,
		CHEMINERT_C55_CW1,
		CHEMINERT_C55_CW2,
		CHEMINERT_C55_CW3,
		CHEMINERT_C55_CW4,
		CHEMINERT_C55_CW5,
		CHEMINERT_C55_CW6,
		CHEMINERT_C55_GH,
		CHEMINERT_C55_GO1,
		CHEMINERT_C55_GO2,
		CHEMINERT_C55_GO3,
		CHEMINERT_C55_GO4,
		CHEMINERT_C55_GO5,
		CHEMINERT_C55_GO6,
		CHEMINERT_C55_MD,
		CHEMINERT_C55_MN,
		CHEMINERT_C55_SB,
		CHEMINERT_C55_SD,
		CHEMINERT_C55_SDCC,
		CHEMINERT_C55_SDCW,
		CHEMINERT_C55_SN,
		CHEMINERT_C55_VR,
		
}Command_Cheminert_type_t;

//actuator
typedef union{ 
struct{
		Command_Cheminert_type_t para;
	  uint16_t timeout;
}request;
struct{
		uint8_t ret;
		uint8_t size;
		uint8_t buf[64];
}response;
}cheminert_c52_c55_type_t;


#endif

