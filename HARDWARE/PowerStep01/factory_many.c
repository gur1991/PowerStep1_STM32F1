
#include "factory_many.h"

//右下角第一位为blank position 位
//第一步一定要确保此为没有试管
//转动后保证
//无 1   有 0
int ClearAndCheckBlankPosition(void)
{
		int i;
		printf("clear blank position \r\n");
		PowerStep_Select_Motor_Baby(3);
		BSP_MotorControl_Move(0, BACKWARD, 5000);
		BSP_MotorControl_WaitWhileActive(0);	
	
		return Light_Sensor_Get(4);
}


//左角第一位为wait position 位
//一定要确保此为没有试管
//转动后保证
//无 1   有 0
int ClearAndCheckWaitPosition(void)
{
		int i;
		printf("clear wait position \r\n");
		PowerStep_Select_Motor_Baby(2);	
		BSP_MotorControl_Move(0, BACKWARD, 5000);
		BSP_MotorControl_WaitWhileActive(0);
	
		return Light_Sensor_Get(1);
}






//左下角第一位为ready position 位
//第一步一定要确保此位有试管
//转动后保证
//无 1   有 0
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


//右上角第一位为left position 位
//操作后此位置并不一定有试管
//转动后保证
//无 1   有 0
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
//确保有试管从next到blank
//转动后保证-->电机复位
//无 1   有 0
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



//左上角 wait  右上角 left
//left ->wait
//如果left有试管就确保转给wait
//转动后保证-->电机复位
//无 1   有 0
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
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);//此处NULL只能是NULL，无需传参数
	
	
		
	for(i=1;i<SIZE_MOTOR_ARRAY;i++){
			result=ConfigMotorAllDevice(i);//配置电机参数
			init_motor_device(TempMotor);//把电机参数保存在数组里
			PowerStep_Select_Motor_Baby(i);
			
			if(!result){
				Powerstep01_Init_Register(&motor_config_array[i]);//后续再需要初始化，无需上位机填写
			}
			else
			{	
				Powerstep01_Init_Register(NULL);
			}
			printf("M[%d]-speed： %f \r\n",i,motor_config_array[i].vm.cp.maxSpeed);
  }		
	
	//开机自检
	FirstOpenMotorCheckPosition();


	
	while(1){
#if 0			
		//等待试管架放置完毕
			//刷下信号 就可以启动
			while(!Light_Sensor_Get(24)){
			//StopALLMotorMotion();
				delay_ms(10);
			}
#endif			
			while(1){
				//step 1：清空blank position
					if(!ClearAndCheckBlankPosition()){
						printf("clear blank position fail \r\n");
						break;
					}else{
							printf("clear blank position successful \r\n");
							//step 2：准备next position
							if(ReadyAndCheckNextPosition()){
									printf("ready next position fail \r\n");
									break;
							}else{
									printf("ready next position successful \r\n");
									//step 3  移动next ->blank position 电机会自动复位
									if(NextMoveTowardBlankPosition()){
												printf("next->balnk position fail \r\n");
												break;
									}else{
												printf("next->balnk position successful \r\n");
												//step 4：清空wait position
												if(!ClearAndCheckWaitPosition()){
														printf("clear wait position fail \r\n");
														break;
												}else{
														printf("clear wait position successful \r\n");
														//step 5：准备left position
														if(ReadyAndCheckLeftPosition()){
																	printf("ready left position fail \r\n");
																	break;
														}else{
																	printf("ready left position successful \r\n");
																	//step 6：移动 left->wait 电机会自动复位
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
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);//此处NULL只能是NULL，无需传参数
	
	
		
	for(i=1;i<SIZE_MOTOR_ARRAY;i++){
			result=ConfigMotorAllDevice(i);//配置电机参数
			init_motor_device(TempMotor);//把电机参数保存在数组里
			PowerStep_Select_Motor_Baby(i);
			
			if(!result){
				Powerstep01_Init_Register(&motor_config_array[i]);//后续再需要初始化，无需上位机填写
			}
			else
			{	
				Powerstep01_Init_Register(NULL);
			}
			printf("M[%d]-speed： %f \r\n",i,motor_config_array[i].vm.cp.maxSpeed);
  }		
	
	//开机自检
	FirstOpenMotorCheckPosition();


	
	while(1){
#if 0			
		//等待试管架放置完毕
			//刷下信号 就可以启动
			while(!Light_Sensor_Get(24)){
			//StopALLMotorMotion();
				delay_ms(10);
			}
#endif			
			while(1){
				//step 1：清空blank position
					if(!ClearAndCheckBlankPosition()){
						printf("clear blank position fail \r\n");
						break;
					}else{
							printf("clear blank position successful \r\n");
							//step 2：准备next position
							if(ReadyAndCheckNextPosition()){
									printf("ready next position fail \r\n");
									break;
							}else{
									printf("ready next position successful \r\n");
								
									//step 3  移动next ->blank position 电机会自动复位
									if(NextMoveTowardBlankPosition()){
												printf("next->balnk position fail \r\n");
												break;
									}else{
												printf("next->balnk position successful \r\n");
									}
								}	
								//step 4：清空wait position
								if(!ClearAndCheckWaitPosition()){
												printf("clear wait position fail \r\n");
												break;
								}else{
												printf("clear wait position successful \r\n");
												//step 5：准备left position
												if(ReadyAndCheckLeftPosition()){
															printf("ready left position fail \r\n");
															break;
												}else{
															printf("ready left position successful \r\n");
															//step 6：移动 left->wait 电机会自动复位
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