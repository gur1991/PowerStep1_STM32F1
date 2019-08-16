#include "keep_temperature.h"
#include "delay.h"

static int SET_VALUE=0;

static int current_value=0,last_value=0;//µ±Ç°ÎÂ¶ÈÖµ£¬ÉÏ´ÎµÄÎÂ¶ÈÖµ£¬ÓÃÓÚÅÐ¶ÏÉè¶¨ÎÂ¶ÈÊÇ·ñ¸Ä±ä
static int last_duty_cycle=0, duty_cycle=0; //µ±Ç°Õ¼¿Õ±ÈºÍÉÏ´ÎÕ¼¿Õ±È£¬ÓÃÓÚÅÐ¶ÏÊÇ·ñÖØÐÂÉè¶¨pwm
static int last_value_diff=0, value_diff=0;//µ±Ç°ÎÂ¶È²îÖµºÍÉÏ´ÎÎÂ¶È²îÖµ£¬ÓÃÓÚÅÐ¶ÏÎÂ¶È±ä»¯Ç÷ÊÆµÄÔ¤ÆÚ
	
static int negative_feedback=0,max_positive_feedback=0,positive_feedback=0;	//Õý¸º²¹³¥
static int negative_flag=0,positive_flag=0;//·´À¡µÄ¼ÆÊý±êÖ¾


Thermometer_t *ThermometerHandle = 0;


Thermometer_t ds18b20=
{
	DS18B20_Init,
	GetTemperatureDegree,
	SetTemperatureDegree,
	KeepTemperatureDegree
};

Thermometer_t* Ds18b20_GetHandle(void)
{
  return (&ds18b20);
}
void ThermometerChooseHandle(THERMOMETER_type id)
{

  if (id == DS18B20)
  {
    ThermometerHandle = Ds18b20_GetHandle();
	}else if(id == PT1000)
	{
		ThermometerHandle = PT1000_GetHandle();
	}	
}


void SetTemperatureDegree(int degree, TMEPERATURE_type devices)
{
					SET_VALUE=degree;
}

int GetTemperatureDegree(TMEPERATURE_type devices)
{
			if(TMEPERATURE_CURRENT==devices)
				return (int)((DS18B20_Get_Temp(TMEPERATURE_ONE)+DS18B20_Get_Temp(TMEPERATURE_TWO))/2);
			else	
				return DS18B20_Get_Temp(devices);
}

//Îå·ÖÖÓÖ®ÄÚµÈ´ý£¬·ñÔòÍË³ö
uint8_t Rearch_Degree_Wait(void)
{
	uint8_t ret=0;
	int i=0;
	while(1)
	{
		if(i>=300)
		{
				ret=1;
				break;
		}
		i++;
		if(SET_VALUE-GetTemperatureDegree(TMEPERATURE_CURRENT)<=3&&SET_VALUE-GetTemperatureDegree(TMEPERATURE_CURRENT)>=0)
				break;

		if(GetTemperatureDegree(TMEPERATURE_CURRENT)-SET_VALUE<=3&&GetTemperatureDegree(TMEPERATURE_CURRENT)-SET_VALUE>=0)
				break;
		
		delay_ms(1000);
		
	}
	return ret;
}	


//ÔÚÔ¤ÉèÖµµÄ1.5·ü½øÐÐ¹ý³å²¹³¥¼ÆËã---½â¾ö¼ÓÈÈ¹ý¶à
//ÔÚÔ¤ÉèÖµµÃ5VÄÚ½øÐÐ¼ÓÈÈ²¹³¥¼ÆËã---½â¾ö¼ÓÈÈ²»¹»ÎÊÌâ
void KeepTemperatureDegree(void)
{
		//Ã¿ËÄÃë·´À¡¼ÆËãÒ»´Î
		//static int time_count = 4000/MAIN_DELAY;

			//Èç¹ûÎÂ¶ÈÖØÉè£¬ËùÓÐ²ÎÊýÐèÒªÖØÐÂÐ£×¼
			if(SET_VALUE!=last_value){
					last_value=SET_VALUE;
					{
							last_duty_cycle=0;
							duty_cycle=0;
						
							positive_feedback=0;
							max_positive_feedback=0;
						
							last_value_diff=0;
							value_diff=0;
						
							negative_feedback=0;
							negative_flag=0;
							positive_flag=0;

					}	
			}
	
			current_value =(int)((DS18B20_Get_Temp(TMEPERATURE_ONE)+DS18B20_Get_Temp(TMEPERATURE_TWO))/2);
			
			if(current_value>=SET_VALUE){
					duty_cycle=500;
			}else{
					//Ã¿Ò»¸öµÈ¼¶¸ü»»£¬¾ÍÒªÖØÉèÐ£×¼Öµ
					if(SET_VALUE-current_value<0&&SET_VALUE<450){duty_cycle=465;}
					else if(SET_VALUE-current_value<15&&SET_VALUE<450){duty_cycle=420;negative_feedback=0;}//Éè¶¨ÎÂ¶È¹ý¸ß£¬´ËÕ¼¿Õ±È²»ÓÃ
					else if(SET_VALUE-current_value<20){duty_cycle=350;if(SET_VALUE<450){negative_feedback=0;}}
					else if(SET_VALUE-current_value<35){duty_cycle=300;negative_feedback=0;}
					else if(SET_VALUE-current_value<40){duty_cycle=250;negative_feedback=0;}
					else if(SET_VALUE-current_value<50){duty_cycle=200;negative_feedback=0;}
					else {duty_cycle=0;negative_feedback=0;}
			}
			
					negative_flag++;
					//Èç¹û5sÖÓÎÂ¶ÈÃ»ÓÐ±ä»¯£¬ÇÒÃ»ÓÐ´ïµ½Ô¤ÉèÖµ£¬Ò²Òª²¹³
				//¼ÓÈÈ4s ÀäÈ´40s
					if(negative_flag){
							
							positive_flag++;
							value_diff = SET_VALUE - current_value;
						
							if((value_diff>0)&&(last_value_diff<=value_diff))negative_feedback+=3;
							else if(value_diff<0){negative_feedback-=3;negative_flag=0;}
							else if(value_diff==0&&last_value_diff>0)negative_feedback+=3;//¹ý³å»Øµ÷
							else if(value_diff==0&&last_value_diff<0&&positive_flag==10){negative_feedback-=3;negative_flag=0;}//ÀäÈ´»Øµ÷
							negative_flag = 0;//¼ÆÊýÖµÇåÁã
						
							last_value_diff=value_diff;
					}
					
			
			
			//1.5VÒÔÍâ²»¼ÆËã²¹³¥Öµ
		  //½â¾ö´Ó40->25¶ÈÈç´Ë´óÂä²îµÄ¼ÆËãÇé¿ö				
			if(current_value-SET_VALUE>15)negative_feedback=0;		
					
			//¸º²¹³¥Î»ÖÃ
			if(duty_cycle!=500)
					duty_cycle-=negative_feedback;
						
			//printf("duty_cycle:%d negative:%d ---SET_VALUE:%d  current_value:%d\r\n",duty_cycle,negative_feedback,SET_VALUE,current_value);
			
			
			//Èç¹ûÕâ´Î¼ÆËãµÄÕ¼¿Õ±ÈºÍÖ®Ç°Ò»Ñù£¬¾Í²»È¥ÖØÐÂÉè¶¨
			if(last_duty_cycle == duty_cycle)return ;
			
			last_duty_cycle = duty_cycle;
			TIM_SetTIM3Compare4(duty_cycle);	
			
			return ;
}
