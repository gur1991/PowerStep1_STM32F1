#ifndef __UART2_H
#define __UART2_H
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

#define MAX_LENGTH 128

extern u8 USART2_RX_BUF[MAX_LENGTH]; 	
extern int USART2_RX_CNT;   			

extern int FLAG_UART_MASTER;
extern int FLAG_UART_CHEMINERT;
extern int FLAG_RECEIVE_ACK;

void UART2_Init(u32 bound);
void UART2_Send_Data(u8 *buf,int len);
void UART2_Receive_Data(u8 *buf,int *len);	
#endif
