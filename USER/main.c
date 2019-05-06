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
 ALIENTEK ��Ӣ��STM32F103������ ʵ��23
 SPIʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//Ҫд�뵽W25Q64���ַ�������
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
	
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	usmart_dev.init(84); 		   		//��ʼ��USMART	
//	LED_Init();							//��ʼ��LED	
	//KEY_Init();							//��ʼ������
 	//LCD_Init();           				//��ʼ��LCD FSMC�ӿ�
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
