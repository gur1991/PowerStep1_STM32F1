#include "spi1.h"
#include "config.h"

SPI_HandleTypeDef SPI1_Handler;  //SPI1���

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{
    SPI1_Handler.Instance=SPI1;                         //SPI2
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    //--------------------important
		SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    
		SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI1_Handler);//��ʼ��
    
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //ʹ��SPI1
}

void SPI1_AD_CS_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();           //ʹ��GPIOCʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;          	//C4 C5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����       
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //��ʼ��
	
	/***********************************************************/
	    __HAL_RCC_GPIOB_CLK_ENABLE();           //ʹ��GPIOBʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;          	//B0 B1 B2
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����       
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��
	
	/***********************************************************/
		__HAL_RCC_GPIOF_CLK_ENABLE();           //ʹ��GPIOFʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_11;          	//F11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����       
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);     //��ʼ��
}

#if 0
//SPI1�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	
    GPIO_InitTypeDef GPIO_Initure;
	  if(hspi->Instance == SPI1)
  {  
    __HAL_RCC_GPIOA_CLK_ENABLE();       //ʹ��GPIOAʱ��
    __HAL_RCC_SPI1_CLK_ENABLE();        //ʹ��SPI1ʱ��
   // __HAL_RCC_AFIO_CLK_ENABLE();
	/*
	  GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
    GPIO_Initure.Pull=GPIO_PULLUP;                  //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;             //����            
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	*/
		
		  /* Configure SPI SCK */
  GPIO_Initure.Pin = GPIO_PIN_5;
  GPIO_Initure.Mode = GPIO_MODE_AF_PP;
  GPIO_Initure.Pull  = GPIO_PULLUP;
  GPIO_Initure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_Initure);

  GPIO_Initure.Pin = GPIO_PIN_6;
  GPIO_Initure.Pull  = GPIO_PULLDOWN;
	GPIO_Initure.Mode=GPIO_MODE_INPUT;  
  HAL_GPIO_Init(GPIOA, &GPIO_Initure);
  
  GPIO_Initure.Pin = GPIO_PIN_7;
  GPIO_Initure.Pull  = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	}

}
#endif


//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ42Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI1_Handler);            //�ر�SPI
    SPI1_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI1_Handler);             //ʹ��SPI
    
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 100);       
 	return Rxdata;          		    //�����յ�������		
}

/*

static __inline void Set_AD_CS(AD_type cs,AD_LEVEL_type level)
{
		switch(cs){
			case AD1_CS:SPI1_AD1_CS=level;break;
			case AD2_CS:SPI1_AD2_CS=level;break;
			case AD3_CS:SPI1_AD3_CS=level;break;
			case AD4_CS:SPI1_AD4_CS=level;break;
			case AD5_CS:SPI1_AD5_CS=level;break;
			case AD6_CS:SPI1_AD6_CS=level;break;
		}
}	


void SPI1_Transfer(u8* txData,u8*rxData,int len,AD_type cs){
		int i;
		//SPI1_CS=0;
		Set_AD_CS(cs,CS_LOW);
		for(i=0;i<len;i++){
				rxData[i]=SPI1_ReadWriteByte(txData[i]);
		}
		//SPI1_CS=1;
		Set_AD_CS(cs,CS_HIGH);
}

u8 AD_Sensor_Init(void){
		SPI1_Init();		   			        //��ʼ��SPI
		SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_64); //����Ϊ42Mʱ��,����ģʽ
		
		SPI1_AD_CS_init();
		SPI1_AD1_CS=1;
		SPI1_AD2_CS=1;
		SPI1_AD3_CS=1;
		SPI1_AD4_CS=1;
		SPI1_AD5_CS=1;
		SPI1_AD6_CS=1;
		return 0;
}
int AD_Sensor_Get_Data(AD_type cs){
		u8 txbuf[3]={0x00,0x00,0x00};
		u8 rxbuf[3]={0,0,0};
		u8 bit_low=0;
		u8 bit_high=0;
		u16 value=0;
		
		SPI1_Transfer(txbuf,rxbuf,sizeof(txbuf),cs);
				
		bit_high|=rxbuf[0]<<6;//ȡ��2bit
		bit_high|=rxbuf[1]>>2;//ȡ��6bit
		bit_low|=rxbuf[1]<<6;
		bit_low|=rxbuf[2]>>2;
		
		value=bit_high;
		value<<=8;
		value|=bit_low;
		
		return value;
}
*/
