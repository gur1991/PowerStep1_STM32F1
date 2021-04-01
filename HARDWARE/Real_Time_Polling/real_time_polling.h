#ifndef __REAL_TIME_POLLING_H
#define __REAL_TIME_POLLING_H
#include "sys.h"
#include "rs232_change_protocol.h"
#include "temperature.h"
#include "config.h"
void  Real_Time_Polling_Current_Index(void);
void  Real_Time_Polling_Init(void);

typedef struct{ 
		uint32_t degree;
		uint32_t weightOne;
	  uint32_t weightTwo;
	  uint32_t weightThree;
	  uint32_t weightFour;
	  uint8_t liquid;
	  uint8_t status;
}real_time_polling_type_t;

typedef struct{ 
	  uint32_t pumpPress;
}real_time_polling_press_type_t;



typedef union{ 
struct{
	uint8_t export_liquid_state;
	uint8_t nor[43];
}request;
struct{
	real_time_polling_type_t polling;
	uint8_t ret;
}response;
}Real_Time_Polling_t;

typedef union{ 
struct{
	uint8_t nor[44];
}request;
struct{
	real_time_polling_press_type_t polling_press;
	uint8_t ret;
}response;
}Real_Time_Polling_Press_t;



real_time_polling_press_type_t  Get_Real_Time_Polling_Press(void);
real_time_polling_type_t  Get_Real_Time_Polling_Value(void);

#endif


