#ifndef __DS18B20_KEEP_H
#define __DS18B20_KEEP_H
#include "ds18b20.h"
#include "temperature.h"
#include "config.h"

//�����֮�ڵȴ��������˳�
uint8_t Rearch_Degree_Wait(void);

//�趨�¶�
void SetTemperatureDegree(int degree, TMEPERATURE_type devices);


//�����¶�
void KeepTemperatureDegree(void);


//����¶�
int GetTemperatureDegree(TMEPERATURE_type devices);


//����IDѡ����
void ThermometerChooseHandle(THERMOMETER_type id);

Thermometer_t* Ds18b20_GetHandle(void);

extern Thermometer_t *ThermometerHandle;
#endif 
