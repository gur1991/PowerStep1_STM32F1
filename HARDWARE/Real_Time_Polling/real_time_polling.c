#include "real_time_polling.h"
#include "slave_uart_control_interface.h"
#include "config.h"
#include "temperature.h"

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
		
		uint32_t degree=0;
		uint32_t weight1=0;
		uint32_t weight2=0;
		uint32_t weight3=0;
		uint32_t weight4=0;
	  uint8_t status=ALL_GOOD;	  
	
		uint8_t ret=STM32_Change_Protocol_Control_Mini_Board_Get_Degree(data,&degree,&status);
		if(!ret)
		{	
				POLLING_data.status=status;
				//解决温度异常	
				if(degree<=800)// ||degree==1000)
				{ 
					POLLING_data.degree=degree;
				}
				POLLING_data.degreeConnect=0;
		}else{
				POLLING_data.degreeConnect=1;
		}
		
	  POLLING_data.liquid=Liquid_Sensor_Get();
		
		weight1=Get_weight_current_gram(WEIGHT_ONE);
		weight2=Get_weight_current_gram(WEIGHT_TWO);
		weight3=Get_weight_current_gram(WEIGHT_THREE);
		weight4=Get_weight_current_gram(WEIGHT_FOUR);
		//解决重力异常	 weight1
		//if(weight1<5000)
		{ 
			POLLING_data.weightOne=weight1;
		}
		//weight2
		//if(weight2<5000)
		{ 
			POLLING_data.weightTwo=weight2;
		}
		//weight3	
		//if(weight3<5000)
		{ 
			POLLING_data.weightThree=weight3;
		}
		//weight4	
	//if(weight4<5000)
		{ 
			POLLING_data.weightFour=weight4;
		}
		
#endif
	
#if USE_GRADIENT_CONTROL_BOARD
		POLLING_press.pumpPress=PumpHandle->readPress();
		
		if(POLLING_press.pumpPress<0)
			 POLLING_press.pumpConnect=1;
		else
			 POLLING_press.pumpConnect=0;
#endif	
}	

real_time_polling_type_t  Get_Real_Time_Polling_Value(void)
{
#if (USE_AUTOMATIC_INJECTION_BOARD||USE_GRADIENT_CONTROL_BOARD)		
		Real_Time_Polling_Current_Index();
#endif	
		return POLLING_data;
}	

real_time_polling_press_type_t  Get_Real_Time_Polling_Press(void)
{
#if (USE_AUTOMATIC_INJECTION_BOARD||USE_GRADIENT_CONTROL_BOARD)		
		Real_Time_Polling_Current_Index();
#endif	
	return POLLING_press;
}	



