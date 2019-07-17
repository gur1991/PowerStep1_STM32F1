#ifndef __UART4_H
#define __UART4_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//RS485��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


#define LEN_MAX_UART4 256
extern u8 ARM_RS232_ASK;
extern UART_HandleTypeDef UART4_Handler;
extern u8 UART4_RX_BUF[LEN_MAX_UART4]; 		
extern int UART4_RX_CNT;   			

//ģʽ����
#define UART4_TX_EN		1 
#define EN_USART4_RX 	1		

void UART4_Init(u32 bound);
void UART4_Send_Data(u8 *buf,int len);
void UART4_Receive_Data(u8 *buf,int *len);

void HAL_UART_RxCpltCallback_Uart4(void);
#endif
