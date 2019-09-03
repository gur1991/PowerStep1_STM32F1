#include "keep_temperature.h"
#include "delay.h"

static int SET_VALUE=0;
static int FLAG_SET_VALUE=0;
static int FLAG_START_PID=0;

pid_type_t pid;



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
					FLAG_START_PID=1;
					pid.balance=400.0-0.98*(degree-355);
}

int GetTemperatureDegree(TMEPERATURE_type devices)
{
			if(TMEPERATURE_CURRENT==devices)
				return (int)((DS18B20_Get_Temp(TMEPERATURE_ONE)+DS18B20_Get_Temp(TMEPERATURE_TWO))/2);
			else	
				return DS18B20_Get_Temp(devices);
}

//�����֮�ڵȴ��������˳�
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


int PID_Control(int temperature)
{
	pid.setValue = SET_VALUE;
	pid.actualValue=temperature;
	pid.differ_pre=pid.differ_last;
	pid.differ_last=pid.differ;
	
	pid.differ =pid.setValue - pid.actualValue;
	
	printf("set:%d get:%d ",pid.setValue,pid.actualValue);
	if(pid.differ>=5){pid.duty_pwm=0;}
	//else if(pid.differ>=5){pid.duty_pwm=400;}
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
		int current_value=0;
		int duty_cycle=0;
		current_value =(int)((DS18B20_Get_Temp(TMEPERATURE_ONE)+DS18B20_Get_Temp(TMEPERATURE_TWO))/2);
		duty_cycle= PID_Control(current_value);
		
		
	
		if(duty_cycle>=500)duty_cycle=500;
		else if(duty_cycle<=0)duty_cycle=0;
	
		printf(" duty:%d\r\n",duty_cycle);
		
		TIM_SetTIM3Compare4(duty_cycle);	
}	




