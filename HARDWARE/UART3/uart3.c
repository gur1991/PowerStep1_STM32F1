#include "uart3.h"
#include "usart.h"
#include "delay.h"
#include "uart_command_control_protocol.h"
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

UART_HandleTypeDef USART3_Handler;  //USART2���(����RS485)
//#define  RXBUFFERSIZE_UART3 1
//u8 aRxBuffer_uart3[RXBUFFERSIZE_UART3];//HAL��ʹ�õĴ��ڽ��ջ���

#if EN_USART3_RX   		//���ʹ���˽���   	  
//���ջ����� 	
u8 UART3_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 UART3_RX_CNT=0;  
u8 FLAG_UART_SLAVE =0;

void USART3_IRQHandler(void)
{
    u8 res;
    if((__HAL_UART_GET_FLAG(&USART3_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�
	{	 	
		HAL_UART_Receive(&USART3_Handler,&res,1,1000);
		if(UART3_RX_CNT<64)
		{
			UART3_RX_BUF[UART3_RX_CNT]=res;		//��¼���յ���ֵ
			//printf("slave uart %d  UART3_RX_BUF:%d\r\n",UART3_RX_BUF[UART3_RX_CNT],UART3_RX_CNT);
			//printf(" rx_buf[%d] %d \r\n",UART3_RX_CNT,UART3_RX_BUF[UART3_RX_CNT]);
			UART3_RX_CNT++;						//������������1	
		} 
		
		if(UART3_RX_CNT>=3&&UART3_RX_BUF[UART3_RX_CNT-1]==OVER_UART_VALUE1&&UART3_RX_BUF[UART3_RX_CNT-2]==OVER_UART_VALUE0){
					//printf(" slave over \r\n");
					//protocol_handle_uart_powerstep01_plain_slave_cmd();
					FLAG_UART_SLAVE=1;
		}

	} 
} 
/*

//����1�жϷ������
void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif

	HAL_UART_IRQHandler(&USART3_Handler);	//����HAL���жϴ����ú���
	timeout=0;
   while (HAL_UART_GetState(&USART3_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&USART3_Handler, (u8 *)aRxBuffer_UART3, RXBUFFERSIZE_UART3) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
*/
#endif

#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB1Periph_USART3            ((uint32_t)0x00040000)
#define GPIO_PartialRemap_USART3    ((uint32_t)0x00140010)  /*!< USART3 Partial Alternate Function mapping */

//��ʼ��IO ����2
//bound:������
void UART3_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART3ʱ��
	//RCC_APB2PeriphClockCmd();
//	GPIO_PinRemapConfig();
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��
 // RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	
#if 1  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_11;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
#else	
//PDF ����
		GPIO_Initure.Pin = GPIO_PIN_11;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
#endif	
	
	
    //USART ��ʼ������
	USART3_Handler.Instance=USART3;			        //USART2
	USART3_Handler.Init.BaudRate=bound;		        //������
	USART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART3_Handler.Init.StopBits=UART_STOPBITS_1;		//һ��ֹͣλ
	USART3_Handler.Init.Parity=UART_PARITY_NONE;		//����żУ��λ
	USART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	USART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	USART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART3_Handler);			        //HAL_UART_Init()��ʹ��USART2
  //HAL_UART_Receive_IT(&USART3_Handler, (u8 *)aRxBuffer_UART3, RXBUFFERSIZE_UART3);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  __HAL_UART_DISABLE_IT(&USART3_Handler,UART_IT_TC);
#if EN_USART3_RX
	__HAL_UART_ENABLE_IT(&USART3_Handler,UART_IT_RXNE);//���������ж�
	HAL_NVIC_EnableIRQ(USART3_IRQn);				        //ʹ��USART1�ж�
	HAL_NVIC_SetPriority(USART3_IRQn,3,3);			        //��ռ���ȼ�3�������ȼ�3
#endif	
//	UART3_TX_EN=0;											//Ĭ��Ϊ����ģʽ		
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void UART3_Send_Data(u8 *buf,u8 len)
{
//	UART3_TX_EN=1;			//����Ϊ����ģʽ
	HAL_UART_Transmit(&USART3_Handler,buf,len,1000);//����2��������
//	UART3_TX_EN=0;				//����Ϊ����ģʽ	
	UART3_RX_CNT=0;
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void UART3_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=UART3_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	//delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==UART3_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=UART3_RX_BUF[i];	
		}		
		*len=UART3_RX_CNT;	//��¼�������ݳ���
		UART3_RX_CNT=0;		//����
	}
} 
