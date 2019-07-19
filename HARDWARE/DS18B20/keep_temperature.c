#include "keep_temperature.h"


static int SET_VALUE=0;

static int current_value=0,last_value=0;//当前温度值，上次的温度值，用于判断设定温度是否改变
static int last_duty_cycle=0, duty_cycle=0; //当前占空比和上次占空比，用于判断是否重新设定pwm
static int last_value_diff=0, value_diff=0;//当前温度差值和上次温度差值，用于判断温度变化趋势的预期
	
static int negative_feedback=0,positive_feedback=0,max_positive_feedback=0;	//正负补偿
static int negative_flag=0,positive_flag=0;//反馈的计数标志


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
			return current_value;
}



//在预设值的1.5伏进行过冲补偿计算---解决加热过多
//在预设值得5V内进行加热补偿计算---解决加热不够问题
void KeepTemperatureDegree(void)
{
		//每四秒反馈计算一次
		static int time_count = 4000/MAIN_DELAY;

			//如果温度重设，所有参数需要重新校准
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
					//每一个等级更换，就要重设校准值
					if(SET_VALUE-current_value<10&&SET_VALUE<450){duty_cycle=465;}
					else if(SET_VALUE-current_value<15&&SET_VALUE<450){duty_cycle=420;negative_feedback=0;}//设定温度过高，此占空比不用
					else if(SET_VALUE-current_value<20){duty_cycle=350;if(SET_VALUE<450){negative_feedback=0;}}
					else if(SET_VALUE-current_value<35){duty_cycle=300;negative_feedback=0;}
					else if(SET_VALUE-current_value<40){duty_cycle=250;negative_feedback=0;}
					else if(SET_VALUE-current_value<50){duty_cycle=200;negative_feedback=0;}
					else {duty_cycle=0;negative_feedback=0;}
			}
			
					negative_flag++;
					//如果5s钟温度没有变化，且没有达到预设值，也要补�
				//加热4s 冷却40s
					if(negative_flag>=time_count){
							
							positive_flag++;
							value_diff = SET_VALUE - current_value;
						
							if((value_diff>0)&&(last_value_diff<=value_diff))negative_feedback+=3;
							else if(value_diff<0){negative_feedback-=3;negative_flag=0;}
							else if(value_diff==0&&last_value_diff>0)negative_feedback+=3;//过冲回调
							else if(value_diff==0&&last_value_diff<0&&positive_flag==10){negative_feedback-=3;negative_flag=0;}//冷却回调
							negative_flag = 0;//计数值清零
						
							last_value_diff=value_diff;
					}
					
			
			
			//1.5V以外不计算补偿值
		  //解决从40->25度如此大落差的计算情况				
			if(current_value-SET_VALUE>15)negative_feedback=0;		
					
			//负补偿位置
			if(duty_cycle!=500)
					duty_cycle-=negative_feedback;
						
			printf("duty_cycle:%d negative:%d ---SET_VALUE:%d  current_value:%d\r\n",duty_cycle,negative_feedback,SET_VALUE,current_value);
			
			
			//如果这次计算的占空比和之前一样，就不去重新设定
			if(last_duty_cycle == duty_cycle)return ;
			
			last_duty_cycle = duty_cycle;
			TIM_SetTIM3Compare4(duty_cycle);	
			
			return ;
}
