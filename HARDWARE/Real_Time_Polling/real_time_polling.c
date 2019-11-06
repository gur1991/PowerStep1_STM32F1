#include "real_time_polling.h"
#include "slave_uart_control_interface.h"

real_time_polling_type_t PollingData;
real_time_polling_press_type_t PollingPressData;


void  Real_Time_Polling_Current_Index(void)
{
	
#if USE_AUTOMATIC_INJECTION_BOARD
	  get_single_temperature_degree_type_t data;
		data.request.devices=TMEPERATURE_CURRENT;
	
		memset(&PollingData, 0, sizeof(PollingData));
	  STM32_Change_Protocol_Control_Mini_Board_Get_Degree(data,&PollingData.degree);
	 
	  PollingData.liquid=Liquid_Sensor_Get();
		PollingData.weightOne=Get_weight_current_gram(WEIGHT_ONE);
	  PollingData.weightTwo=Get_weight_current_gram(WEIGHT_TWO);
		PollingData.weightThree=Get_weight_current_gram(WEIGHT_THREE);
		PollingData.weightFour=Get_weight_current_gram(WEIGHT_FOUR);

#elif USE_GRADIENT_CONTROL_BOARD
	
		PollingPressData.pumpPress=PumpHandle->readPress();

#endif	
	
}	


