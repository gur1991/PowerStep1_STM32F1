#include "uart4.h"
#include "delay.h"
#include "uart_command_control_protocol.h"
#include "config.h"

UART_HandleTypeDef UART4_Handler;  

//���ջ����� 	
u8 UART4_RX_BUF[LEN_MAX_UART4];  	
//���յ������ݳ���
int UART4_RX_CNT=0;  
u8 ARM_RS232_ASK =0;

void UART4_IRQHandler(void)
{
    u8 res;
    if((__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�
	 {	 	
		HAL_UART_Receive(&UART4_Handler,&res,1,30);//115200 256byte ��Ҫ20ms�����ڸ�30ms
			
		//����ϴ�ָ����û�н������Ͳ�������µ�ָ�� 
		if(ARM_RS232_ASK)return;
		 
		 
		if(UART4_RX_CNT<LEN_MAX_UART4)
		{
			UART4_RX_BUF[UART4_RX_CNT]=res;		//��¼���յ���ֵ
			//LOGD("r[%d]0x%x\r\n",UART4_RX_CNT,res);
			UART4_RX_CNT++;						//������������1	
			
		}
/*		
		if(UART4_RX_CNT == 2)
		{
				if( UART4_RX_BUF[0]==START_UART_VALUE0 && UART4_RX_BUF[1]==START_UART_VALUE1){ ;}
				else {UART4_RX_BUF[0]= UART4_RX_BUF[1];UART4_RX_CNT=1; LOGE("fuck\r\n");}
		}
*/
//		else 
			if(UART4_RX_CNT==sizeof(Powerstep1_contorl_motor_command_t))
		{
					ARM_RS232_ASK=1;
		}

	}
} 

//��ʼ��IO ����2
//bound:������
void UART4_Init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	//__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_UART4_CLK_ENABLE();			

    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_11;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

	
	
    //USART ��ʼ������
	UART4_Handler.Instance=UART4;			        //USART2
	UART4_Handler.Init.BaudRate=bound;		        //������
	UART4_Handler.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	UART4_Handler.Init.StopBits=UART_STOPBITS_1;		//һ��ֹͣλ
	UART4_Handler.Init.Parity=UART_PARITY_NONE;		//����żУ��λ
	UART4_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	UART4_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	UART4_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART4_Handler);			        //HAL_UART_Init()��ʹ��USART2
  
  __HAL_UART_DISABLE_IT(&UART4_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&UART4_Handler,UART_IT_RXNE);//���������ж�
	HAL_NVIC_EnableIRQ(UART4_IRQn);				        //ʹ��UART4�ж�
	HAL_NVIC_SetPriority(UART4_IRQn,3,3);			        //��ռ���ȼ�3�������ȼ�3

}

//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void UART4_Send_Data(u8 *buf,int len)
{
	HAL_UART_Transmit(&UART4_Handler,buf,len,100);//����4��������
	UART4_RX_CNT=0;
}
//buf:���ջ����׵�ַ
//len:���������ݳ���
void UART4_Receive_Data(u8 *buf,int RxLen, int *len)
{
	int rxlen=UART4_RX_CNT;
	int i=0;
	*len=0;				
	if(rxlen<RxLen)
	{
		*len=rxlen;
		return;
	}
		
	else if(rxlen==UART4_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<RxLen;i++)
		{
			buf[i]=UART4_RX_BUF[i];	
		}		
		*len=UART4_RX_CNT;	//��¼�������ݳ���
		UART4_RX_CNT=0;		//����
	}
} 
void Uart4_Rx_Clear(void)
{
	UART4_RX_CNT=0;			//����
}	

