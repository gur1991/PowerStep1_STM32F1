#include "real_time_polling.h"
#include "slave_uart_control_interface.h"

static real_time_polling_type_t POLLING_data;
static real_time_polling_press_type_t POLLING_press;


void  Real_Time_Polling_Init(void)
{
		memset( (void*)&POLLING_data, 0, sizeof(POLLING_data));
		memset( (void*)&POLLING_press, 0, sizeof(POLLING_press));
}

void  Real_Time_Polling_Current_Index(void)
{
	
#if USE_AUTOMATIC_INJECTION_BOARD
	  get_single_temperature_degree_type_t data;
		data.request.devices=TMEPERATURE_CURRENT;
	
		memset(&POLLING_data, 0, sizeof(POLLING_data));
	  STM32_Change_Protocol_Control_Mini_Board_Get_Degree(data,&POLLING_data.degree);
	 
	  POLLING_data.liquid=Liquid_Sensor_Get();
		POLLING_data.weightOne=Get_weight_current_gram(WEIGHT_ONE);
	  POLLING_data.weightTwo=Get_weight_current_gram(WEIGHT_TWO);
		POLLING_data.weightThree=Get_weight_current_gram(WEIGHT_THREE);
		POLLING_data.weightFour=Get_weight_current_gram(WEIGHT_FOUR);

#endif
	
	
#if USE_GRADIENT_CONTROL_BOARD
	
		POLLING_press.pumpPress=PumpHandle->readPress();

#endif	
	
}	

real_time_polling_type_t  Get_Real_Time_Polling_Value(void)
{
		return POLLING_data;
}	

real_time_polling_press_type_t  Get_Real_Time_Polling_Press(void)
{
		return POLLING_press;
}	
