#include "factory_single.h"
#include "delay.h"
#include "delay.h"
void FactoryMotorTestMode_single(void)
{
	int i,result,value;
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
	
	
	FirstOpenMotorCheckPosition();

while(1){
	
	
#if 1		
	
	if(!Light_Sensor_Get(1)){
			printf("check to stop motor 1\r\n");
			PowerStep_Select_Motor_Baby(1);
			//delay_ms(300);
			BSP_MotorControl_HardStop(0);
			RestSelectMotorPosition(1,6,120,BACKWARD);
		
			printf("check to BACKWARD motor 2\r\n");
			PowerStep_Select_Motor_Baby(2);
			
			BSP_MotorControl_Move(0, BACKWARD, 15000);
	}

	if(!Light_Sensor_Get(2)){
			printf("check to stop motor 2\r\n");
			PowerStep_Select_Motor_Baby(2);
			delay_ms(2*1000);
			BSP_MotorControl_HardStop(0);
			
			delay_ms(1000);
			printf("check to FORWARD motor 4\r\n");
			PowerStep_Select_Motor_Baby(4);
			ChangeSpeedMotorRun(4 ,20000 ,120, FORWARD);
			BSP_MotorControl_Move(0, FORWARD, 4000);
	}
	
	if(!Light_Sensor_Get(4)){
			PowerStep_Select_Motor_Baby(4);
			delay_ms(300);
			BSP_MotorControl_HardStop(0);
		
			RestSelectMotorPosition(4,3,120,BACKWARD);
		
			printf("check to FORWARD motor 3\r\n");
			PowerStep_Select_Motor_Baby(3);
			BSP_MotorControl_Move(0, BACKWARD, 15000);
	}
	if(!Light_Sensor_Get(5)){
			printf("check to stop motor 3\r\n");
			PowerStep_Select_Motor_Baby(3);
			delay_ms(2*1000);
			BSP_MotorControl_HardStop(0);
		
			delay_ms(1000);
			printf("check to FORWARD motor 1\r\n");
			PowerStep_Select_Motor_Baby(1);
			ChangeSpeedMotorRun(1 ,20000 ,120, FORWARD);
			BSP_MotorControl_Move(0, FORWARD, 4000);
	}
	
	delay_ms(1);
#endif
	
	
	
		//����ֹͣ���е���˶�
	if(!Light_Sensor_Get(24)){
			printf("check to stop all motor motion\r\n");
			StopALLMotorMotion();
	}
		
#if 0
		for(i=1;i<=6;i++)
		{
				value=Light_Sensor_Get(i);
				printf("light[%d]:%d\r\n",i,value);
				delay_ms(100);
		}
		//Light_Sensor_Get_All();
		//printf("light --> 0x%X \r\n", gStatusLight[0]&0x40);
#endif
				
			delay_ms(300);
	}


}	

