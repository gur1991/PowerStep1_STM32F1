#include "keep_temperature.h"


static int SET_VALUE=0;

static int current_value=0,last_value=0;//µ±Ç°ÎÂ¶ÈÖµ£¬ÉÏ´ÎµÄÎÂ¶ÈÖµ£¬ÓÃÓÚÅĞ¶ÏÉè¶¨ÎÂ¶ÈÊÇ·ñ¸Ä±ä
static int last_duty_cycle=0, duty_cycle=0; //µ±Ç°Õ¼¿Õ±ÈºÍÉÏ´ÎÕ¼¿Õ±È£¬ÓÃÓÚÅĞ¶ÏÊÇ·ñÖØĞÂÉè¶¨pwm
static int last_value_diff=0, value_diff=0;//µ±Ç°ÎÂ¶È²îÖµºÍÉÏ´ÎÎÂ¶È²îÖµ£¬ÓÃÓÚÅĞ¶ÏÎÂ¶È±ä»¯Ç÷ÊÆµÄÔ¤ÆÚ
	
static int negative_feedback=0,positive_feedback=0,max_positive_feedback=0;	//Õı¸º²¹³¥
static int negative_flag=0,positive_flag=0;//·´À¡µÄ¼ÆÊı±êÖ¾


Thermometer_t *ThermometerHandle = 0;


Thermometer_t ds18b20=
{
	DS18B20_Init,
	GetTemperatureDegree,
	SetTemperatureDegree,
	KeepTemperatureDegree2
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
			return current_value;
}


//ÔÚÔ¤ÉèÖµµÄ1.5·ü½øĞĞ¹ı³å²¹³¥¼ÆËã---½â¾ö¼ÓÈÈ¹ı¶à
//ÔÚÔ¤ÉèÖµµÃ5VÄÚ½øĞĞ¼ÓÈÈ²¹³¥¼ÆËã---½â¾ö¼ÓÈÈ²»¹»ÎÊÌâ
int KeepTemperatureDegree(void)
{

			//Èç¹ûÎÂ¶ÈÖØÉè£¬ËùÓĞ²ÎÊıĞèÒªÖØĞÂĞ£×¼
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
	
	
			current_value =DS18B20_Get_Temp();
			
			if(current_value>=SET_VALUE){
					duty_cycle=500;
					//negative_feedback=0;
			}else{
					//Ã¿Ò»¸öµÈ¼¶¸ü»»£¬¾ÍÒªÖØÉèĞ£×¼Öµ
					if(SET_VALUE-current_value<10){duty_cycle=465;}
					else if(SET_VALUE-current_value<15){duty_cycle=440;negative_feedback=0;}
					else if(SET_VALUE-current_value<20){duty_cycle=390;negative_feedback=0;}
					else if(SET_VALUE-current_value<35){duty_cycle=300;negative_feedback=0;}
					else if(SET_VALUE-current_value<40){duty_cycle=250;negative_feedback=0;}
					else if(SET_VALUE-current_value<50){duty_cycle=200;negative_feedback=0;}
					else {duty_cycle=0;negative_feedback=0;}
					
					//Õı²¹³¥Î»ÖÃ
					if(SET_VALUE-current_value<15)
							duty_cycle+=max_positive_feedback;
			}
			
			value_diff = SET_VALUE - current_value;
			//ÎÂ¶È¼Ó²»ÉÏÈ¥
			//Èç¹ûµÃµ½µÄÎÂ¶ÈÖµÆ«²îÔ½À´Ô½´ó£¬¾ÍÒª¶ÔÕ¼¿Õ±È½øĞĞ²¹³¥
				if((value_diff>0)&&(last_value_diff<=value_diff)){
					negative_flag++;
				//	printf("negative_flag:%d\r\n",negative_flag);
					//Èç¹û2sÖÓÎÂ¶ÈÃ»ÓĞ±ä»¯£¬ÇÒÃ»ÓĞ´ïµ½Ô¤ÉèÖµ£¬Ò²Òª²¹³¥
					if(negative_flag>=5){
							if(SET_VALUE-current_value<10)negative_feedback+=5;
							else if(SET_VALUE-current_value<15)negative_feedback+=10;
							else if(SET_VALUE-current_value<30)negative_feedback+=20;
							else negative_feedback+=30;
						
							negative_flag = 0;//¼ÆÊıÖµÇåÁã
					}
					
			//Èç¹ûÎÂ¶È¹ı³åÀ÷º¦£¬¾ÍÒª¸º²¹³¥---³¬¹ıÔ¤ÉèÖµ		
			}else if(value_diff<0){
						positive_flag++;
						if(positive_flag>=5){
								//positive_feedback+=5;
								//Ã¿Ò»Ãë¼ÆËãÒ»´Î£¬¼ÇÂ¼×î´óÖµ£¬¸ù¾İ¹ı³åµÄ×î´óÖµÀ´¼ÆËãĞèÒª²¹³¥µÄÊı
								//if(max_positive_feedback<positive_feedback)
										max_positive_feedback+=5;
								
								positive_flag=0;
						}		
						
			}
			
			last_value_diff=value_diff;
			
			//¸º²¹³¥Î»ÖÃ
			if(duty_cycle!=500)
					duty_cycle-=negative_feedback;
						
			printf("duty_cycle:%d negative:%d positive:%d---SET_VALUE:%d  current_value:%d\r\n",duty_cycle,negative_feedback,max_positive_feedback,SET_VALUE,current_value);
			
			//Õı¸º²¹³¥×ÛºÏºó£¬½øĞĞ½µÊıÊı´¦Àí£¬½â¾öÀÛ¼ÓÒç³ö
			
			if(max_positive_feedback>negative_feedback){
					max_positive_feedback-=negative_feedback;
					negative_feedback=0;
			}else {
				  negative_feedback-=max_positive_feedback;
					max_positive_feedback=0;
			}
			
			
			
			
			//Èç¹ûÕâ´Î¼ÆËãµÄÕ¼¿Õ±ÈºÍÖ®Ç°Ò»Ñù£¬¾Í²»È¥ÖØĞÂÉè¶¨
			if(last_duty_cycle == duty_cycle)return current_value;
			
			last_duty_cycle = duty_cycle;
			TIM_SetTIM3Compare4(duty_cycle);	
			
			return current_value;
}





