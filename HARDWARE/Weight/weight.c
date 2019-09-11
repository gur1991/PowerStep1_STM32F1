#include "weight.h"

static int WARNNIG_VALUE_ONE=0;
static int WARNNIG_VALUE_TWO=0;
static int WARNNIG_VALUE_THREE=0;
static int WARNNIG_VALUE_FOUR=0;

static void __set_weight_sensor_warnning_line(WEIGHT_type weight,int degree)
{
		if(weight==WEIGHT_ONE)		WARNNIG_VALUE_ONE= degree;
		if(weight==WEIGHT_TWO)		WARNNIG_VALUE_TWO= degree;
		if(weight==WEIGHT_THREE)	WARNNIG_VALUE_THREE= degree;
		if(weight==WEIGHT_FOUR)		WARNNIG_VALUE_FOUR= degree;
}
/*
cs1----cs4
weight1--weight4
*/
int get_weight_sensor_value(WEIGHT_type weight)
{
		int value=0; 
	
		if(weight==WEIGHT_ONE)		value = AD_Sensor_Get_Data(AD1_CS);
		if(weight==WEIGHT_TWO)		value = AD_Sensor_Get_Data(AD2_CS);
		if(weight==WEIGHT_THREE)	value = AD_Sensor_Get_Data(AD3_CS);
		if(weight==WEIGHT_FOUR)		value = AD_Sensor_Get_Data(AD4_CS);

		return value;
}	


//value->gram
static __inline int weight_AD_value_change_to_unit(int value)
{
	//0.58--- 实际测量估算
	return (int)(value/0.855);
}

/*获得指定重力传感器的gram值
*/
int Get_weight_current_gram(WEIGHT_type weight)
{
	int value=0;
	int gram=0;
	value = get_weight_sensor_value(weight);
	gram=weight_AD_value_change_to_unit(value);
	
	return gram;
}



//克->value
static __inline int weight_unit_change_to_AD_value(int gram)
{
	//0.58--- 实际测量估算
	return (int)(gram*0.855);
}	
/**************************interface for extern******************************************/
void Set_Weight_Sensor_Warnning_Line(WEIGHT_type weight,int gram)
{
		int degree=weight_unit_change_to_AD_value(gram);
		
		__set_weight_sensor_warnning_line(weight, degree);

}
//result 1----warning    0----normal

uint8_t Get_Single_Weight_Sensor_Warnning_Result(WEIGHT_type weight)
{		
		uint8_t ret=0;
		int value = get_weight_sensor_value(weight);
		
		switch(weight)
		{
			case WEIGHT_ONE:
					if(value>=WARNNIG_VALUE_ONE)ret=1;
					break;
			case WEIGHT_TWO:
					if(value>=WARNNIG_VALUE_TWO)ret=1;
					break;		
			case WEIGHT_THREE:
					if(value>=WARNNIG_VALUE_THREE)ret=1;
					break;		
			case WEIGHT_FOUR:
					if(value>=WARNNIG_VALUE_FOUR)ret=1;
					break;		
			
			default:
					ret=0;
		}
		return ret;
}


/*
0000 xxxx
bit x----result
w4     w3     w2     w1 
bit3   bit2   bit1   bit0
*/
uint8_t Get_All_Weight_Sensor_Warnning_Result(void)
{
		uint8_t value=0;
	
		value|=Get_Single_Weight_Sensor_Warnning_Result(WEIGHT_ONE)<<0;
		value|=Get_Single_Weight_Sensor_Warnning_Result(WEIGHT_TWO)<<1;
		value|=Get_Single_Weight_Sensor_Warnning_Result(WEIGHT_THREE)<<2;
		value|=Get_Single_Weight_Sensor_Warnning_Result(WEIGHT_FOUR)<<3;
	
		return value;
}	


/**************************weight interface for RS232******************************************/

