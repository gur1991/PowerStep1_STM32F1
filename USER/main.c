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

int main(void)
{	
    u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE;	
	u8 TX_BUF[5]={1,2,3,4,5};
	u8 RX_BUF[5];
	u8 len=0;
	
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
	
	RS485_Init(115200);
	UART3_Init(115200);
	while(1){
		//test_protocl();
		test_run();
		
		
	//RS485_Send_Data(TX_BUF,5);
	//UART3_Send_Data(TX_BUF,5);
	//delay_ms(10);
	//UART3_Receive_Data(RX_BUF,&len);
		//RS485_Receive_Data(RX_BUF,&len);
	//while(1){
		//	printf("%d  %d \r\n",RX_BUF[i],len);
			//i++;
			//if(i==len)break;
//	}
	}
	while(1){
	
	}
	return 0;
}
