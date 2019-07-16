
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
								BSP_MotorControl_HardStop(0);
							//光感检测到后，多跑一会，让试管摆正
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
					if(!Light_Sensor_Get(12))
					{			
								BSP_MotorControl_HardStop(0);
								//光感检测到后，多跑一会，让试管摆正
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
//确保有试管从next到blank
//转动后保证-->电机复位
//无 1   有 0
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
							BSP_MotorControl_WaitWhileActive(0);//走过一个试管的长度，避免重复检测一个
							BSP_MotorControl_Move(0, FORWARD, 20000);//再继续前行
							times-=80;
					}
					if(!Light_Sensor_Get(4)){
							BSP_MotorControl_HardStop(0);
							break;
					}else if(i>=times){
							//此处添加的目的是，防止传感器每检测到试管，也要停止
							//停止计算的方法是预设跑一次25s，每检测到一个试管减少0.8s，最多减少10s
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
		
	  //水平向外5500
		PowerStep_Select_Motor_Baby(5);
		BSP_MotorControl_Move(0, BACKWARD, 5500);
		BSP_MotorControl_WaitWhileActive(0);
		
		//垂直向下170000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, FORWARD, 170000);
		BSP_MotorControl_WaitWhileActive(0);
		InjectLiquidTask();
#if 1	
		//垂直向上70000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, BACKWARD, 70000);
		BSP_MotorControl_WaitWhileActive(0);
	
		//水平向内1000
		PowerStep_Select_Motor_Baby(5);
		BSP_MotorControl_Move(0, FORWARD, 1000);
		BSP_MotorControl_WaitWhileActive(0);
		
	  //垂直向下70000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, FORWARD, 70000);
		BSP_MotorControl_WaitWhileActive(0);
		RestInjectAllPosition();
		
		//垂直向上35000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, BACKWARD, 35000);
		BSP_MotorControl_WaitWhileActive(0);
		
		//水平向内500
		PowerStep_Select_Motor_Baby(5);
		BSP_MotorControl_Move(0, FORWARD, 500);
		BSP_MotorControl_WaitWhileActive(0);
		
		//垂直向下70000
		PowerStep_Select_Motor_Baby(6);
		BSP_MotorControl_Move(0, FORWARD, 50000);
		BSP_MotorControl_WaitWhileActive(0);
		
		//垂直向上100000
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
	
	
	
	
//			InjectLiquidTask();
		//CollectSampleTask();
//	return ;
	//开机自检
	FirstOpenMotorCheckPosition();
	RestInjectAllPosition();

	
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
									//break;
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