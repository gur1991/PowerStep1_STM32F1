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
 ALIENTEK 精英板STM32F103开发板 实验23
 SPI实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{	
	get_light_sensor_level_t data;
	uint8_t value;
	
  HAL_Init();                    	 	//初始化HAL库    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口
	usmart_dev.init(84); 		   		//初始化USMART	
	LED_Init();							//初始化LED	
	KEY_Init();							//初始化按键
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
