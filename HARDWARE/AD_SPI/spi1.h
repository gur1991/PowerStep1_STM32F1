#ifndef __SPI1_H
#define __SPI1_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//SPI驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

typedef enum{
	AD1_CS=10,
	AD2_CS,
	AD3_CS,
	AD4_CS,
	AD5_CS,
	AD6_CS,
}AD_type;

typedef enum{
	CS_LOW=0,
	CS_HIGH=1,
}AD_LEVEL_type;



//extern SPI_HandleTypeDef SPI1_Handler;  //SPI句柄

//void SPI1_Init(void);
//void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
//u8 SPI1_ReadWriteByte(u8 TxData);

u8 AD_SENSOR_init(void);
int AD_SENSOR_GET_DATA(AD_type cs);

#define	SPI1_AD1_CS 		PCout(4)  		//AD1_CS
#define	SPI1_AD2_CS 		PCout(5)  		//AD2_CS
#define	SPI1_AD3_CS 		PBout(0)  		//AD3_CS
#define	SPI1_AD4_CS 		PBout(1)  		//AD4_CS
#define	SPI1_AD5_CS 		PBout(2)  		//AD5_CS
#define	SPI1_AD6_CS 		PFout(11)  		//AD6_CS

void HAL_SPI_MspInit_Extern(SPI_HandleTypeDef *hspi);
#endif
