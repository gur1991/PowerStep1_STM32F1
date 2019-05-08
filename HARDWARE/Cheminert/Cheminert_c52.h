#ifndef _CHEMINERT_C52_H_INCLUDED
#define _CHEMINERT_C52_H_INCLUDED
#include "stdint.h"	 
#include "stdio.h"	 
#include "sys.h"
#include "rs485.h"
#include "delay.h"
#include <stdbool.h>

u8 cheminert_c52_c55_transfer(u8*tx_buf,u8 tx_size,u8*rx_buf, u8*rx_size,uint16_t timeout,bool wait_flag);
#endif