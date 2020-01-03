
#include "uart5.h"
#include "uart_command_control_protocol.h"

UART_HandleTypeDef UART5_Handler;  

u8 UART5_RX_BUF[LEN_MAX_UART5];  	
int UART5_RX_CNT=0;  
int MINI_RS232_ASK=0;

void UART5_IRQHandler(void)
{
    u8 res;
    if((__HAL_UART_GET_FLAG(&UART5_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�
	 {	 	
		HAL_UART_Receive(&UART5_Handler,&res,1,10);//115200 256byte ��Ҫ20ms�����ڸ�30ms
		if(UART5_RX_CNT<LEN_MAX_UART5)
		{
			UART5_RX_BUF[UART5_RX_CNT]=res;		//��¼���յ���ֵ
			UART5_RX_CNT++;						//������������1	
		} 
		if(UART5_RX_CNT==sizeof(Powerstep1_contorl_motor_command_t))
		{
					MINI_RS232_ASK=1;
		}

	}
} 

void UART5_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_UART5_CLK_ENABLE();			

    GPIO_Initure.Pin = GPIO_PIN_12;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_2;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

	
	UART5_Handler.Instance=UART5;			        //UART5
	UART5_Handler.Init.BaudRate=bound;		        //������
	UART5_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	UART5_Handler.Init.StopBits=UART_STOPBITS_1;		//һ��ֹͣλ
	UART5_Handler.Init.Parity=UART_PARITY_NONE;		//����żУ��λ
	UART5_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	UART5_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	UART5_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART5_Handler);			        //HAL_UART_Init()��ʹ��USART2
  
	//HAL_UART_Receive_IT(&UART5_Handler, &aRxBuffer_UART5, 1);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  __HAL_UART_DISABLE_IT(&UART5_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&UART5_Handler,UART_IT_RXNE);//���������ж�
	HAL_NVIC_EnableIRQ(UART5_IRQn);				        //ʹ��UART5�ж�
	HAL_NVIC_SetPriority(UART5_IRQn,3,3);			        //��ռ���ȼ�3�������ȼ�3

}

void UART5_Send_Data(u8 *buf,int len)
{
	HAL_UART_Transmit(&UART5_Handler,buf,len,100);//����4��������
	UART5_RX_CNT=0;
}
//buf:���ջ����׵�ַ
//len:���������ݳ���
void UART5_Receive_Data(u8 *buf,int *len)
{
	int rxlen=UART5_RX_CNT;
	int i=0;
	*len=0;				//Ĭ��Ϊ0
	if(rxlen==UART5_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=UART5_RX_BUF[i];	
		}		
		*len=UART5_RX_CNT;	//��¼�������ݳ���
		UART5_RX_CNT=0;		//����
	}
} 

void Uart5_Rx_Clear(void)
{
	UART5_RX_CNT=0;		//����
}	