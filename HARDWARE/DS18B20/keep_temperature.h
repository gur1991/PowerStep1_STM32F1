#ifndef __DS18B20_KEEP_H
#define __DS18B20_KEEP_H
#include "ds18b20.h"
#include "temperature.h"

//五分钟之内等待，否则退出
uint8_t Rearch_Degree_Wait(void);

//设定温度
void SetTemperatureDegree(int degree, TMEPERATURE_type devices);


//保持温度
void KeepTemperatureDegree(void);

void KeepTemperatureDegree_Duty(void);

//获得温度
int GetTemperatureDegree(TMEPERATURE_type devices);


//根据ID选择函数
void ThermometerChooseHandle(THERMOMETER_type id);

Thermometer_t* Ds18b20_GetHandle(void);

extern Thermometer_t *ThermometerHandle;


typedef struct pid_type{
	int setValue;
	int actualValue;
	
	int differ;
	int differ_last;
	int differ_pre;

	float Kp,Ki,Kd;
	float Up,Ui,Ud,Upid;
	
	float  balance;
	float duty_pwm;
	float duty_pwm_last;
}pid_type_t;


void Pid_init(void);
#endif 
