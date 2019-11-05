#ifndef __RS232_CHANGER_PROTOCOL_H
#define __RS232_CHANGER_PROTOCOL_H
#include "temperature.h"

typedef union{
struct{
		TMEPERATURE_type devices;
		int degree;
}request;
struct{
		uint8_t ret;
}response;
}mini_set_single_temperature_degree_type_t;

typedef union{
struct{
		TMEPERATURE_type devices;
}request;
struct{
		int degree;
		uint8_t ret;
}response;
}mini_get_single_temperature_degree_type_t;


uint8_t STM32_Change_Protocol_Control_Mini_Board_Get_Degree(get_single_temperature_degree_type_t data,int*degree);

uint8_t STM32_Change_Protocol_Control_Mini_Board_Set_Degree(set_single_temperature_degree_type_t data);



#endif

