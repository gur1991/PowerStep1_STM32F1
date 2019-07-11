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
	
	HAL_Init();                    	 	//��ʼ��HAL��    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	usmart_dev.init(84); 		   		//��ʼ��USMART	
//	KEY_Init();							//��ʼ������
//	Light_Sensor_Init();
//	TIM3_PWM_Init(500-1,72-1);       	//72M/72=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ500����ôPWMƵ��Ϊ1M/500=2kHZ
//	AD_SENSOR_init();
	
//	RS485_Init(115200);//with rk3188 rs232 exchange info
//	UART3_Init(115200);//with slave rs232 exchange info
	printf("into slave device \r\n");

	
//	TIM_SetTIM3Compare4(500);	//�޸ıȽ�ֵ���޸�ռ�ձ�


  BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, 1);
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);
	
	
	//������ʼ������powerStep01
	for(i=1;i<15;i++){
			PowerStep_Select_Motor_Baby(i);
			Powerstep01_Init_Register(NULL);
	}
	//���һ��ͳһ��������Դ��������ʼ��
	//֮�����ֱ�ӵ���
	//����֮ǰ����PowerStep_Select_Motor_Baby(int chip);��ѡ��������
	
	
  //BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);
  //BSP_MotorControl_AttachBusyInterrupt(MyBusyInterruptHandler);
  //BSP_MotorControl_AttachErrorHandler(MyErrorHandler);
 	  PowerStep_Select_Motor_Baby(14);

	while(1){
#if 1		
		BSP_MotorControl_Move(0, FORWARD, 6000);
		BSP_MotorControl_WaitWhileActive(0);
		BSP_MotorControl_Move(0, BACKWARD, 6000);
		BSP_MotorControl_WaitWhileActive(0);
#endif
#if 0	
		PowerStep_Select_Motor_Baby(4);
		BSP_MotorControl_Move(0, FORWARD, 6000);
		PowerStep_Select_Motor_Baby(1);
		BSP_MotorControl_Move(0, FORWARD, 6000);

		BSP_MotorControl_WaitWhileActive(0);
		
		PowerStep_Select_Motor_Baby(4);
		BSP_MotorControl_Move(0, BACKWARD, 6000);
		PowerStep_Select_Motor_Baby(1);
		BSP_MotorControl_Move(0, BACKWARD, 6000);
		
		BSP_MotorControl_WaitWhileActive(0);
		
		PowerStep_Select_Motor_Baby(4);
		BSP_MotorControl_CmdResetPos(0);
		
		PowerStep_Select_Motor_Baby(1);
		BSP_MotorControl_CmdResetPos(0);
		#endif
		
#if 0
		printf(" rst start\r\n");
		Powerstep01_Board_ReleaseReset(0);
		delay_ms(10);
		Powerstep01_Board_Reset(0);
		delay_ms(10);
#endif



#if 0
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
#endif				
			delay_ms(10);
	}
	return 0;
}
