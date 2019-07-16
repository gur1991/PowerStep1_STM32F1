
#include "factory_many.h"



//���½ǵ�һλΪblank position λ
//��һ��һ��Ҫȷ����Ϊû���Թ�
//ת����֤
//�� 1   �� 0
int ClearAndCheckBlankPosition(void)
{
		int i;
		printf("clear blank position \r\n");
		PowerStep_Select_Motor_Baby(3);
		BSP_MotorControl_Move(0, BACKWARD, 5000);
		BSP_MotorControl_WaitWhileActive(0);	
	
		return Light_Sensor_Get(4);
}


//��ǵ�һλΪwait position λ
//һ��Ҫȷ����Ϊû���Թ�
//ת����֤
//�� 1   �� 0
int ClearAndCheckWaitPosition(void)
{
		int i;
		printf("clear wait position \r\n");
		PowerStep_Select_Motor_Baby(2);	
		BSP_MotorControl_Move(0, BACKWARD, 5000);
		BSP_MotorControl_WaitWhileActive(0);
	
		return Light_Sensor_Get(1);
}






//���½ǵ�һλΪready position λ
//��һ��һ��Ҫȷ����λ���Թ�
//ת����֤
//�� 1   �� 0
int ReadyAndCheckNextPosition(void)
{
		int i=0;
		printf("ready blank position \r\n");
		PowerStep_Select_Motor_Baby(2);
		BSP_MotorControl_Move(0, BACKWARD, 15000);
							
		while(1)
		{
				  i++;
					if(!Light_Sensor_Get(2))
					{
								BSP_MotorControl_HardStop(0);
							//��м�⵽�󣬶���һ�ᣬ���Թܰ���
								BSP_MotorControl_Move(0, BACKWARD, 5000);
								BSP_MotorControl_WaitWhileActive(0);
								BSP_MotorControl_HardStop(0);
								break;
					}else if(i>=1000)
					{
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(10);
		}
		return Light_Sensor_Get(2);
}


//���Ͻǵ�һλΪleft position λ
//�������λ�ò���һ�����Թ�
//ת����֤
//�� 1   �� 0
int ReadyAndCheckLeftPosition(void)
{
		int i;
		printf("ready left position \r\n");
		PowerStep_Select_Motor_Baby(3);
		BSP_MotorControl_Move(0, BACKWARD, 15000);
		while(1)
		{
				  i++;
					if(!Light_Sensor_Get(12))
					{			
								BSP_MotorControl_HardStop(0);
								//��м�⵽�󣬶���һ�ᣬ���Թܰ���
								BSP_MotorControl_Move(0, BACKWARD, 5000);
								BSP_MotorControl_WaitWhileActive(0);
								BSP_MotorControl_HardStop(0);
								break;
					}else if(i>=1000)
					{
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(10);
		}
	
		return Light_Sensor_Get(12);
}


//next -> blank
//ȷ�����Թܴ�next��blank
//ת����֤-->�����λ
//�� 1   �� 0
int NextMoveTowardBlankPosition(void)
{
			int i=0,times=2500;
	
			printf("next -> blank\r\n");
			PowerStep_Select_Motor_Baby(4);
			BSP_MotorControl_Move(0, FORWARD, 20000);

			//ChangeSpeedMotorRun(4 ,20000 ,120, FORWARD);
			//ChangeSpeedMotorRun(4 ,10000 ,120, FORWARD);
			
			//ChangeSpeedMotorRun(4 ,10000 ,120, FORWARD);
			
			//BSP_MotorControl_Move(0, FORWARD, 4000);
			while(1){
					i++;
					if(Light_Sensor_Get(23))
					{
							PowerStep_Select_Motor_Baby(4);
							BSP_MotorControl_HardStop(0);
							CollectSampleTask();
							
							PowerStep_Select_Motor_Baby(4);
							BSP_MotorControl_Move(0, FORWARD, 600);
							BSP_MotorControl_WaitWhileActive(0);//�߹�һ���Թܵĳ��ȣ������ظ����һ��
							BSP_MotorControl_Move(0, FORWARD, 20000);//�ټ���ǰ��
							times-=80;
					}
					if(!Light_Sensor_Get(4)){
							BSP_MotorControl_HardStop(0);
							break;
					}else if(i>=times){
							//�˴���ӵ�Ŀ���ǣ���ֹ������ÿ��⵽�Թܣ�ҲҪֹͣ
							//ֹͣ����ķ�����Ԥ����һ��25s��ÿ��⵽һ���Թܼ���0.8s��������10s
							BSP_MotorControl_HardStop(0);
							break;	
					}
					delay_ms(10);
		 }
		RestSelectMotorPosition(4,3,120,BACKWARD);	
		return Light_Sensor_Get(4);
}	



//���Ͻ� wait  ���Ͻ� left
//left ->wait
//���left���Թܾ�ȷ��ת��wait
//ת����֤-->�����λ
//�� 1   �� 0
int LeftMoveTowardWaitPosition(void)
{
		int i;
		printf("left ->wait\r\n");
		PowerStep_Select_Motor_Baby(1);
		ChangeSpeedMotorRun(1 ,20000 ,120, FORWARD);
		BSP_MotorControl_Move(0, FORWARD, 4000);
		while(1){
				i++;
				if(!Light_Sensor_Get(1)){
						BSP_MotorControl_HardStop(0);
						break;
				}else if(i>=200){
						BSP_MotorControl_HardStop(0);
						break;
				}
				delay_ms(10);
		}
		RestSelectMotorPosition(1,6,120,BACKWARD);
		return Light_Sensor_Get(1);
}	
/****************************sample collect start************************************/
void RestCollectHorizontalPosition(void)
{
		PowerStep_Select_Motor_Baby(5);
		BSP_MotorControl_Move(0, FORWARD, 7000);
		while(1)
		{
					if(!Light_Sensor_Get(7))
					{
								//printf("stop H\r\n");
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(1);
		}
}
void RestCollectVerticalPosition(void)
{
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, BACKWARD, 400000);
		while(1)
		{
					if(!Light_Sensor_Get(8))
					{
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(1);
		}
		
}
void RestCollectAllPosition(void)
{
		RestCollectVerticalPosition();
		RestCollectHorizontalPosition();
}	



void  CollectSampleTask(void)
{
	  RestCollectAllPosition();
		
	  //ˮƽ����5500
		PowerStep_Select_Motor_Baby(5);
		BSP_MotorControl_Move(0, BACKWARD, 5500);
		BSP_MotorControl_WaitWhileActive(0);
		
		//��ֱ����170000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, FORWARD, 170000);
		BSP_MotorControl_WaitWhileActive(0);
		InjectLiquidTask();
#if 1	
		//��ֱ����70000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, BACKWARD, 70000);
		BSP_MotorControl_WaitWhileActive(0);
	
		//ˮƽ����1000
		PowerStep_Select_Motor_Baby(5);
		BSP_MotorControl_Move(0, FORWARD, 1000);
		BSP_MotorControl_WaitWhileActive(0);
		
	  //��ֱ����70000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, FORWARD, 70000);
		BSP_MotorControl_WaitWhileActive(0);
		RestInjectAllPosition();
		
		//��ֱ����35000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, BACKWARD, 35000);
		BSP_MotorControl_WaitWhileActive(0);
		
		//ˮƽ����500
		PowerStep_Select_Motor_Baby(5);
		BSP_MotorControl_Move(0, FORWARD, 500);
		BSP_MotorControl_WaitWhileActive(0);
		
		//��ֱ����70000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, FORWARD, 50000);
		BSP_MotorControl_WaitWhileActive(0);
		
		//��ֱ����100000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, BACKWARD, 10000);
		BSP_MotorControl_WaitWhileActive(0);
		
#endif
		RestCollectAllPosition();
}
/****************************sample collect end************************************/



/****************************sample  inject start************************************/

void RestInjectBigPosition(void)
{
		PowerStep_Select_Motor_Baby(7);
		BSP_MotorControl_Move(0, BACKWARD, 200000);
		while(1)
		{
					if(!Light_Sensor_Get(9))
					{
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(1);
		}
}
void RestInjectLittlePosition(void)
{
		PowerStep_Select_Motor_Baby(8);
		BSP_MotorControl_Move(0, BACKWARD, 200000);
		while(1)
		{
					if(!Light_Sensor_Get(10))
					{
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(1);
		}
}

void RestInjectAllPosition(void)
{
		RestInjectBigPosition();
		RestInjectLittlePosition();
}	


void InjectLittleWork(uint32_t steps)
{
		PowerStep_Select_Motor_Baby(8);
		BSP_MotorControl_Move(0, FORWARD, steps);
		BSP_MotorControl_WaitWhileActive(0);
}


void InjectBigWork(uint32_t steps)
{
		PowerStep_Select_Motor_Baby(7);
		BSP_MotorControl_Move(0, FORWARD, steps);
		BSP_MotorControl_WaitWhileActive(0);
}

void  InjectLiquidTask(void)
{
		InjectBigWork(30000);
		InjectLittleWork(30000);
}	

/****************************sample inject end************************************/




void FactoryMotorTestMode_many(void)
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
	
	
	
	
//			InjectLiquidTask();
		//CollectSampleTask();
//	return ;
	//�����Լ�
	FirstOpenMotorCheckPosition();
	RestInjectAllPosition();

	
	while(1){
#if 0			
		//�ȴ��Թܼܷ������
			//ˢ���ź� �Ϳ�������
			while(!Light_Sensor_Get(24)){
			//StopALLMotorMotion();
				delay_ms(10);
			}
#endif			
			while(1){
				//step 1�����blank position
					if(!ClearAndCheckBlankPosition()){
						printf("clear blank position fail \r\n");
						break;
					}else{
							printf("clear blank position successful \r\n");
							//step 2��׼��next position
							if(ReadyAndCheckNextPosition()){
									printf("ready next position fail \r\n");
									//break;
							}else{
									printf("ready next position successful \r\n");
								
									//step 3  �ƶ�next ->blank position ������Զ���λ
									if(NextMoveTowardBlankPosition()){
												printf("next->balnk position fail \r\n");
												break;
									}else{
												printf("next->balnk position successful \r\n");
									}
								}	
								//step 4�����wait position
								if(!ClearAndCheckWaitPosition()){
												printf("clear wait position fail \r\n");
												break;
								}else{
												printf("clear wait position successful \r\n");
												//step 5��׼��left position
												if(ReadyAndCheckLeftPosition()){
															printf("ready left position fail \r\n");
															break;
												}else{
															printf("ready left position successful \r\n");
															//step 6���ƶ� left->wait ������Զ���λ
															if(LeftMoveTowardWaitPosition()){
																		printf("left -> move position fail \r\n");
															}else{
																		printf("left -> move position successful \r\n");
															}
															printf("circle over \r\n");
															break;
												}
								}
									
							
				}
			}
		}
	delay_ms(500);	
}