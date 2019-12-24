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

u8 Weight_Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();           
    
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_SET);
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速       
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);     //初始化
	

		SPI3_Init();		   			        //初始化SPI
		SPI3_SetSpeed(SPI_BAUDRATEPRESCALER_64); //设置为42M时钟,高速模式
	return 0;
}





static __inline void Set_AD_CS(AD_type cs,AD_LEVEL_type level)
{
		switch(cs){
			case AD1_CS:WEIGHT_AD1_CS=level;break;
			case AD2_CS:WEIGHT_AD2_CS=level;break;
			case AD3_CS:WEIGHT_AD3_CS=level;break;
			case AD4_CS:WEIGHT_AD4_CS=level;break;
		}
}	


void SPI3_Transfer(u8* txData,u8*rxData,int len,AD_type cs){
		int i;
		Set_AD_CS(cs,CS_LOW);
		for(i=0;i<len;i++){
				rxData[i]=SPI3_ReadWriteByte(txData[i]);
		}
		Set_AD_CS(cs,CS_HIGH);
}

int AD_Sensor_Get_Data(AD_type cs){
		u8 txbuf[3]={0x00,0x00,0x00};
		u8 rxbuf[3]={0,0,0};
		u8 bit_low=0;
		u8 bit_high=0;
		u16 value=0;
		
		SPI3_Transfer(txbuf,rxbuf,sizeof(txbuf),cs);
				
		bit_high|=rxbuf[0]<<6;//取低2bit
		bit_high|=rxbuf[1]>>2;//取高6bit
		bit_low|=rxbuf[1]<<6;
		bit_low|=rxbuf[2]>>2;
		
		value=bit_high;
		value<<=8;
		value|=bit_low;
		
//		printf("here value:%d \r\n",value);
		return value;
}




