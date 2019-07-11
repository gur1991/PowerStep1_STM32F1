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

init_motor_speed_tension_type_t TempMotor;


int ConfigMotorAllDevice(int chip)
{
	int ret=0;
	switch(chip)
	{
		case 1:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=400;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		
   		break;
		case 2:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=30;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		
   		break;
		case 3:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=400;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		
   		break;
		case 4:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=400;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		
   		break;
		case 5:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=400;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		
   		break;	
		case 13:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=100;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		
   		break;
		case 14:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=200;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=200;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=20;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		
   		break;
		default:
			ret=-1;
	}	
	return ret;
}



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
	
	HAL_Init();                    	 	//��ʼ��HAL��    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	usmart_dev.init(84); 		   		//��ʼ��USMART	
//	KEY_Init();							//��ʼ������
	Light_Sensor_Init();
//	TIM3_PWM_Init(500-1,72-1);       	//72M/72=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ500����ôPWMƵ��Ϊ1M/500=2kHZ
//	AD_SENSOR_init();
	
//	RS485_Init(115200);//with rk3188 rs232 exchange info
//	UART3_Init(115200);//with slave rs232 exchange info
	printf("into slave device \r\n");

	
//	TIM_SetTIM3Compare4(500);	//�޸ıȽ�ֵ���޸�ռ�ձ�


  BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, 1);
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);//�˴�NULLֻ����NULL�����贫����
	
	
		
	for(i=1;i<SIZE_MOTOR_ARRAY;i++){
			result=ConfigMotorAllDevice(i);//���õ������
			init_motor_device(TempMotor);//�ѵ������������������
			PowerStep_Select_Motor_Baby(i);
			
			if(!result){
				Powerstep01_Init_Register(&motor_config_array[i]);//��������Ҫ��ʼ����������λ����д
			}
			else
			{	
				Powerstep01_Init_Register(NULL);
			}
			printf("M[%d]-speed�� %f \r\n",i,motor_config_array[i].vm.cp.maxSpeed);
  }	
	
  //BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);
  //BSP_MotorControl_AttachBusyInterrupt(MyBusyInterruptHandler);
  //BSP_MotorControl_AttachErrorHandler(MyErrorHandler);
	
	PowerStep_Select_Motor_Baby(2);
	BSP_MotorControl_Move(0, BACKWARD, 40000);
	PowerStep_Select_Motor_Baby(13);
	BSP_MotorControl_Move(0, FORWARD, 600000);
	while(1){
#if 1		
	//BSP_MotorControl_WaitWhileActive(0);
	if(!Light_Sensor_Get(24)||!Light_Sensor_Get(1)){
			printf("check to stop\r\n");
			PowerStep_Select_Motor_Baby(2);
		  BSP_MotorControl_HardStop(0);
		  //BSP_MotorControl_CmdSoftStop(0);
	}
#endif
	
		
#if 0
		for(i=1;i<=24;i++)
		{
				value=Light_Sensor_Get(i);
				printf("light[%d]:%d\r\n",i,value);
				delay_ms(100);
		}
		Light_Sensor_Get_All();
		printf("light --> 0x%X \r\n", gStatusLight[0]&0x40);

#endif
		
#if 0		
		BSP_MotorControl_Move(0, FORWARD, 12000);
		BSP_MotorControl_WaitWhileActive(0);
		//BSP_MotorControl_CmdResetPos(0);
		BSP_MotorControl_Move(0, BACKWARD, 12000);
		BSP_MotorControl_WaitWhileActive(0);
		//BSP_MotorControl_CmdResetPos(0);
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
