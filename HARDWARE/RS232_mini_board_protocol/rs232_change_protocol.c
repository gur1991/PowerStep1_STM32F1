#include "rs232_change_protocol.h"
#include "mini_board_protocol.h"
#include "config.h"


uint8_t STM32_Change_Protocol_Control_Mini_Board_Set_Degree(set_single_temperature_degree_type_t data)
{
		uint8_t ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD		
  	mini_set_single_temperature_degree_type_t action;
		
		action.request.degree=data.request.degree;
	  action.request.devices=data.request.devices;

		ret=Master_Mini_Board_Set_Degree(action);
#endif

	
		return ret;
}	

uint8_t STM32_Change_Protocol_Control_Mini_Board_Get_Degree(get_single_temperature_degree_type_t data,int*degree)
{
		uint8_t ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD				
    mini_get_single_temperature_degree_type_t action;
		
	  action.request.devices=data.request.devices;
	
		ret=Master_Mini_Board_Get_Degree(action,degree);
#endif
	
		return ret;
}	
