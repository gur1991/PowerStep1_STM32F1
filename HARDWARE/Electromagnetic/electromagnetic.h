#ifndef __ELECTROMAGNETIC_H
#define __ELECTROMAGNETIC_H
#include "sys.h"

#define	FET0 		PGout(13)  		
#define	FET1 		PGout(12)  		
#define	FET2 		PGout(11) 
#define	FET3 		PGout(10)
#define	FET4 		PGout(9) 

#define	FET5 		PDout(7)
#define	FET6 		PDout(4)
#define	FET7 		PDout(3)
#define	FET8 		PDout(2)
#define	FET9 		PDout(1)

void Electromagnetic_init(void);

void electromagnetic_control(uint8_t devices, uint8_t status);

typedef enum FT_status_type
{
	OPEN_FT=1,
	CLOSE_FT=0,
}FT_status_type_t;

typedef enum FT_Device_type
{
	ELECTROMAGNETIC_A=0,
	ELECTROMAGNETIC_B,
	ELECTROMAGNETIC_C,
	
	ELECTROMAGNETIC_D,
	ELECTROMAGNETIC_E,
	ELECTROMAGNETIC_F,
	
	ELECTROMAGNETIC_G,
	DEGASSER_CONTORL,//一直在用
	DIAPHRAM_PUMP_CONTORL,//液路传感器检测到信号
}FT_Device_type_t;
// 7 --脱气机
// 8 --隔膜泵


typedef union{ 
	struct{
		uint8_t devices;//0-9
		uint8_t status;//1 open ---0 close
	}request;
	struct{
		uint8_t ret;
	}response;
}electromagnetic_type_t;

#endif
