#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
#include "w25qxx.h"
#include "light.h"
#include "uart3.h"
#include "rs485.h"
#include "light.h"
#include "uart_command_control_protocol.h"
#include "master_uart_control_interface.h"
/************************************************
 ALIENTEK ��Ӣ��STM32F103������ ʵ��23
 SPIʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{	
	get_light_sensor_level_t data;
	uint8_t value;
	
  HAL_Init();                    	 	//��ʼ��HAL��    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	usmart_dev.init(84); 		   		//��ʼ��USMART	
	LED_Init();							//��ʼ��LED	
	KEY_Init();							//��ʼ������
	Light_Sensor_Init();

	RS485_Init(115200);//with rk3188 rs232 exchange info
	UART3_Init(115200);//with slave rs232 exchange info
	printf("into slave device \r\n");
	data.request.number=1;
	data.response.ret=1;
	data.response.value=3;
	master_get_light_sensor_level(data,&value);
	
	printf("ggg size %d",sizeof(Powerstep1_contorl_motor_command_t));
	while(1){
			if(FLAG_UART_RK3188){
					printf("receive all \r\n");
					protocol_handle_uart_powerstep01_plain_slave_cmd();
					FLAG_UART_RK3188=0;
			}
	
	
			delay_ms(10);
	}
	return 0;
}
