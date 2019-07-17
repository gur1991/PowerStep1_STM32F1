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
#include "uart4.h"
#include "rs485.h"
#include "light.h"
#include "uart_command_control_protocol.h"
#include "slave_uart_control_interface.h"
#include "printpaper.h"
#include "baby18dad400.h"
#include "factory_single.h"
#include "factory_many.h"


int main(void)
{	
	int result;
	get_light_sensor_level_t data;
	uint8_t value;
	int32_t pos;
  uint32_t readData;
  uint32_t ret;
	    u8 key;
	int len=0,i;
	u8 rx_buf[128];
	int flag_motor_begin=0;
	HAL_Init();                    	 	//��ʼ��HAL��    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	usmart_dev.init(84); 		   		//��ʼ��USMART	
	Light_Sensor_Init();
//	TIM3_PWM_Init(500-1,72-1);       	//72M/72=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ500����ôPWMƵ��Ϊ1M/500=2kHZ
//	AD_SENSOR_init();
	
//	RS485_Init(115200);//with rk3188 rs232 exchange info
//	UART3_Init(115200);//with slave rs232 exchange info
		UART4_Init(115200);
	printf("into slave device \r\n");


		
//	TIM_SetTIM3Compare4(500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	//FactoryMotorTestMode_single();
	//FactoryMotorTestMode_many();
	
	
//	u8 tx_buf4[]={0x23,0xaa,0x55};
//	UART4_Send_Data(tx_buf4,sizeof(tx_buf4));
	printf("send data %d \r\n",sizeof(Powerstep1_contorl_motor_command_t));
while(1){
		if(ARM_RS232_ASK){
						printf("start receive !\r\n");
						protocol_handle_uart_powerstep01_plain_slave_cmd();
						ARM_RS232_ASK=0;
		}	
		delay_ms(1);
	}

/*
			for(i=1;i<=1;i++)
		{
				value=Light_Sensor_Get(i);
				printf("light[%d]:%d\r\n",i,value);
				delay_ms(100);
		}

*/
	return 0;
}
