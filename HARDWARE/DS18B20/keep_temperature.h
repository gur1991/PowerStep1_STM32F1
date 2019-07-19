#ifndef __DS18B20_KEEP_H
#define __DS18B20_KEEP_H
#include "ds18b20.h"
#include "temperature.h"

#define MAIN_DELAY 1

//设定温度
void SetTemperatureDegree(int degree, TMEPERATURE_type devices);


//保持温度
void KeepTemperatureDegree(void);


//获得温度
int GetTemperatureDegree(TMEPERATURE_type devices);


//根据ID选择函数
void ThermometerChooseHandle(THERMOMETER_type id);

Thermometer_t* Ds18b20_GetHandle(void);

extern Thermometer_t *ThermometerHandle;
#endif 
