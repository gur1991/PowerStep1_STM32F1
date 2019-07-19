#ifndef __DS18B20_H
#define __DS18B20_H
#include "sys.h"

//G6 G7
//IO��������
#define DS18B20_IO_IN()  {GPIOG->CRH&=0XF0FFFFFF;GPIOG->CRL|=8<<24;}
#define DS18B20_IO_OUT() {GPIOG->CRH&=0XF0FFFFFF;GPIOG->CRL|=3<<24;}

#define DS18B20_IO_IN_T2()  {GPIOG->CRH&=0X0FFFFFFF;GPIOG->CRL|=8<<28;}
#define DS18B20_IO_OUT_T2() {GPIOG->CRH&=0X0FFFFFFF;GPIOG->CRL|=3<<28;}

//IO��������											   
#define	DS18B20_DQ_OUT PGout(6) 
#define	DS18B20_DQ_IN  PGin(6)  

#define	DS18B20_DQ_OUT_T2 PGout(7) 
#define	DS18B20_DQ_IN_T2  PGin(7)  
   	
u8 DS18B20_Init(void);			//��ʼ��DS18B20

//chip 1 2
short DS18B20_Get_Temp(int chip);	//��ȡ�¶�




#endif
