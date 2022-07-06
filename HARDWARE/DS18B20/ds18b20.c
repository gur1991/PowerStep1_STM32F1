#include "ds18b20.h"
#include "delay.h"
#include "config.h"

static int CHOOSE_DS18B20=1;

//复位DS18B20
void DS18B20_Rst(void)	   
{
	
#if USE_KEEP_TEMPERATURE_BOARD		
if(CHOOSE_DS18B20==1){	
	DS18B20_IO_OUT();   //设置为输出
	DS18B20_DQ_OUT=0;  	//拉低DQ
	delay_us(750);      //拉低750us
	DS18B20_DQ_OUT=1;  	//DQ=1 
	delay_us(15);       //15US
}else{
	DS18B20_IO_OUT_T2();   //设置为输出
	DS18B20_DQ_OUT_T2=0;  	//拉低DQ
	delay_us(750);      //拉低750us
	DS18B20_DQ_OUT_T2=1;  	//DQ=1 
	delay_us(15);       //15US
}	
#endif

}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void) 	   
{  
#if USE_KEEP_TEMPERATURE_BOARD	
	u8 retry=0;
if(CHOOSE_DS18B20==1){		
	DS18B20_IO_IN();    //设置为输入
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};
}else
{
	DS18B20_IO_IN_T2();    //设置为输入
    while (DS18B20_DQ_IN_T2&&retry<200)
	{
		retry++;
		delay_us(1);
	};
}	
	if(retry>=200)return 1;
	else retry=0;
if(CHOOSE_DS18B20==1){	
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
}else{
    while (!DS18B20_DQ_IN_T2&&retry<240)
	{
		retry++;
		delay_us(1);
	};
}	
	if(retry>=240)return 1;	    

#endif

	return 0;
}

//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit(void) 
{
	u8 data=0;
	
#if USE_KEEP_TEMPERATURE_BOARD	
if(CHOOSE_DS18B20==1){	
	DS18B20_IO_OUT();   //设置为输出
	DS18B20_DQ_OUT=0; 
	delay_us(2);
	DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();    //设置为输入
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
	else data=0;	 
	delay_us(50);
}else{
	DS18B20_IO_OUT_T2();   //设置为输出
	DS18B20_DQ_OUT_T2=0; 
	delay_us(2);
	DS18B20_DQ_OUT_T2=1; 
	DS18B20_IO_IN_T2();    //设置为输入
	delay_us(12);
	if(DS18B20_DQ_IN_T2)data=1;
	else data=0;	 
	delay_us(50);
}	

#endif
	return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
u8 DS18B20_Read_Byte(void)   
{        
	u8 i,j,dat;
	dat=0;
#if USE_KEEP_TEMPERATURE_BOARD	
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }
#endif	
	return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)     
 {    
#if USE_KEEP_TEMPERATURE_BOARD	 
    u8 j;
    u8 testb;
if(CHOOSE_DS18B20==1){		 
    DS18B20_IO_OUT();     //设置为输出
}else{
		DS18B20_IO_OUT_T2();     //设置为输出
}	
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // 写1
        {
						if(CHOOSE_DS18B20==1){
								DS18B20_DQ_OUT=0;
								delay_us(2);                            
								DS18B20_DQ_OUT=1;
								delay_us(60); 
						}else{
								DS18B20_DQ_OUT_T2=0;
								delay_us(2);                            
								DS18B20_DQ_OUT_T2=1;
								delay_us(60); 
						}			
        }
        else            //写0
        {
						if(CHOOSE_DS18B20==1){
								DS18B20_DQ_OUT=0;
								delay_us(60);             
								DS18B20_DQ_OUT=1;
								delay_us(2);         
						}else{
								DS18B20_DQ_OUT_T2=0;
								delay_us(60);             
								DS18B20_DQ_OUT_T2=1;
								delay_us(2);  
						}			
        }
    }
#endif	
}
 
//开始温度转换
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)
{
#if USE_KEEP_TEMPERATURE_BOARD	
	GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOE_CLK_ENABLE();			//开启GPIOG时钟
	
  GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;           //PG6 7
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
  HAL_GPIO_Init(GPIOE,&GPIO_Initure);     //初始化
 
	DS18B20_Rst();
	return DS18B20_Check();
#endif
	return 0;	
}

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 

short __DS18B20_Get_Temp__(int chip)
{
#if USE_KEEP_TEMPERATURE_BOARD	
    u8 temp;
    u8 TL,TH;
    short tem;
		CHOOSE_DS18B20=chip;
	
    DS18B20_Start ();           //开始转换
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert	    
    TL=DS18B20_Read_Byte();     // LSB   
    TH=DS18B20_Read_Byte();     // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(double)tem*0.625;//转换     
	if(temp)return tem; //返回温度值
	else return -tem;    
#endif
		
		return 0;
}	

short DS18B20_Get_Temp(int chip)
{
	int ret=0;
	short value[3]={0,0,0};
	int i=0;
	
	for(i=0;i<3;i++)
	{
		value[i]=__DS18B20_Get_Temp__(chip);
	}
	
	return (value[0]+value[1]+value[2])/3.0;
}






