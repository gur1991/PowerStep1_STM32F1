#include "spi1.h"
#include "config.h"

SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{
    SPI1_Handler.Instance=SPI1;                         //SPI2
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    //--------------------important
		SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    
		SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//定义波特率预分频的值:波特率预分频值为256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI1_Handler);//初始化
    
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI1
}

void SPI1_AD_CS_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();           //使能GPIOC时钟
    
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;          	//C4 C5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速       
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //初始化
	
	/***********************************************************/
	    __HAL_RCC_GPIOB_CLK_ENABLE();           //使能GPIOB时钟
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;          	//B0 B1 B2
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速       
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化
	
	/***********************************************************/
		__HAL_RCC_GPIOF_CLK_ENABLE();           //使能GPIOF时钟
    
    GPIO_Initure.Pin=GPIO_PIN_11;          	//F11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速       
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);     //初始化
}

#if 0
//SPI1底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	
    GPIO_InitTypeDef GPIO_Initure;
	  if(hspi->Instance == SPI1)
  {  
    __HAL_RCC_GPIOA_CLK_ENABLE();       //使能GPIOA时钟
    __HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI1时钟
   // __HAL_RCC_AFIO_CLK_ENABLE();
	/*
	  GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;             //快速            
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


//SPI速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1时钟一般为42Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI1_Handler);            //关闭SPI
    SPI1_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI1_Handler);             //使能SPI
    
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 100);       
 	return Rxdata;          		    //返回收到的数据		
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
		SPI1_Init();		   			        //初始化SPI
		SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_64); //设置为42M时钟,高速模式
		
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
				
		bit_high|=rxbuf[0]<<6;//取低2bit
		bit_high|=rxbuf[1]>>2;//取高6bit
		bit_low|=rxbuf[1]<<6;
		bit_low|=rxbuf[2]>>2;
		
		value=bit_high;
		value<<=8;
		value|=bit_low;
		
		return value;
}
*/
