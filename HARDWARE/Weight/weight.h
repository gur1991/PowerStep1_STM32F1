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


/**ÉèÖÃµÄÊÇÖØÁ¿£¬¶àÉÙg£¬·µ»ØµÄÊÇ0ÔòÕı³££¬·µ»Ø1Ôò¾¯¸æ*/

/*»ñÈ¡µ¥¸öÖØÁ¦´«¸ĞÆ÷ÊÇ·ñ³¬ÖØ*/
uint8_t Get_Single_Weight_Sensor_Warnning_Result(WEIGHT_type weight);

/*»ñÈ¡ËùÓĞÖØÁ¦´«¸ĞÆ÷ÊÇ·ñ¾¯¸æØ*/
uint8_t Get_All_Weight_Sensor_Warnning_Result(void);

/*Éè¶¨´«¸ĞÆ÷³¬ÖØ ¾¯¸æÏß*/
void Set_Weight_Sensor_Warnning_Line(WEIGHT_type weight,int gram);

#endif