//ÔÚÔ¤ÉèÖµµÄ1.5·ü½øĞĞ¹ı³å²¹³¥¼ÆËã---½â¾ö¼ÓÈÈ¹ı¶à
//ÔÚÔ¤ÉèÖµµÃ5VÄÚ½øĞĞ¼ÓÈÈ²¹³¥¼ÆËã---½â¾ö¼ÓÈÈ²»¹»ÎÊÌâ
void KeepTemperatureDegree2(void)
{

			//Èç¹ûÎÂ¶ÈÖØÉè£¬ËùÓĞ²ÎÊıĞèÒªÖØĞÂĞ£×¼
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
	
	
			current_value =DS18B20_Get_Temp();
			
			if(current_value>=SET_VALUE){
					duty_cycle=500;
			}else{
					//Ã¿Ò»¸öµÈ¼¶¸ü»»£¬¾ÍÒªÖØÉèĞ£×¼Öµ
					if(SET_VALUE-current_value<10&&SET_VALUE<450){duty_cycle=465;}
					else if(SET_VALUE-current_value<15&&SET_VALUE<450){duty_cycle=420;negative_feedback=0;}//Éè¶¨ÎÂ¶È¹ı¸ß£¬´ËÕ¼¿Õ±È²»ÓÃ
					else if(SET_VALUE-current_value<20){duty_cycle=350;if(SET_VALUE<450){negative_feedback=0;}}
					else if(SET_VALUE-current_value<35){duty_cycle=300;negative_feedback=0;}
					else if(SET_VALUE-current_value<40){duty_cycle=250;negative_feedback=0;}
					else if(SET_VALUE-current_value<50){duty_cycle=200;negative_feedback=0;}
					else {duty_cycle=0;negative_feedback=0;}
			}
			
					negative_flag++;
					//Èç¹û5sÖÓÎÂ¶ÈÃ»ÓĞ±ä»¯£¬ÇÒÃ»ÓĞ´ïµ½Ô¤ÉèÖµ£¬Ò²Òª²¹³
				//¼ÓÈÈ4s ÀäÈ´40s
					if(negative_flag>=5){
							
							positive_flag++;
							value_diff = SET_VALUE - current_value;
						
							if((value_diff>0)&&(last_value_diff<=value_diff))negative_feedback+=3;
							else if(value_diff<0){negative_feedback-=3;negative_flag=0;}
							else if(value_diff==0&&last_value_diff>0)negative_feedback+=3;//¹ı³å»Øµ÷
							else if(value_diff==0&&last_value_diff<0&&positive_flag==10){negative_feedback-=3;negative_flag=0;}//ÀäÈ´»Øµ÷
							negative_flag = 0;//¼ÆÊıÖµÇåÁã
						
							last_value_diff=value_diff;
					}
					
			
			
			//1.5VÒÔÍâ²»¼ÆËã²¹³¥Öµ
		  //½â¾ö´Ó40->25¶ÈÈç´Ë´óÂä²îµÄ¼ÆËãÇé¿ö				
			if(current_value-SET_VALUE>15)negative_feedback=0;		
					
			//¸º²¹³¥Î»ÖÃ
			if(duty_cycle!=500)
					duty_cycle-=negative_feedback;
						
			printf("duty_cycle:%d negative:%d ---SET_VALUE:%d  current_value:%d\r\n",duty_cycle,negative_feedback,SET_VALUE,current_value);
			
			
			//Èç¹ûÕâ´Î¼ÆËãµÄÕ¼¿Õ±ÈºÍÖ®Ç°Ò»Ñù£¬¾Í²»È¥ÖØĞÂÉè¶¨
			if(last_duty_cycle == duty_cycle)return ;
			
			last_duty_cycle = duty_cycle;
			TIM_SetTIM3Compare4(duty_cycle);	
			
			return ;
}
