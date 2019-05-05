#ifndef __UART3_H
#define __UART3_H
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

extern u8 FLAG_UART_SLAVE;
extern UART_HandleTypeDef USART3_Handler;
extern u8 UART3_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern u8 UART3_RX_CNT;   			//���յ������ݳ���

//ģʽ����
#define UART3_TX_EN		1 //PDout(7)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ�����EN_USART2_RXΪ1����������Ϊ0
#define EN_USART3_RX 	1			//0,������;1,����.

void UART3_Init(u32 bound);
void UART3_Send_Data(u8 *buf,u8 len);
void UART3_Receive_Data(u8 *buf,u8 *len);	
#endif
