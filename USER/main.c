#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
#include "w25qxx.h"

#include "uart3.h"
#include "rs485.h"
#include "uart_command_control_protocol.h"
/************************************************
 ALIENTEK 精英板STM32F103开发板 实验23
 SPI实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//要写入到W25Q64的字符串数组
const u8 TEXT_Buffer[]={"WarShipSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)
extern UART_HandleTypeDef USART3_Handler; 
int main(void)
{	
    u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE;	
	u8 TX_BUF[5]={11,22,33,0x0d,0x0a};
	u8 RX_BUF[5];
	u8 len=0;
		move_type_t tx_move;
	tx_move.request.devices=2;
	tx_move.request.steps=6400;
	tx_move.request.types=POWERSTEP01_GO_TO;
	
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口
	usmart_dev.init(84); 		   		//初始化USMART	
//	LED_Init();							//初始化LED	
	//KEY_Init();							//初始化按键
 	//LCD_Init();           				//初始化LCD FSMC接口
	//printf("before motor \r\n");
	//motorInit();
	//printf("end motor \r\n");
#if 0
	RS485_Init(115200);
	UART3_Init(115200);
	
	RS485_Send_Data(TX_BUF,5);
	//delay_ms(1);
	//UART3_Receive_Data(RX_BUF,&len);
	//delay_ms(200);
	UART3_Send_Data(TX_BUF,5);
	//delay_ms(1);
	//RS485_Receive_Data(RX_BUF,&len);
	//delay_ms(200);
	//master_powerStep01_move_command(tx_move);
while(1){}

return 0;
#endif

#if 0
			RS485_Init(115200);
		printf("into master device \r\n");
		//RS485_Send_Data(TX_BUF,5);
		master_powerStep01_move_command(tx_move);
	while(1){
				//RS485_Send_Data(TX_BUF,5);
				//delay_ms(1000);
	}
#else
	UART3_Init(115200);
	printf("into slave device \r\n");
	while(1){
			if(FLAG_UART_SLAVE){
				printf("receive all \r\n");
				protocol_handle_uart_powerstep01_plain_slave_cmd();
				FLAG_UART_SLAVE=0;
			}
	}

#endif	
	return 0;
}
