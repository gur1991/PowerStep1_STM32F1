#ifndef _WEIGHT_H
#define _WEIGHT_H
#include "spi1.h"
#include "spi3.h"
#include "config.h"
#include "sys.h"

typedef enum{
	AD1_CS=10,
	AD2_CS,
	AD3_CS,
	AD4_CS,
}AD_type;

typedef enum{
	CS_LOW=0,
	CS_HIGH=1,
}AD_LEVEL_type;

//只有两个接口---
//获取数据的接口被被weight 的模块重新包装

extern u8 Weight_Sensor_Init(void);

extern int AD_Sensor_Get_Data(AD_type cs);

#define	WEIGHT_AD1_CS 		PGout(10)  		//AD1_CS
#define	WEIGHT_AD2_CS 		PGout(11)  		//AD2_CS
#define	WEIGHT_AD3_CS 		PGout(12)  		//AD3_CS
#define	WEIGHT_AD4_CS 		PGout(13)  		//AD4_CS



typedef enum{
	WEIGHT_ONE=15,
	WEIGHT_TWO,
	WEIGHT_THREE,
	WEIGHT_FOUR,	
}WEIGHT_type;



typedef union{
struct{
		uint32_t  weight;
}request;
struct{
		uint32_t gram;
		uint8_t ret;
		uint8_t nor[3];
}response;
}get_weight_current_gram_type_t;

typedef union{
struct{
		uint32_t weight;
		uint32_t gram;
}request;
struct{
		uint8_t ret;
		uint8_t nor[7];
}response;
}set_weight_warning_line_type_t;

typedef union{
struct{
		uint32_t weight;
}request;
struct{
		uint8_t ret;
		uint8_t result;
	  uint8_t nor[2];
}response;
}get_single_weight_warning_result_type_t;

typedef union{
struct{
		uint8_t nor[4];
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














