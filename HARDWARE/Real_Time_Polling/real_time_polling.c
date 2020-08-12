#include "real_time_polling.h"
#include "slave_uart_control_interface.h"
#include "config.h"

static real_time_polling_type_t POLLING_data;
static real_time_polling_press_type_t POLLING_press;


void  Real_Time_Polling_Init(void)
{
		memset( (void*)&POLLING_data, 0, sizeof(POLLING_data));
		memset( (void*)&POLLING_press, 0, sizeof(POLLING_press));
}

void  Real_Time_Polling_Current_Index(void)
{
		//LOGD("start \r\n");
	
#if USE_AUTOMATIC_INJECTION_BOARD
	  get_single_temperature_degree_type_t data;
		data.request.devices=TMEPERATURE_CURRENT;
		
		uint32_t degree=0;
		uint32_t weight1=0;
		uint32_t weight2=0;
		uint32_t weight3=0;
		uint32_t weight4=0;
	
		//memset(&POLLING_data, 0, sizeof(POLLING_data));
	  
	
		STM32_Change_Protocol_Control_Mini_Board_Get_Degree(data,&degree);
		
		//����¶��쳣	
		if(degree<=800 || degree==1000)
		{ 
			POLLING_data.degree=degree;
			//LOGD("degree:%d \r\n",degree);
		}
		//LOGD("POLLING_data.degree:%d \r\n",POLLING_data.degree);

	  POLLING_data.liquid=Liquid_Sensor_Get();
		
		weight1=Get_weight_current_gram(WEIGHT_ONE);
		weight2=Get_weight_current_gram(WEIGHT_TWO);
		weight3=Get_weight_current_gram(WEIGHT_THREE);
		weight4=Get_weight_current_gram(WEIGHT_FOUR);
		//��������쳣	 weight1
		if(weight1<5000)
		{ 
			POLLING_data.weightOne=weight1;
		}
		//weight2
		if(weight2<5000)
		{ 
			POLLING_data.weightTwo=weight2;
		}
		//weight3	
		if(weight3<5000)
		{ 
			POLLING_data.weightThree=weight3;
		}
		//weight4	
		if(weight4<5000)
		{ 
			POLLING_data.weightFour=weight4;
		}
		
#endif
	
	
#if USE_GRADIENT_CONTROL_BOARD
	
		POLLING_press.pumpPress=PumpHandle->readPress();

#endif	
	//LOGD("end \r\n");
}	

real_time_polling_type_t  Get_Real_Time_Polling_Value(void)
{
		return POLLING_data;
}	

real_time_polling_press_type_t  Get_Real_Time_Polling_Press(void)
{
		return POLLING_press;
}	
