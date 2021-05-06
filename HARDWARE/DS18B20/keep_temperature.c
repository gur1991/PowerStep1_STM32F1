#include "keep_temperature.h"
#include "delay.h"
#include "config.h"
#include "temperature.h"
#include <stdlib.h>

static int SET_VALUE=0;
//static int FLAG_SET_VALUE=0;
static int FLAG_START_PID=0;
static int current_value=0;
pid_type_t pid;
static uint8_t T_STATUS=ALL_GOOD;


Thermometer_t *ThermometerHandle = 0;

Thermometer_t ds18b20=
{
	DS18B20_Init,
	GetTemperatureDegree,
	SetTemperatureDegree,
	KeepTemperatureDegree,
	GetTemperatureStatus
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

uint8_t GetTemperatureStatus(void)
{return T_STATUS;}	


void SetTemperatureDegree(int degree, TMEPERATURE_type devices)
{
					SET_VALUE=degree;
					FLAG_START_PID=1;
					pid.balance=400.0-0.98*(degree-355);
}

void KeepTemperatureDegree_Duty(void)
{	
#if USE_KEEP_TEMPERATURE_BOARD		
	float keep_duty=435-(SET_VALUE-375)*0.6;
	TIM_SetTIM3Compare4((int)keep_duty);
#endif
}	

int GetTemperatureDegree(TMEPERATURE_type devices)
{
	//异常后，设置当前温度为1000

/*
	if(SET_VALUE<0)
	{
		return 1000;
	}
*/
	
	return current_value;

	}

//五分钟之内等待，否则退出
uint8_t Rearch_Degree_Wait(void)
{
	uint8_t ret=0;
	
#if USE_KEEP_TEMPERATURE_BOARD		
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
		
		KeepTemperatureDegree();
		
		delay_ms(1000);
		
	}
#endif
	
	return ret;
}	


void Pid_init(void)
{
	pid.setValue = 0;
	pid.actualValue= 0;
	pid.differ= 0;
	pid.differ_last =0;
	pid.differ_pre=0;
	
	pid.Kp=0.8;
	pid.Ki=0.8;
	pid.Kd=0.4;
	pid.Up=0.0,
	pid.Ui=0.0;
	pid.Ud=0.0;
	pid.Upid=0.0;
	pid.balance=460.0;
	pid.duty_pwm=0.0;
	pid.duty_pwm_last=pid.balance;
}	

//SET_VALUE < 0  abort
int PID_Control(int temperature)
{
	if(temperature == 1000)return 500;
	
	pid.setValue = SET_VALUE;
	pid.actualValue=temperature;
	pid.differ_pre=pid.differ_last;
	pid.differ_last=pid.differ;
	
	pid.differ =pid.setValue - pid.actualValue;
	
	LOGD("set:%d get:%d ",pid.setValue,pid.actualValue);
	if(pid.differ>=5){pid.duty_pwm=0;}
	else if(pid.differ<0){
			pid.duty_pwm=500;
			pid.duty_pwm_last+=pid.Ud;
	}
	else if(pid.differ==0){
			pid.duty_pwm=500;
			pid.duty_pwm_last+=0.4*pid.Ud;
	}
	else {

		if(FLAG_START_PID)
		{
			pid.duty_pwm_last=pid.balance;
			FLAG_START_PID=0;
		}
		pid.Up=pid.differ*pid.Kp;
		pid.Ui=-pid.differ_last*pid.Ki;
		pid.Ud=pid.differ_pre*pid.Kd;
		pid.Upid=pid.Up+pid.Ui+pid.Ud;

		pid.duty_pwm=pid.duty_pwm_last-pid.Upid;
		pid.duty_pwm_last=pid.duty_pwm;
		
	}

	return (int)pid.duty_pwm;
}

void KeepTemperatureDegree(void)
{	

#if USE_KEEP_TEMPERATURE_BOARD	
		int duty_cycle=0;
		int temp1=0,temp2=0;
		static int i=0;
		
		temp1=DS18B20_Get_Temp(TMEPERATURE_ONE);
		temp2=DS18B20_Get_Temp(TMEPERATURE_TWO);
	  LOGD("T1:%0.1f,T2:%0.1f.\r\n",temp1*0.1,temp2*0.1);	
	
		T_STATUS=ALL_GOOD;
		if(temp1<=0 || temp1>=600){
				if(temp2<=0 || temp2>=600)T_STATUS=ALL_BROKE;
			  else  T_STATUS=NUMBER_ONE_BROKE;
		}else if(temp2<=0 || temp2>=600)T_STATUS=NUMBER_TWO_BROKE;
		
		
	
		if(T_STATUS==NUMBER_ONE_BROKE){current_value = temp2;}//温度计1坏了 
	  else if(T_STATUS==NUMBER_TWO_BROKE){current_value = temp1;}//温度计2坏了
    else if(T_STATUS==ALL_GOOD){
			if(abs(temp1-temp2)>100)T_STATUS=ALL_BROKE;//两个温度传感器差10度也是异常
			else current_value = (int)((temp1+temp2)/2);
		}//温度都可以 
		
		//连续5次检测到温度过高，则判断为异常，设置温度0；单次异常不会触发此机制
	  if(T_STATUS==ALL_BROKE)
		{
				i++;
				if(i>=20)
				{
					current_value=1000;
					SetTemperatureDegree(-1,TMEPERATURE_CURRENT);//温度异常，设置为0
				}	
		}else{
			i=0;
		}
		
		//current_value =(int)((DS18B20_Get_Temp(TMEPERATURE_ONE)+DS18B20_Get_Temp(TMEPERATURE_TWO))/2);
		
		
		duty_cycle= PID_Control(current_value);
		
		
	
		if(duty_cycle>=500)duty_cycle=500;
		else if(duty_cycle<=0)duty_cycle=0;
	
		LOGD(" duty:%d\r\n",duty_cycle);
		
		//duty_cycle=0;
		TIM_SetTIM3Compare4(duty_cycle);	

#endif		
}	




