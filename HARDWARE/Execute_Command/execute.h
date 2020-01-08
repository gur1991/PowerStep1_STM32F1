#ifndef __EXECUTE_CONTROL_INTERFACE_H
#define __EXECUTE_CONTROL_INTERFACE_H
#include "sys.h"

typedef union{ 
	struct{
		uint32_t MaxPress;
		uint32_t MinPress;
	}request;
	struct{
		uint8_t ret;
		uint8_t nor[7];
	}response;
}Set_Pumps100_Press_type_t;

typedef union{ 
	struct{
		uint32_t IdleFlowSpeed;
	}request;
	struct{
		uint8_t ret;
		uint8_t nor[3];
	}response;
}Balance_Chromatographic_Column_type_t;


typedef union{ 
	struct{
		uint32_t Work_Flow_Speed;
		uint32_t A_timeS;
		uint32_t B_timeS;
		uint32_t C_timeS;
		uint32_t A2_timeS;
	}request;
	struct{
		uint8_t ret;
		uint8_t nor[19];
	}response;
}Gradient_control_buffer_type_t;


typedef union{ 
	struct{
		uint32_t weightA;
		uint32_t weightB;
		uint32_t weightC;
		uint32_t weightD;
	}request;
	struct{
		uint8_t ret;
		uint8_t nor[19];
	}response;
}Gravity_Sensor_Setting_type_t;

typedef union{ 
	struct{
		uint8_t nor[20];
	}request;
	struct{
		uint32_t weightA;
		uint32_t weightB;
		uint32_t weightC;
		uint32_t weightD;
		uint8_t ret;
	}response;
}Gravity_Sensor_Getting_type_t;



#endif
