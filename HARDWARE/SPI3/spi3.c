#include "spi3.h"
#include "config.h"
#include "stm32f1xx_hal_gpio_ex.h"

SPI_HandleTypeDef SPI3_Handler;  

void SPI3_Init(void)
{
    SPI3_Handler.Instance=SPI3;                         //SPI3
    SPI3_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI3_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI3_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI3_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI3_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI3_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI3_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//定义波特率预分频的值:波特率预分频值为256
    SPI3_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI3_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI3_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI3_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI3_Handler);//初始化
    
    __HAL_SPI_ENABLE(&SPI3_Handler);                    //使能SPI3


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
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI3_Handler);            //关闭SPI
    SPI3_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI3_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI3_Handler);             //使能SPI
    
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI3_Handler,&TxData,&Rxdata,1, 10);   

		//printf("Rxdata：%d\r\n",Rxdata);
 	return Rxdata;          		   
}
