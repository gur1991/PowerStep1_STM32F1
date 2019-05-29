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
#include "printpaper.h"
#include "baby18dad400.h"
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
	int32_t pos;
  uint32_t readData;
  uint32_t ret;
	    u8 key;
	int len=0;
	
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
	//master_get_light_sensor_level(data,&value);
	
	//printf("ggg size %d",sizeof(Powerstep1_contorl_motor_command_t));
	//Printer_Formart();

	while(1){
		/*
			if(FLAG_UART_RK3188){
					printf("receive all \r\n");
					protocol_handle_uart_powerstep01_plain_slave_cmd();
					FLAG_UART_RK3188=0;
			}
		*/
					key=KEY_Scan(0);
					if(key==KEY1_PRES)//KEY1按下,发送一次数据
				{
						printf("start key1 !\r\n");
			/*
						baby18_detector_name_read_protocol(NULL,0,35);
						baby18_hardware_version_read_protocol(NULL,0,35);
						baby18_model_read_protocol(NULL,0,35);
						baby18_firmware_version_read_protocol(NULL,0,35);
						baby18_serial_number_read_protocol(NULL,0,35);
						baby18_status_of_detector_read_protocol(NULL,0,35);
						baby18_status_of_detector_read_with_checksum_protocol(NULL,0,35);
						baby18_break_errors_write_with_checksum_protocol(NULL,0,35);
						baby18_wavelength_range_read_protocol(NULL,0,35);
						baby18_wavelength_on_channels_read_protocol(NULL,0,35);
						baby18_wavelength_on_channels_write_protocol("A254B380",8,35);
			*/
						Baby18dad_Execute_Interface(DETECTOR_NAME_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(HARDWARE_VERSION_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(MODEL_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(FIRMWARE_VERSION_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(SERIAL_NUMBER_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(STATUS_OF_DETECTOR_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(STATUS_OF_DETECTOR_READ_WITH_CHECKSUM,NULL,0,&len,30);
						Baby18dad_Execute_Interface(BREAK_ERROR_WRITE,NULL,0,&len,30);
						Baby18dad_Execute_Interface(WAVELENGTH_RANGE_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(WAVELENGTH_ON_CHANNELS_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(WAVELENGTH_ON_CHANNELS_WRITE,"A123B456",8,&len,30);
						Baby18dad_Execute_Interface(WAVELENGTH_SCAN_RANGE_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(WAVELENGTH_SCAN_RANGE_WRITE,"X200Y400",8,&len,30);
						Baby18dad_Execute_Interface(ELECTRONIC_BAND_WIDTH_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(ELECTRONIC_BAND_WIDTH_WRITE,"10",2,&len,30);
						Baby18dad_Execute_Interface(ELECTRONIC_HALF_WIDTH_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(ELECTRONIC_HALF_WIDTH_WRITE,"08",2,&len,30);
						Baby18dad_Execute_Interface(SUBSCRIPTION_FREQUENCY_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(SUBSCRIPTION_FREQUENCY_WRITE,"01",2,&len,30);
						Baby18dad_Execute_Interface(TIME_CONSTANT_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(TIME_CONSTANT_WRITE,"1",1,&len,30);
						Baby18dad_Execute_Interface(LAMP_USAGE_PARAMETERS_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(BOARD_DIP_SWITCHES_STATE_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(AUTOMATIC_IGNITION_STATE_READ,NULL,0,&len,30);
						
						
						
						
						printf("end key1 !\r\n");
				}

	
			delay_ms(10);
	}
	return 0;
}
