#include "rs232_change_protocol.h"
#include "mini_board_protocol.h"


uint8_t STM32_Change_Protocol_Control_Mini_Board_Set_Degree(set_single_temperature_degree_type_t data)
{
		uint8_t ret=0;
    mini_set_single_temperature_degree_type_t action;
		
		action.request.degree=data.request.degree;
	  action.request.devices=data.request.devices;

		ret=Master_Mini_Board_Set_Degree(action);
	
		return ret;
}	

uint8_t STM32_Change_Protocol_Control_Mini_Board_Get_Degree(get_single_temperature_degree_type_t data,int*degree)
{
		uint8_t ret=0;
    mini_get_single_temperature_degree_type_t action;
		
	  action.request.devices=data.request.devices;
	
		ret=Master_Mini_Board_Get_Degree(action,degree);
	
		return ret;
}	
