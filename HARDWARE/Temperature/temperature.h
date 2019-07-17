#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H
#include "timer.h"
#include "spi1.h"

typedef enum{
	TMEPERATURE_ONE=30,
	TMEPERATURE_TWO,
	
}TMEPERATURE_type;


//两个温度，三个重力
//void set_temperature_degree(int degree,AD_type AD_CS );
//int get_temperature_degree(AD_type AD_CS);


//一个加热器，两个温度计，控制温箱温度
//Main_Keep_Temperature_Degree 在stm32 main 执行while
int Get_Single_Temperature_Degree(TMEPERATURE_type devices);

void Set_Single_Temperature_Degree(int degree, TMEPERATURE_type devices);

void Main_Keep_Temperature_Degree(void);


typedef union{
struct{
		TMEPERATURE_type devices;
		int degree;
}request;
struct{
		uint8_t ret;
}response;
}set_single_temperature_degree_type_t;

typedef union{
struct{
		TMEPERATURE_type devices;
}request;
struct{
		int degree;
		uint8_t ret;
}response;
}get_single_temperature_degree_type_t;



#endif
