#ifndef __ELECTROMAGNETIC_H
#define __ELECTROMAGNETIC_H
#include "sys.h"

#define	FET1 		PBout(4)  		
#define	FET2 		PBout(5) 
#define	FET3 		PBout(6)
#define	FET4 		PBout(7) 

#define	FET5 		PBout(8)
#define	FET6 		PBout(9)

#define	VALUE_A 	PEout(2)//预留给电磁阀
#define	VALUE_B   PEout(3)//VALUE0---VALUE4
#define	VALUE_C 	PEout(4)
#define	VALUE_0 	PEout(5)

#define	VALUE_D 		PEout(2)  		
#define	VALUE_E     PEout(3) 
#define	VALUE_F 		PEout(4)
#define	VALUE_G 		PEout(5) 

#define	VALUE_1 		PEout(6)
#define	VALUE_2 		PFout(0)



void Electromagnetic_init(void);

void electromagnetic_control(uint8_t devices, uint8_t status);



typedef enum FT_status_type
{
	OPEN_FT=1,
	CLOSE_FT=0,
}FT_status_type_t;

//此处电磁阀对应的设备要重新定义
typedef enum FT_Device_type
{
	DEGASSER_CONTORL=1,//一直在用
	PELTIER_CONTORL=2,
	FAN_ONE=3,
	FAN_TWO=4,
	
	ELECTROMAGNETIC_A=7,
	ELECTROMAGNETIC_B,
	ELECTROMAGNETIC_C,
	
	ELECTROMAGNETIC_D=11,
	ELECTROMAGNETIC_E,
	ELECTROMAGNETIC_F,
	
	ELECTROMAGNETIC_G,
	
	//DIAPHRAM_PUMP_CONTORL,//液路传感器检测到信号
}FT_Device_type_t;

// 7 --脱气机
// 8 --隔膜泵
typedef union{ 
	struct{
		uint8_t devices;//0-9
		uint8_t status;//1 open ---0 close
		uint8_t nor[2];
	}request;
	struct{
		uint8_t ret;
	}response;
}electromagnetic_type_t;

typedef union{
	struct{
		uint8_t device_1;
		uint8_t status_1;

		uint8_t device_2;
		uint8_t status_2;

		uint8_t device_3;
		uint8_t status_3;

		uint8_t nor[2];
	}request;
	struct{
		uint8_t ret;
	}response;
}electromagnetic_package_type_t;

void electromagnetic_control_package(electromagnetic_package_type_t data);
#endif
