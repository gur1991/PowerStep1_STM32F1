#ifndef __REAL_TIME_POLLING_H
#define __REAL_TIME_POLLING_H
#include "sys.h"
#include "config.h"
#include "rs232_change_protocol.h"
#include "temperature.h"

void  Real_Time_Polling_Current_Index(void);

typedef struct{ 
		int degree;
		int weightOne;
	  int weightTwo;
	  int weightThree;
	  int weightFour;
	  uint8_t liquid;
}real_time_polling_type_t;

typedef struct{ 
	  int pumpPress;
}real_time_polling_press_type_t;


extern real_time_polling_type_t PollingData;
extern real_time_polling_press_type_t PollingPressData;


typedef union{ 
struct{
	uint8_t nor;
}request;
struct{
	real_time_polling_type_t polling;
	uint8_t ret;
}response;
}Real_Time_Polling_t;

typedef union{ 
struct{
	uint8_t nor;
}request;
struct{
	real_time_polling_press_type_t polling_press;
	uint8_t ret;
}response;
}Real_Time_Polling_Press_t;

#endif


