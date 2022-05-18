#include "uart2.h"
#include "delay.h"
#include "uart_command_control_protocol.h"
#include "config.h"

UART_HandleTypeDef USART2_Handler;  

u8 USART2_RX_BUF[MAX_LENGTH];  	
int USART2_RX_CNT=0; 
u8 FLAG_RECEIVE_RFID=0;
u8 FLAG_SCAN_OK=0;

void USART2_IRQHandler(void)
{
    u8 res;	  
    if((__HAL_UART_GET_FLAG(&USART2_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�
	{	
		HAL_UART_Receive(&USART2_Handler,&res,1,2);
		if(USART2_RX_CNT<MAX_LENGTH)
		{
			USART2_RX_BUF[USART2_RX_CNT]=res;		//��¼���յ���ֵ
			USART2_RX_CNT++;						//������������1 
		}


//RFID�˴���ȡ��Ҫ�޸�		
#if USE_AUTOMATIC_INJECTION_BOARD		
		if(USART2_RX_CNT>=7 && USART2_RX_BUF[0]==0xff)
		{
				if(USART2_RX_CNT == USART2_RX_BUF[1]+7)FLAG_RECEIVE_RFID=1;
		}
		
		if(USART2_RX_CNT>=3 && USART2_RX_BUF[USART2_RX_CNT-1]==0x0a
												&& USART2_RX_BUF[USART2_RX_CNT-2]==0x0d)
		{
				FLAG_SCAN_OK=1;
		}else if(USART2_RX_CNT<3)
		{
				FLAG_SCAN_OK=0;
		}
#endif		
		
	} 
}    


void UART2_Config_Baurd(u32 bound)
{
	USART2_Handler.Init.BaudRate=bound;		        
	HAL_UART_Init(&USART2_Handler);			     
}


//��ʼ��IO ����2
//bound:������
void UART2_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
	__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_2; 			//PA2
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;		//����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA2
	
	GPIO_Initure.Pin=GPIO_PIN_3; 			//PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//��������
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA3
/*	
	//PD7���������485ģʽ����  
    GPIO_Initure.Pin=GPIO_PIN_7; 			//PD7
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
  */  
    //USART ��ʼ������
	USART2_Handler.Instance=USART2;			        //USART2
	USART2_Handler.Init.BaudRate=bound;		        //������
	USART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART2_Handler.Init.StopBits=UART_STOPBITS_1;		//һ��ֹͣλ
	USART2_Handler.Init.Parity=UART_PARITY_NONE;		//����żУ��λ
	USART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	USART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	USART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART2_Handler);			        //HAL_UART_Init()��ʹ��USART2	
		
  __HAL_UART_DISABLE_IT(&USART2_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&USART2_Handler,UART_IT_RXNE);//���������ж�
	HAL_NVIC_EnableIRQ(USART2_IRQn);				        //ʹ��USART1�ж�
	HAL_NVIC_SetPriority(USART2_IRQn,3,3);			        //��ռ���ȼ�3�������ȼ�3
}


//��ʼ��IO ����2
//bound:������
void UART2_Init_Check(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
	__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_2; 			//PA2
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;		//����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA2
	
	GPIO_Initure.Pin=GPIO_PIN_3; 			//PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//��������
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA3
	

    //USART ��ʼ������
	USART2_Handler.Instance=USART2;			        //USART2
	USART2_Handler.Init.BaudRate=bound;		        //������
	USART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART2_Handler.Init.StopBits=UART_STOPBITS_1;		//һ��ֹͣλ
	USART2_Handler.Init.Parity=UART_PARITY_EVEN;		//����żУ��λ
	USART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	USART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	USART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART2_Handler);			        //HAL_UART_Init()��ʹ��USART2
    
  __HAL_UART_DISABLE_IT(&USART2_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&USART2_Handler,UART_IT_RXNE);//���������ж�
	HAL_NVIC_EnableIRQ(USART2_IRQn);				        //ʹ��USART1�ж�
	HAL_NVIC_SetPriority(USART2_IRQn,3,3);			        //��ռ���ȼ�3�������ȼ�3
}

void UART2_Send_Data(u8 *buf,int len)
{
	HAL_UART_Transmit(&USART2_Handler,buf,len,10);//����2��������
	USART2_RX_CNT=0;
}

void UART2_Receive_Data(u8 *buf,int *len)
{
	int i=0;
	for(i=0;i<USART2_RX_CNT;i++)
	{
		buf[i]=USART2_RX_BUF[i];	
	}		
	*len=USART2_RX_CNT;	
	
	USART2_RX_CNT=0;	
} 

void Uart2_Rx_Clear(void)
{
	USART2_RX_CNT=0;		//����
}	
