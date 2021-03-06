#ifndef _WEIGHT_H
#define _WEIGHT_H
#include "spi1.h"

typedef enum{
	WEIGHT_ONE=15,
	WEIGHT_TWO,
	WEIGHT_THREE,
	WEIGHT_FOUR,	
}WEIGHT_type;



typedef union{
struct{
		WEIGHT_type weight;
}request;
struct{
		int gram;
		uint8_t ret;
}response;
}get_weight_current_gram_type_t;

typedef union{
struct{
		WEIGHT_type weight;
		int gram;
}request;
struct{
		uint8_t ret;
}response;
}set_weight_warning_line_type_t;

typedef union{
struct{
		WEIGHT_type weight;
}request;
struct{
		uint8_t ret;
		uint8_t result;
}response;
}get_single_weight_warning_result_type_t;

typedef union{
struct{
		uint8_t nor;
}request;
struct{
		uint8_t ret;
		uint8_t result;
}response;
}get_all_weight_warning_result_type_t;


/**设置的是重量，多少g，返回的是0则正常，返回1则警告*/

/*获取单个重力传感器是否超重*/
uint8_t Get_Single_Weight_Sensor_Warnning_Result(WEIGHT_type weight);

/*获取所有重力传感器是否警告�*/
uint8_t Get_All_Weight_Sensor_Warnning_Result(void);

/*设定传感器超重 警告线*/
void Set_Weight_Sensor_Warnning_Line(WEIGHT_type weight,int gram);

/*获得指定重力传感器的gram值
*/
int Get_weight_current_gram(WEIGHT_type weight);
#endif














