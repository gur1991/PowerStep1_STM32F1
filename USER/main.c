#include "key.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usmart.h"
#include "light.h"
#include "uart3.h"
#include "uart4.h"
#include "uart2.h"
#include "light.h"
#include "weight.h"
#include "slave_uart_control_interface.h"
#include "printpaper.h"
#include "baby18dad400.h"
#include "factory_single.h"
#include "factory_many.h"
#include "keep_temperature.h"
#include "temperature.h"
#include "uart_choose.h"
#include "config.h"
#include "electromagnetic.h"
#include "wdg.h"
#include "step01.h"
#include "pump_s100.h"
#include "fm100.h"
#include "S1125.h"

int main(void)
{	
	HAL_Init();                    	 	
  Stm32_Clock_Init(RCC_PLL_MUL9);   	
	delay_init(72);               		
	uart_init(115200);					
	//usmart_dev.init(84); 		   	
	UART4_Init(115200);
	//IWDG_Init(4,625*6); //6s   	MAX
  //IWDG_Start();
	int i;
			
#if USE_SENSOR_BOARD	
	TIM3_PWM_Init(500-1,72-1);
	Pid_init();
	TIM_SetTIM3Compare4(500);	
	ThermometerChooseHandle(DS18B20);
	ThermometerHandle->init();
	ThermometerHandle->set_degree(0,TMEPERATURE_CURRENT);
	
	//TIM5_Init(COUNT_TIME, 7999);//10Khz Ƶ�� 5000����  500ms 
	
	UART2_Init(9600);
	UART3_Init(9600);
  Uart_cs_init();
	AD_Sensor_Init();//�ĸ�������������ʼ��
	//S1125_Init();//�¹���
	
	Electromagnetic_init();//��ŷ�
	Init_Scan_FM100(true);
	printf("sensor board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

#if USE_MOTOR_BOARD
	Light_Sensor_Init();
	BSP_Motor_Control_Init();
	
	printf("motor board,protocol size:%d\r\n",sizeof(Powerstep1_contorl_motor_command_t));
#endif

//ThermometerHandle->set_degree(372,TMEPERATURE_CURRENT);
//	scan_test();
#if 0
printf("zz1\r\n");
Rest_Transporter_Belt();
printf("zz2\r\n");
LeftMoveTowardWaitPosition();
printf("zz3\r\n");
#endif

#if 0
	Rest_Transporter_Belt();
delay_ms(3000);
//RestSelectMotorOrgin(M1_BLANK_NEXT,BLANK_LIGHT,M1_NEXT_TO_BLANK, 23000);
Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK, 2000);
Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK, 1320);
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Pitch_Move_Next();
Normal_Blank_Rest();
Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK, 600);
RestSelectMotorOrgin(M1_BLANK_NEXT,M1_LIGHT,M1_BLANK_TO_NEXT, 30000);
Belt_Move_At_SameTime();
LeftMoveTowardWaitPosition();

Rest_Transporter_Belt();
Belt_Move_At_SameTime();

	Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK, 2000);
	delay_ms(3000);
	Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK, 1320);
	delay_ms(3000);
	for(i=0;i<10;i++){
		delay_ms(3000);
		Normal_Pitch_Move_Next();
	}
	RestSelectMotorOrgin(M1_BLANK_NEXT,BLANK_LIGHT,M1_NEXT_TO_BLANK, 10*1000);
	
	RestSelectMotorOrgin(M1_BLANK_NEXT,M1_LIGHT,M1_BLANK_TO_NEXT, 60*1000);
#endif	
	//Motor_Move_And_Wait(M9_IN_OUT, M9_IN, 10000);
	//delay_ms(1000);
	//RestSelectMotorOrgin(M9_IN_OUT,M9_LIGHT,M9_OUT, 60*1000);
	//RestSelectMotorOrgin(M10_BIG_IN_OUT,M10_LIGHT,M10_BIG_OUT, 60*1000);
while(1){
		
	{
		//printf("light[%d] %d\r\n",5,Light_Sensor_Get(5));
		//delay_ms(200);
	}


#if 1

		if(ARM_RS232_ASK)
		{
						printf("start receive !\r\n");
						//delay_ms(5);
#if USE_SENSOR_BOARD						
						KeepTemperatureDegree_Duty();
#endif			
						protocol_handle_uart_powerstep01_plain_slave_cmd();
						ARM_RS232_ASK=0;
		}	
		
		delay_ms(10);

		
		
#if USE_SENSOR_BOARD
				i++;
		if(i==100)
		{
			i=0;	
			
			keep_thermometer_degree();
		}		
#endif
		
#endif	
		//IWDG_Feed();
	}

	return 0;
}
