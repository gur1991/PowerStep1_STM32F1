#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H
#include "timer.h"
#include "spi1.h"
#include "weight.h"
typedef enum{
	TMEPERATURE_ONE=1,
	TMEPERATURE_TWO,
	TMEPERATURE_CURRENT
	
}TMEPERATURE_type;

enum{
	ALL_GOOD=0,
	NUMBER_ONE_BROKE=1,
	NUMBER_TWO_BROKE=2,
	ALL_BROKE=3,
};


//一个加热器，两个温度计，控制温箱温度
//Main_Keep_Temperature_Degree 在stm32 main 执行while
int Get_Single_Temperature_Degree(TMEPERATURE_type devices);

void Set_Single_Temperature_Degree(int degree, TMEPERATURE_type devices);

void Main_Keep_Temperature_Degree(void);

uint8_t Temperature_Get_Status(void);

typedef union{
struct{
		uint8_t devices;
		uint32_t degree;
}request;
struct{
		uint8_t ret;
		uint8_t nor[7];
}response;
}set_single_temperature_degree_type_t;

typedef union{
struct{
		int devices;
}request;
struct{
		int degree;
		uint8_t ret;
		uint8_t status;
		uint8_t nor[2];
}response;
}get_single_temperature_degree_type_t;



typedef struct
{
	u8 (*init)(void);
  int (*get_degree)(TMEPERATURE_type devices);
  void(*set_degree)(int degree, TMEPERATURE_type devices);
	void (*keep_degree)(void);
	uint8_t (*get_status)(void);
}Thermometer_t;	


typedef enum{
	PT1000=0,
	DS18B20,
}THERMOMETER_type;

Thermometer_t* PT1000_GetHandle(void);


#endif
