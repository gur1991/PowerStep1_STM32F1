#include "temperature.h"

static int TMP_TEMPERATURE_VALUE_ONE=0; 
static int TMP_TEMPERATURE_VALUE_TWO=0;
static int TMP_TEMPERATURE_VALUE=0;

static  int temperature_change_to_value(int temperature)
{
	printf("tmp:%d\r\n",temperature);
	switch(temperature){
		case 545:return 9235;
		case 540:return 9162;
		case 535:return	9000;
		case 530:return	8902;
	  case 525:return	8815;
		case 520:return	8700;
		case 515:return	8632;
		case 510:return 8500;
		case 505:return 8450;
		case 500:return 8387;
		case 495:return 8275;
		case 490:return 8200;
		case 485:return 8100;
		case 480:return 8015;
		case 475:return 7903;
		case 470:return 7826;
		case 465:return 7741;
		case 460:return 7683;
		case 455:return 7555;
		case 450:return 7311;
		case 445:return 7265;
		case 440:return 7139;
		case 435:return 7000;
		case 430:return 6903;
		case 425:return 6858;
		case 420:return 6774;
		case 415:return 6701;
		case 410:return 6585;		
		case 405:return 6483;
		case 400:return 6397;
		case 395:return 6335;
		case 390:return 6239;
		case 385:return 6175;
		case 380:return 6094;
		case 375:return 6020;
		case 370:return 5900;
		case 365:return 5770;
		case 360:return 5519;
		case 355:return 5379;
		case 350:return 5275;
		case 345:return 5220;
		case 340:return 5180;
		case 335:return 5080;
		case 330:return 5050;
	}
	return -1;
}

static int value_change_to_temperature(int value)
{
	if(value>9235)return 550;
	if(value>9162)return 545;
	if(value>9000)return 540;
	if(value>8902)return 535;
	if(value>8815)return 530;
	if(value>8700)return 525;
	if(value>8632)return 520;
	if(value>8500)return 515;
	if(value>8450)return 510;
	if(value>8387)return 505;
	if(value>8275)return 500;
	if(value>8200)return 495;
	if(value>8100)return 490;
	if(value>8015)return 485;
	if(value>7903)return 480;
	if(value>7826)return 475;
	if(value>7741)return 470;
	if(value>7683)return 465;
	if(value>7555)return 460;
	if(value>7311)return 455;
	if(value>7265)return 450;
	if(value>7139)return 445;
	if(value>7000)return 440;
	if(value>6903)return 435;
	if(value>6858)return 430;
	if(value>6774)return 425;
	if(value>6701)return 420;
	if(value>6585)return 415;
	if(value>6483)return 410;
	if(value>6397)return 405;
	if(value>6335)return 400;	
	if(value>6239)return 395;		
	if(value>6175)return 390;
	if(value>6094)return 385;
	if(value>6020)return 380;
	if(value>5900)return 375;	
	if(value>5770)return 370;		
	if(value>5519)return 365;
	if(value>5379)return 360;
	if(value>5275)return 355;	
	if(value>5220)return 350;
	if(value>5220)return 345;
	if(value>5180)return 340;
	if(value>5080)return 335;		
	if(value>5050)return 330;		
	return 0;
}
int get_temperature_degree(AD_type AD_CS){
		int value=0,temperature=0;
		value=AD_SENSOR_GET_DATA(AD_CS);
		temperature=value_change_to_temperature(value);
		//printf("get_temperature_degree:%d  %d\r\n",value,temperature);
		return temperature;
}


static int set_calculate_duty_cycle(int value,int list_value)
{
	int duty_cycle=0;
			if(list_value-value>1000){duty_cycle=0;return duty_cycle;}
			if(list_value-value>500){duty_cycle=250;return duty_cycle;}
			if(list_value-value>300){duty_cycle=350;return duty_cycle;}
			if(list_value-value>100){duty_cycle=400;return duty_cycle;}
			if(list_value-value>30){duty_cycle=480;return duty_cycle;}
			if(list_value-value>20){duty_cycle=495;return duty_cycle;}
			if(list_value-value<=20){duty_cycle=500;return duty_cycle;}
			return 500;	
}


//两个温度计和与设定值对比
static  void __keep_temperature_degree(void)
{
		int value=0,value1=0,value2=0;
		int duty_cycle=0;
		int last_duty_cycle=0;

		value1=AD_SENSOR_GET_DATA(AD1_CS);
		value2=AD_SENSOR_GET_DATA(AD2_CS);
		value=value1+value2;
	
		//设定值与查表值对比
		if(value>=TMP_TEMPERATURE_VALUE)
		{
				TIM_SetTIM3Compare4(500);//占空比为0，停止加热
		}
		else if(value<TMP_TEMPERATURE_VALUE)
		{
				duty_cycle=set_calculate_duty_cycle(value,TMP_TEMPERATURE_VALUE);
				printf("set duty_cycle:%d\r\n",duty_cycle);
				if(last_duty_cycle==duty_cycle)return;
				last_duty_cycle=duty_cycle;
				TIM_SetTIM3Compare4(duty_cycle);
		}	
}	
/*
void set_temperature_degree(int degree,AD_type AD_CS){
		int value=0;
		int duty_cycle=0;
		int last_duty_cycle=0;
	
		TMP_TEMPERATURE_VALUE_ONE=temperature_change_to_value(degree);
		
		value=AD_SENSOR_GET_DATA(AD_CS);
		
	
		//设定值与查表值对比
		if(value>=TMP_TEMPERATURE)
		{
				TIM_SetTIM3Compare4(500);//占空比为0，停止加热
		}
		else if(value<TMP_TEMPERATURE)
		{
				duty_cycle=set_calculate_duty_cycle(value,TMP_TEMPERATURE);
				printf("set duty_cycle:%d\r\n",duty_cycle);
				if(last_duty_cycle==duty_cycle)return;
				last_duty_cycle=duty_cycle;
				TIM_SetTIM3Compare4(duty_cycle);
		}	

}
*/
/************************temperature for extern***************************************/

/*
AD1_CS---tm1
AD2_CS---tm2
*/

int Get_Single_Temperature_Degree(TMEPERATURE_type devices)
{
		int temperature=0;
	
		if(TMEPERATURE_ONE==devices)temperature=get_temperature_degree(AD1_CS);
		if(TMEPERATURE_TWO==devices)temperature=get_temperature_degree(AD2_CS);
	
		return temperature;
}	

void Set_Single_Temperature_Degree(int degree, TMEPERATURE_type devices)
{		
		if(TMEPERATURE_ONE==devices)TMP_TEMPERATURE_VALUE_ONE=temperature_change_to_value(degree);
		if(TMEPERATURE_TWO==devices)TMP_TEMPERATURE_VALUE_TWO=temperature_change_to_value(degree);
		TMP_TEMPERATURE_VALUE=TMP_TEMPERATURE_VALUE_ONE+TMP_TEMPERATURE_VALUE_TWO;
}	


void Main_Keep_Temperature_Degree(void)
{
		__keep_temperature_degree();
}	
