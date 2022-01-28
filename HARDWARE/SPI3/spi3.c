#include "spi3.h"
#include "config.h"
#include "stm32f1xx_hal_gpio_ex.h"

SPI_HandleTypeDef SPI3_Handler;  

void SPI3_Init(void)
{
    SPI3_Handler.Instance=SPI3;                         //SPI3
    SPI3_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI3_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI3_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI3_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI3_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI3_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI3_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI3_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI3_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI3_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI3_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI3_Handler);//��ʼ��
    
    __HAL_SPI_ENABLE(&SPI3_Handler);                    //ʹ��SPI3


}





#if 0
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
		if(hspi->Instance==SPI3)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			__HAL_RCC_SPI3_CLK_ENABLE();
			__HAL_RCC_AFIO_CLK_ENABLE();
			//__HAL_AFIO_REMAP_SWJ_NOJTAG();
			
			/**SPI3 GPIO Configuration    
			PB3     ------> SPI3_SCK
			PB4     ------> SPI3_MISO
			PB5     ------> SPI3_MOSI 
			*/
			GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

			GPIO_InitStruct.Pin = GPIO_PIN_4;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLDOWN;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		}	
}
#endif


void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI3_Handler);            //�ر�SPI
    SPI3_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI3_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI3_Handler);             //ʹ��SPI
    
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI3_Handler,&TxData,&Rxdata,1, 10);   

		//printf("Rxdata��%d\r\n",Rxdata);
 	return Rxdata;          		   
}
