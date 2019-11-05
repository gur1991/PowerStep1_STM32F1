#ifndef __DS18B20_H
#define __DS18B20_H
#include "sys.h"

//G6 G7->E2 E3
//IO方向设置
#define DS18B20_IO_IN()  {GPIOE->CRL&=0XFFFFF0FF;GPIOE->CRL|=8<<8;}
#define DS18B20_IO_OUT() {GPIOE->CRL&=0XFFFFF0FF;GPIOE->CRL|=3<<8;}

#define DS18B20_IO_IN_T2()  {GPIOE->CRL&=0XFFFF0FFF;GPIOE->CRL|=(u32)8<<12;}
#define DS18B20_IO_OUT_T2() {GPIOE->CRL&=0XFFFF0FFF;GPIOE->CRL|=(u32)3<<12;}

//IO操作函数											   
#define	DS18B20_DQ_OUT PEout(2) 
#define	DS18B20_DQ_IN  PEin(2)  

#define	DS18B20_DQ_OUT_T2 PEout(3) 
#define	DS18B20_DQ_IN_T2  PEin(3)  
   	
u8 DS18B20_Init(void);			//初始化DS18B20

//chip 1 2
short DS18B20_Get_Temp(int chip);	//获取温度




#endif
