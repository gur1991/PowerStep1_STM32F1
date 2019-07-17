#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H
#include "timer.h"
#include "spi1.h"

typedef enum{
	TMEPERATURE_ONE=30,
	TMEPERATURE_TWO,
	
}TMEPERATURE_type;


//�����¶ȣ���������
//void set_temperature_degree(int degree,AD_type AD_CS );
//int get_temperature_degree(AD_type AD_CS);


//һ���������������¶ȼƣ����������¶�
//Main_Keep_Temperature_Degree ��stm32 main ִ��while
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
