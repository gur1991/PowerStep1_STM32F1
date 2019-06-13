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
 ALIENTEK ¾«Ó¢°åSTM32F103¿ª·¢°å ÊµÑé23
 SPIÊµÑé-HAL¿âº¯Êý°æ
 ¼¼ÊõÖ§³Ö£ºwww.openedv.com
 ÌÔ±¦µêÆÌ£º http://eboard.taobao.com 
 ¹Ø×¢Î¢ÐÅ¹«ÖÚÆ½Ì¨Î¢ÐÅºÅ£º"ÕýµãÔ­×Ó"£¬Ãâ·Ñ»ñÈ¡STM32×ÊÁÏ¡£
 ¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾  
 ×÷Õß£ºÕýµãÔ­×Ó @ALIENTEK
************************************************/

int main(void)
{	
	get_light_sensor_level_t data;
	uint8_t value;
	int32_t pos;
  uint32_t readData;
  uint32_t ret;
	    u8 key;
	int len=0,i;
	u8 rx_buf[128];
	
	HAL_Init();                    	 	//³õÊ¼»¯HAL¿â    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//ÉèÖÃÊ±ÖÓ,72M
	delay_init(72);               		//³õÊ¼»¯ÑÓÊ±º¯Êý
	uart_init(115200);					//³õÊ¼»¯´®¿Ú
	usmart_dev.init(84); 		   		//³õÊ¼»¯USMART	
	LED_Init();							//³õÊ¼»¯LED	
	KEY_Init();							//³õÊ¼»¯°´¼ü
	Light_Sensor_Init();
	TIM3_PWM_Init(500-1,72-1);       	//72M/72=1MµÄ¼ÆÊýÆµÂÊ£¬×Ô¶¯ÖØ×°ÔØÎª500£¬ÄÇÃ´PWMÆµÂÊÎª1M/500=2kHZ
	AD_SENSOR_init();
	
	RS485_Init(115200);//with rk3188 rs232 exchange info
	UART3_Init(115200);//with slave rs232 exchange info
	printf("into slave device \r\n");
	data.request.number=1;
	data.response.ret=1;
	data.response.value=3;
	//master_get_light_sensor_level(data,&value);
	
	//printf("ggg size %d",sizeof(Powerstep1_contorl_motor_command_t));
	//Printer_Formart();
	
	TIM_SetTIM3Compare4(500);	//ÐÞ¸Ä±È½ÏÖµ£¬ÐÞ¸ÄÕ¼¿Õ±È
	while(1){
		/*
			if(FLAG_UART_RK3188){
					printf("receive all \r\n");
					protocol_handle_uart_powerstep01_plain_slave_cmd();
					FLAG_UART_RK3188=0;
			}
		*/
		
		
		
		
					key=KEY_Scan(0);
					if(key==KEY1_PRES)//KEY1°´ÏÂ,·¢ËÍÒ»´ÎÊý¾Ý
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
					
//group A - identification 
#if 0					
						Baby18dad_Execute_Interface(DETECTOR_NAME_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(HARDWARE_VERSION_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(MODEL_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(FIRMWARE_VERSION_READ,NULL,0,&len,30);
						Baby18dad_Execute_Interface(SERIAL_NUMBER_READ,NULL,0,&len,30);
#endif 						
//group B – states and parameters 
#if 0
						Baby18dad_Execute_Interface(STATUS_OF_DETECTOR_READ,NULL,0,&len,30);
						printf("ggg6\r\n");
						Baby18dad_Execute_Interface(STATUS_OF_DETECTOR_READ_WITH_CHECKSUM,NULL,0,&len,30);
						printf("ggg7\r\n");
						Baby18dad_Execute_Interface(BREAK_ERROR_WRITE,NULL,0,&len,30);
						printf("ggg8\r\n");
						Baby18dad_Execute_Interface(WAVELENGTH_RANGE_READ,NULL,0,&len,30);
						printf("ggg9\r\n");
						Baby18dad_Execute_Interface(WAVELENGTH_ON_CHANNELS_READ,NULL,0,&len,30);
						printf("ggg10\r\n");
						Baby18dad_Execute_Interface(WAVELENGTH_ON_CHANNELS_WRITE,"A254B380",8,&len,30);
						printf("ggg11\r\n");
						Baby18dad_Execute_Interface(WAVELENGTH_SCAN_RANGE_READ,NULL,0,&len,30);
						printf("ggg11\r\n");
						Baby18dad_Execute_Interface(WAVELENGTH_SCAN_RANGE_WRITE,"X200Y400",8,&len,30);
						printf("ggg12\r\n");
						Baby18dad_Execute_Interface(ELECTRONIC_BAND_WIDTH_READ,NULL,0,&len,30);
						printf("ggg13\r\n");
						Baby18dad_Execute_Interface(ELECTRONIC_BAND_WIDTH_WRITE,"10",2,&len,30);
						printf("ggg14\r\n");
						Baby18dad_Execute_Interface(ELECTRONIC_HALF_WIDTH_READ,NULL,0,&len,30);
						printf("ggg15\r\n");
						Baby18dad_Execute_Interface(ELECTRONIC_HALF_WIDTH_WRITE,"08",2,&len,30);
						printf("ggg16\r\n");
						Baby18dad_Execute_Interface(SUBSCRIPTION_FREQUENCY_READ,NULL,0,&len,30);
						printf("ggg17\r\n");
						Baby18dad_Execute_Interface(SUBSCRIPTION_FREQUENCY_WRITE,"01",2,&len,30);
						printf("ggg18\r\n");
						Baby18dad_Execute_Interface(TIME_CONSTANT_READ,NULL,0,&len,30);
						printf("ggg19\r\n");
						Baby18dad_Execute_Interface(TIME_CONSTANT_WRITE,"1",1,&len,30);
						printf("ggg20\r\n");
						Baby18dad_Execute_Interface(LAMP_USAGE_PARAMETERS_READ,NULL,0,&len,30);
						printf("ggg21\r\n");
						Baby18dad_Execute_Interface(BOARD_DIP_SWITCHES_STATE_READ,NULL,0,&len,30);
						printf("ggg22\r\n");
						Baby18dad_Execute_Interface(AUTOMATIC_IGNITION_STATE_READ,NULL,0,&len,80);
						printf("ggg23\r\n");
						Baby18dad_Execute_Interface(AUTOMATIC_IGNITION_STATE_WRITE,"1",1,&len,80);
						printf("ggg24\r\n");
#endif

//group C – executions commands and queries						
#if 0						
						//Baby18dad_Execute_Interface(LAMP_STATE_READ,NULL,0,&len,50);
						//printf("ggg25\r\n");
						//Baby18dad_Execute_Interface(LAMP_STATE_WRITE,"T",1,&len,500);//err4
						//printf("ggg26\r\n");
						//Baby18dad_Execute_Interface(GENERATE_SOUND_WRITE,"2",1,&len,50);
						//printf("ggg27\r\n");
						//Baby18dad_Execute_Interface(AUTOZERO_READ,NULL,0,&len,50);
						//printf("ggg28\r\n");
						//Baby18dad_Execute_Interface(AUTOZERO_WRITE,NULL,0,&len,50);
						//printf("ggg29\r\n");
						//Baby18dad_Execute_Interface(ABSORBANCE_READ,NULL,0,&len,50);
						//printf("ggg30\r\n");
						//Baby18dad_Execute_Interface(ABSORBANCE_READ_WITH_CHECKSUM,NULL,0,&len,50);
						//printf("ggg31\r\n");
						//Baby18dad_Execute_Interface(INTENSITY_READ,NULL,0,&len,50);
						//printf("ggg32\r\n");
						//Baby18dad_Execute_Interface(INTENSITY_READ_WITH_CHECKSUM,NULL,0,&len,50);						
						//printf("ggg33\r\n");
						//Baby18dad_Execute_Interface(ABSORBRANCE_SUBSCRIBE_START,NULL,0,&len,100);	
						//printf("ggg34\r\n");
						//Baby18dad_Execute_Interface(ABSORBRANCE_SUBSCRIBE_START_WITH_CHECKSUM,NULL,0,&len,100);	
						//printf("ggg35\r\n");
						//Baby18dad_Execute_Interface(INTENSITY_SUBCRIBE_START,NULL,0,&len,100);//------	
						//printf("ggg36\r\n");
						//Baby18dad_Execute_Interface(INTENSITY_SUBCRIBE_START_WITH_CHECKSUM,NULL,0,&len,100);//----	
						//printf("ggg37\r\n");
						//Baby18dad_Execute_Interface(SCAN_ABSORBANCE_READ,NULL,0,&len,100);	//----fail
						//printf("ggg38\r\n");
						//Baby18dad_Execute_Interface(SCAN_INTENSITY_READ,NULL,0,&len,200);	//----ka zhu
						//printf("ggg39\r\n");
						//Baby18dad_Execute_Interface(ABSORBRANCE_SUBSCRIBE_STOP,NULL,0,&len,100);	
						//printf("ggg40\r\n");
						//Baby18dad_Execute_Interface(ABSORBRANCE_SUBSCRIBE_STOP_C,NULL,0,&len,100);	
						//printf("ggg41\r\n");
						//Baby18dad_Execute_Interface(INTENSITY_SUBCRIBE_STOP,NULL,0,&len,100);	
						//printf("ggg42\r\n");
						//Baby18dad_Execute_Interface(INTENSITY_SUBCRIBE_STOP_C,NULL,0,&len,100);	
						//printf("ggg43\r\n");

#endif
//group D – user calibration mode commands and queries 
#if 0					
						Baby18dad_Execute_Interface(USER_CALIB_MODE_READ,NULL,0,&len,100);	
						printf("ggg44\r\n");
						Baby18dad_Execute_Interface(USER_CALIB_MODE_WRITE,"T",1,&len,100);	
						printf("ggg45\r\n");
						Baby18dad_Execute_Interface(USER_CALIB_PARAMETERS_READ,"T",1,&len,100);	
						printf("ggg46\r\n");
						Baby18dad_Execute_Interface(USER_CALIB_PARAMETERS_WRITE,"X254N880",8,&len,100);	
						printf("ggg47\r\n");
						Baby18dad_Execute_Interface(USER_CALIB_AUTOZERO_READ,NULL,0,&len,100);	
						printf("ggg48\r\n");
						Baby18dad_Execute_Interface(USER_CALIB_AUTOZERO_WRITE,NULL,0,&len,100);	
						printf("ggg49\r\n");
						Baby18dad_Execute_Interface(CALIB_100_LIGHT_INTENSITY_RUN,NULL,0,&len,100);	
						printf("ggg50\r\n");
						Baby18dad_Execute_Interface(CALIB_PARAMETRIC_ABSORBANCE_RUN,NULL,0,&len,100);
						printf("ggg51\r\n");
						Baby18dad_Execute_Interface(CALIB_STANDARD_ABSORBANCE_WRITE,NULL,0,&len,100);
						printf("ggg52\r\n");
						Baby18dad_Execute_Interface(STORE_INTENS_CALIB_VALUE_TO_MEMORY,NULL,0,&len,100);
						printf("ggg53\r\n");
						Baby18dad_Execute_Interface(STORE_ABS_CALIB_VALUE_TO_MEMORY,NULL,0,&len,100);						
						printf("ggg54\r\n");
						Baby18dad_Execute_Interface(NEW_LAMP_WRITE,NULL,0,&len,100);	
						printf("ggg55\r\n");	
						//Baby18dad_Execute_Interface(UNIT_REPORT_READ,NULL,0,&len,1000);							
#endif	
						printf("end key1 !\r\n");
				}
				
				
				if(key==KEY0_PRES)//KEY1°´ÏÂ,·¢ËÍÒ»´ÎÊý¾Ý
				{
						printf("start key0 !\r\n");
						//Baby18dad_Execute_Interface(LAMP_STATE_READ,NULL,0,&len,100);
						//Baby18dad_Execute_Interface(ABSORBANCE_READ,NULL,0,&len,100);
						//Baby18dad_Execute_Interface(INTENSITY_READ,NULL,0,&len,100);
						//Baby18dad_Execute_Interface(ABSORBRANCE_SUBSCRIBE_START,NULL,0,&len,100);
						//Baby18dad_Execute_Interface(INTENSITY_READ,NULL,0,&len,100);
						//Baby18dad_Execute_Interface(SCAN_ABSORBANCE_READ,NULL,0,&len,1000);
						//Baby18dad_Execute_Interface(SCAN_INTENSITY_READ,NULL,0,&len,1000);
						//Baby18dad_Execute_Interface(AUTOMATIC_IGNITION_STATE_READ,NULL,0,&len,80);
						//Baby18dad_Execute_Interface(TIME_CONSTANT_READ,NULL,0,&len,100);
						//Baby18dad_Execute_Interface(ELECTRONIC_HALF_WIDTH_READ,NULL,0,&len,100);
					Baby18dad_Execute_Interface(DETECTOR_NAME_READ,NULL,0,&len,100);
				}

				if(FLAG_UART_MASTER){
						printf("start receive !\r\n");
						delay_ms(10);
						RS485_Receive_Data(rx_buf,&len);
						printf("len:%d \r\n",len);
						for(i=0;i<len;i++)
						{
							printf("%c",rx_buf[i]);
						}
						printf("\r\n");
						FLAG_UART_MASTER=0;
				}
				
			delay_ms(10);
	}
	return 0;
}
