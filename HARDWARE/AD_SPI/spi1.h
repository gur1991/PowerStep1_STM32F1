#ifndef __SPI1_H
#define __SPI1_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//SPI��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
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



//extern SPI_HandleTypeDef SPI1_Handler;  //SPI���

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
