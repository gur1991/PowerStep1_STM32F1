
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
								delay_ms(2*1000);
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
					if(!Light_Sensor_Get(24))
					{
								delay_ms(2*1000);
								BSP_MotorControl_HardStop(0);
								break;
					}else if(i>=1000)
					{
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(10);
		}
	
		return Light_Sensor_Get(24);
}


//next -> blank
//ȷ�����Թܴ�next��blank
//ת����֤-->�����λ
//�� 1   �� 0
int NextMoveTowardBlankPosition(void)
{
			int i;
			printf("next to blank\r\n");
			PowerStep_Select_Motor_Baby(4);
			ChangeSpeedMotorRun(4 ,20000 ,120, FORWARD);
			BSP_MotorControl_Move(0, FORWARD, 4000);
			while(1){
					if(!Light_Sensor_Get(4)){
							delay_ms(300);
							BSP_MotorControl_HardStop(0);
							break;
					}else if(i>=200){
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
						printf("check to stop motor 1\r\n");
						//delay_ms(300);
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
	
	//�����Լ�
	FirstOpenMotorCheckPosition();


	
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
									break;
							}else{
									printf("ready next position successful \r\n");
									//step 3  �ƶ�next ->blank position ������Զ���λ
									if(NextMoveTowardBlankPosition()){
												printf("next->balnk position fail \r\n");
												break;
									}else{
												printf("next->balnk position successful \r\n");
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
			}
		}
	delay_ms(500);	
}






















void FactoryMotorTestMode_many2(void)
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
	
	//�����Լ�
	FirstOpenMotorCheckPosition();


	
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
									break;
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