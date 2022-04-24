#include "uart3.h"
#include "usart.h"
#include "delay.h"
#include "uart_command_control_protocol.h"

//����3ֻ���룬û��ʹ�ã�����2��ʹ�ô���3����ı���


UART_HandleTypeDef USART3_Handler; 

u8 UART3_RX_BUF[LEN_MAX_UART3];  
int UART3_RX_CNT=0;  


void USART3_IRQHandler(void)
{
    u8 res;
    if((__HAL_UART_GET_FLAG(&USART3_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�
	{	 	
		HAL_UART_Receive(&USART3_Handler,&res,1,2);
		if(UART3_RX_CNT<LEN_MAX_UART3)
		{
			UART3_RX_BUF[UART3_RX_CNT]=res;		//��¼���յ���ֵ
			UART3_RX_CNT++;						//������������1	
		} 
		

	}
} 

//��ʼ��IO ����2
//bound:������
void UART3_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART3ʱ��
	__HAL_RCC_AFIO_CLK_ENABLE();
#if 1  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_Initure.Pull = GPIO_NOPULL;
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
	USART3_Handler.Init.Parity= UART_PARITY_NONE;//UART_PARITY_NONE;		//����żУ��λ
	USART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	USART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	USART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART3_Handler);			        //HAL_UART_Init()��ʹ��USART2
  //HAL_UART_Receive_IT(&USART3_Handler, (u8 *)aRxBuffer_UART3, RXBUFFERSIZE_UART3);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  __HAL_UART_DISABLE_IT(&USART3_Handler,UART_IT_TC);
#if EN_USART3_RX
	__HAL_UART_ENABLE_IT(&USART3_Handler,UART_IT_RXNE);//���������ж�
	HAL_NVIC_EnableIRQ(USART3_IRQn);				        //ʹ��USART1�ж�
	HAL_NVIC_SetPriority(USART3_IRQn,2,3);			        //��ռ���ȼ�3�������ȼ�3
#endif	
}

//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void UART3_Send_Data(u8 *buf,int len)
{
	HAL_UART_Transmit(&USART3_Handler,buf,len,10);
	UART3_RX_CNT=0;
}
//buf:���ջ����׵�ַ
//len:���������ݳ���
void UART3_Receive_Data(u8 *buf,int *len)
{
	int rxlen=UART3_RX_CNT;
	int i=0;
	*len=0;				//Ĭ��Ϊ0
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
void Uart3_Rx_Clear(void)
{
	UART3_RX_CNT=0;		//����
}	
