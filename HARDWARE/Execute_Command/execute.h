#ifndef __EXECUTE_CONTROL_INTERFACE_H
#define __EXECUTE_CONTROL_INTERFACE_H
#include "sys.h"

typedef union{ 
	struct{
		int MaxPress;
		int MinPress;
	}request;
	struct{
		uint8_t ret;
	}response;
}Set_Pumps100_Press_type_t;

typedef union{ 
	struct{
		int IdleFlowSpeed;
	}request;
	struct{
		uint8_t ret;
	}response;
}Balance_Chromatographic_Column_type_t;


typedef union{ 
	struct{
		int Work_Flow_Speed;
		int A_timeS;
		int B_timeS;
		int C_timeS;
		int A2_timeS;
	}request;
	struct{
		uint8_t ret;
	}response;
}Gradient_control_buffer_type_t;


typedef union{ 
	struct{
		int weightA;
		int weightB;
		int weightC;
		int weightD;
	}request;
	struct{
		uint8_t ret;
	}response;
}Gravity_Sensor_Setting_type_t;

typedef union{ 
	struct{
		uint8_t nor;
	}request;
	struct{
		int weightA;
		int weightB;
		int weightC;
		int weightD;
		uint8_t ret;
	}response;
}Gravity_Sensor_Getting_type_t;



#endif
