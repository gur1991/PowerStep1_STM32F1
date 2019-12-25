#include "uart2.h"
#include "delay.h"
#include "uart_command_control_protocol.h"


UART_HandleTypeDef USART2_Handler;  

//���ջ����� 	
u8 USART2_RX_BUF[MAX_LENGTH];  	
//���յ������ݳ���
int USART2_RX_CNT=0;  
u8 FLAG_UART_BL180_ACK=0;
u8 FLAG_RECEIVE_ANSOWER_BL180=0;

u8 FLAG_UART_CHEMINERT=0;

void USART2_IRQHandler(void)
{
    u8 res;	  
    if((__HAL_UART_GET_FLAG(&USART2_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�
	{	
		HAL_UART_Receive(&USART2_Handler,&res,1,1);
		if(USART2_RX_CNT<MAX_LENGTH)
		{
			
			USART2_RX_BUF[USART2_RX_CNT]=res;		//��¼���յ���ֵ
			//printf("zzzz%d\r\n",res);
			USART2_RX_CNT++;						//������������1 
		}
		
#if USE_CLEANING_DILUTION_BOARD		
		//�������CHEMINERTʵ��
		if(USART2_RX_CNT>=3&&(USART2_RX_BUF[USART2_RX_CNT-2]==0x0d&&USART2_RX_BUF[USART2_RX_CNT-1]==0x0a))
		{
				FLAG_UART_CHEMINERT=1;
		}
#endif

#if USE_AUTOMATIC_INJECTION_BOARD		
		if(USART2_RX_CNT>=7 && USART2_RX_BUF[0]==0xff)
		{
				if(USART2_RX_CNT == USART2_RX_BUF[1]+7)FLAG_RECEIVE_RFID=1;
		}
		
		if((USART2_RX_BUF[1]=='K')&&(USART2_RX_BUF[0]=='O'))
		{
				FLAG_UART_BL180_ACK=1;
		}
		if(USART2_RX_CNT>5&&(USART2_RX_BUF[USART2_RX_CNT-1]==0x0a||USART2_RX_BUF[USART2_RX_CNT-1]==0x0d)){
					FLAG_RECEIVE_ANSOWER_BL180=1;
		}
		
		if(USART2_RX_CNT>4&&USART2_RX_BUF[USART2_RX_CNT-1]==0x0a&&USART2_RX_BUF[USART2_RX_CNT-2]==0x0d)
		{
				FLAG_UART_FM100=1;
		}
		if(USART2_RX_CNT==4&&(USART2_RX_BUF[USART2_RX_CNT-1]=='@')&&(USART2_RX_BUF[USART2_RX_CNT-2]=='@'))
		{
				FLAG_UART_FM100_INTO=1;
		}
		if(USART2_RX_CNT==4&&(USART2_RX_BUF[USART2_RX_CNT-1]=='^')&&(USART2_RX_BUF[USART2_RX_CNT-2]=='^'))
		{
				FLAG_UART_FM100_EXIT=1;
		}
		if(USART2_RX_CNT==10&&(USART2_RX_BUF[USART2_RX_CNT-1]==';'))
		{
				FLAG_UART_FM100_ACK=1;
		}
#endif		

#if USE_GRADIENT_CONTROL_BOARD				

		
		if(USART2_RX_CNT==1&&(USART2_RX_BUF[0]==0x23||USART2_RX_BUF[0]==0x24||USART2_RX_BUF[0]==0x25)){
					FLAG_RECEIVE_ACK_PUMP100=1;
		}
		
		if(USART2_RX_CNT>2&&(USART2_RX_BUF[USART2_RX_CNT-1]==0x0a||USART2_RX_BUF[USART2_RX_CNT-1]==0x0d)){
					FLAG_RECEIVE_ANSOWER_PUMP100=1;
		}	
#endif		
		
	} 
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



//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void UART2_Send_Data(u8 *buf,int len)
{
	//printf("uart2 send\r\n");
	HAL_UART_Transmit(&USART2_Handler,buf,len,100);//����2��������
	USART2_RX_CNT=0;
}
//buf:���ջ����׵�ַ
//len:���������ݳ���
void UART2_Receive_Data(u8 *buf,int *len)
{
	int rxlen=USART2_RX_CNT;
	int i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==USART2_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART2_RX_BUF[i];	
		}		
		*len=USART2_RX_CNT;	//��¼�������ݳ���
		USART2_RX_CNT=0;		//����
	}
} 
