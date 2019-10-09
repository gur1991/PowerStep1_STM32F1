
#include "factory_many.h"
#include "delay.h"
#include "slave_uart_control_interface.h"
//**********************************************plan A**********************************************
void Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
	PowerStep_Select_Motor_Baby(deviceId);	
	BSP_MotorControl_Move(0, direction, stepCount);
	BSP_MotorControl_WaitWhileActive(0);
}
/*
//右下角第一位为blank position 位
//第一步一定要确保此为没有试管
//转动后保证
//无 1   有 0
*/
uint8_t ClearAndCheckBlankPosition(void)
{
	int i=0;
	uint8_t value=0;
	while(1)
	{
		i++;
		PowerStep_Select_Motor_Baby(M2_BLANK_LEFT);
		BSP_MotorControl_Move(0, M2_BLANK_TO_LEFT, 1500);
		BSP_MotorControl_WaitWhileActive(0);
		value=Light_Sensor_Get(BLANK_LIGHT);

		if(value||i>=2)break;
	}
	return value;
}


/*
//右下角第一位为wait position 位
//第一步一定要确保此为没有试管
//转动后保证
//无 1   有 0

*/
uint8_t ClearAndCheckWaitPosition(void)
{
	int i=0;
	uint8_t value=0;
	while(1)
	{
		i++;
		PowerStep_Select_Motor_Baby(M3_WAIT_NEXT);	
		BSP_MotorControl_Move(0, M3_WAIT_TO_NEXT, 1500);
		BSP_MotorControl_WaitWhileActive(0);
		value=Light_Sensor_Get(WAIT_LIGHT);

		if(value||i>=2)break;
	}
	return value;
}

//左下角第一位为ready next position 位
//第一步一定要确保此位有试管
//转动后保证
//无 1   有 0
uint8_t ReadyAndCheckNextPosition(void)
{
	int i=0;
	uint8_t value=0;
	PowerStep_Select_Motor_Baby(M3_WAIT_NEXT);
	BSP_MotorControl_Move(0, M3_WAIT_TO_NEXT, 11000);

	while(1)
	{
		i++;
		value=Light_Sensor_Get(NEXT_LIGHT);
		if(!value)
		{
				BSP_MotorControl_HardStop(0);
				BSP_MotorControl_Move(0, M3_WAIT_TO_NEXT, 5000);
				BSP_MotorControl_HardStop(0);
				break;
		}else if(i>=250)
		{
				BSP_MotorControl_HardStop(0);
				break;
		}
		delay_ms(10);
	}
	
	return value;
}


//右上角第一位为left position 位
//操作后此位置并不一定有试管
//转动后保证
//无 1   有 0
uint8_t ReadyAndCheckLeftPosition(void)
{
		int i=0;
		PowerStep_Select_Motor_Baby(M2_BLANK_LEFT);
		BSP_MotorControl_Move(0, M2_BLANK_TO_LEFT, 11000);
		while(1)
		{
				  i++;
					if(!Light_Sensor_Get(LEFT_LIGHT))
					{			
								BSP_MotorControl_HardStop(0);
								BSP_MotorControl_Move(0, M2_BLANK_TO_LEFT, 5000);
								BSP_MotorControl_WaitWhileActive(0);
								BSP_MotorControl_HardStop(0);
								break;
					}else if(i>=250)
					{
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(10);
		}
		return Light_Sensor_Get(LEFT_LIGHT);
}

//左上角 wait  右上角 left
//left ->wait
//如果left有试管就确保转给wait
//转动后保证-->电机复位
//无 1   有 0
uint8_t LeftMoveTowardWaitPosition(void)
{
		int i=0;
		PowerStep_Select_Motor_Baby(M4_LEFT_WAIT);
		BSP_MotorControl_Move(0, M4_LEFT_TO_WAIT, 23000);
		while(1)
		{
				i++;
				if(!Light_Sensor_Get(WAIT_LIGHT))
				{
						BSP_MotorControl_HardStop(0);
						break;
				}else if(i>=1000)
				{
						BSP_MotorControl_HardStop(0);
						break;
				}
				delay_ms(10);
		}
		//RestSelectMotorOrgin(M4_LEFT_WAIT,M4_LIGHT,M4_WAIT_TO_LEFT, 40*1000);
		return Light_Sensor_Get(WAIT_LIGHT);
}

//**********************************************plan B**********************************************

uint8_t Belt_Move_At_SameTime(void)
{
	uint8_t status=0x00;
	
	PowerStep_Select_Motor_Baby(M3_WAIT_NEXT);
	BSP_MotorControl_Move(0, M3_WAIT_TO_NEXT, 11000);
	PowerStep_Select_Motor_Baby(M2_BLANK_LEFT);
	BSP_MotorControl_Move(0, M2_BLANK_TO_LEFT, 11000);
	BSP_MotorControl_WaitWhileActive(0);
	
	if(Light_Sensor_Get(LEFT_LIGHT)==0)status|=0x01;
	if(Light_Sensor_Get(NEXT_LIGHT)==0)status|=0x10;
	
	return status;
}






/**********************************************************/

void Rest_Sample_Motor_Orgin(void)
{
	RestSelectMotorOrgin(M6_UP_DOWM,M6_LIGHT,M6_UP, 300*1000);
	RestSelectMotorOrgin(M5_FAR_NEAR,M5_LIGHT,M5_NEAR, 15*1000);
}


void Rest_Drain_And_Wash_Motor_Orgin(void)
{
	RestSelectMotorOrgin(M10_BIG_IN_OUT,M10_LIGHT,M10_BIG_OUT, 200*1000);
	RestSelectMotorOrgin(M9_IN_OUT,M9_LIGHT,M9_OUT, 200*1000);
	Choose_Single_Motor_Speed_Config(M5_FAR_NEAR,LOW_SPEED);
	//RestSelectMotorOrgin(M6_UP_DOWM,M6_LIGHT,M6_UP, 300*1000);
	RestSelectMotorOrgin(M5_FAR_NEAR,M5_LIGHT,M5_NEAR, 15*1000);
	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT,M7_MIX_V_UP, 60*1000);
}

void March_Drain_And_Wash_Motor_Orgin(void)
{
	if(Light_Sensor_Get(M10_LIGHT)==0)
		Motor_Move_And_Wait(M10_BIG_IN_OUT, M10_BIG_IN, 10000);
	
	if(Light_Sensor_Get(M9_LIGHT)==0)
		Motor_Move_And_Wait(M9_IN_OUT, M9_IN, 10000);
	
	if(Light_Sensor_Get(M5_LIGHT)==0){
		Choose_Single_Motor_Speed_Config(M5_FAR_NEAR,NORMAL_SPEED);
		Motor_Move_And_Wait(M5_FAR_NEAR, M5_FAR, 2500);
	}
//	if(Light_Sensor_Get(M6_LIGHT)==0)
//		Motor_Move_And_Wait(M6_UP_DOWM, M6_DOWM, 3*10000);
	
	if(Light_Sensor_Get(M7_LIGHT)==0)
		Motor_Move_And_Wait(M7_MIX_V, M7_MIX_V_DOWN, 10000);
}	

void Rest_And_March_Up_Down(void)
{
	Motor_Move_And_Wait(M5_FAR_NEAR, M5_FAR, 5600);
	if(Light_Sensor_Get(M6_LIGHT)==0)
		Motor_Move_And_Wait(M6_UP_DOWM, M6_DOWM, 10000);
	RestSelectMotorOrgin(M6_UP_DOWM,M6_LIGHT,M6_UP, 200*1000);
	RestSelectMotorOrgin(M5_FAR_NEAR,M5_LIGHT,M5_NEAR, 15*1000);
}	


void Rest_Transporter_Belt(void)
{
	RestSelectMotorOrgin(M1_BLANK_NEXT,M1_LIGHT,M1_BLANK_TO_NEXT, 40*1000);
	RestSelectMotorOrgin(M4_LEFT_WAIT,M4_LIGHT,M4_WAIT_TO_LEFT, 40*1000);
}

void March_Transporter_Belt(void)
{
	if(Light_Sensor_Get(M1_LIGHT)==0)
			Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK, 2000);
	
	if(Light_Sensor_Get(M4_LIGHT)==0)
			Motor_Move_And_Wait(M4_LEFT_WAIT, M4_LEFT_TO_WAIT, 2000);
	
}	
void Rest_high_wheel(void)
{
	RestSelectMotorOrgin(M11_HIGH_TURN,M11_LIGHT,M11_RIGHT_TURN, 13500);
}
void March_high_wheel(void)
{
		//Motor_Move_And_Wait(M11_HIGH_TURN, M11_LEFT_TURN, 100);

}	
void RestAllMotorOrgin(void)
{
	Rest_Drain_And_Wash_Motor_Orgin();
	Rest_Transporter_Belt();
	Rest_high_wheel();
}
void MarchAllMotorOrgin(void)
{
	March_Drain_And_Wash_Motor_Orgin();
	March_Transporter_Belt();
	March_high_wheel();
}



void First_Open_Motor_AutoCheck(void)
{
	MarchAllMotorOrgin();
	RestAllMotorOrgin();
	Rest_And_March_Up_Down();
/*	
	Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK,20600);
	RestSelectMotorOrgin(M1_BLANK_NEXT,M1_LIGHT,M1_BLANK_TO_NEXT, 50*1000);
	Belt_Move_At_SameTime();
	Motor_Move_And_Wait(M4_LEFT_WAIT, M4_LEFT_TO_WAIT,20600);
	RestSelectMotorOrgin(M4_LEFT_WAIT,M4_LIGHT,M4_WAIT_TO_LEFT, 50*1000);
*/	

}



void Scan_Motor_Slow_Spin(void)
{
	Choose_Single_Motor_Speed_Config(M8_MIX,LOW_SPEED);
	PowerStep_Select_Motor_Baby(M8_MIX);	
	BSP_MotorControl_Move(0, M8_MIX_RIGHT, 86000);
	
}

void Mix_Blood_High_Speed(void)
{
	Choose_Single_Motor_Speed_Config(M8_MIX,NORMAL_SPEED);
	PowerStep_Select_Motor_Baby(M8_MIX);	
	//BSP_MotorControl_Move(0, M8_MIX_LEFT, 46000);
	//BSP_MotorControl_WaitWhileActive(0);
	BSP_MotorControl_Move(0, M8_MIX_RIGHT, 46000);
	BSP_MotorControl_WaitWhileActive(0);
	BSP_MotorControl_Move(0, M8_MIX_RIGHT, 66000);
	BSP_MotorControl_WaitWhileActive(0);
	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT,M7_MIX_V_UP, 60*1000);
}	
void Mix_Work_Goto_Postion(void)
{
	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT_WORK,M7_MIX_V_DOWN, 40*1000);
	//Motor_Move_And_Wait(M7_MIX_V, M7_MIX_V_DOWN, 3000);
}

	

//有->无-有
void Normal_Pitch_Move_Next(void)
{
	Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK,800);
	RestSelectMotorOrgin(M1_BLANK_NEXT,NORMAL_NEXT_LIGHT,M1_NEXT_TO_BLANK, 5000);
/*	
	if(Light_Sensor_Get(NORMAL_NEXT_LIGHT)==1)
	{
			RestSelectMotorOrgin(M1_BLANK_NEXT,NORMAL_NEXT_LIGHT,M1_NEXT_TO_BLANK, 20000);
			return;
	}else{
			PowerStep_Select_Motor_Baby(M1_BLANK_NEXT);
			BSP_MotorControl_Move(0, M1_NEXT_TO_BLANK, 20000);
			while(i--){
					if(Light_Sensor_Get(NORMAL_NEXT_LIGHT)==1){
						BSP_MotorControl_HardStop(0);
						RestSelectMotorOrgin(M1_BLANK_NEXT,NORMAL_NEXT_LIGHT,M1_NEXT_TO_BLANK, 20000);
						return;
					}	
					delay_ms(10);
			}
			BSP_MotorControl_HardStop(0);
			return ;
	}
*/
}

void Normal_Goto_First_Position(void)
{
	RestSelectMotorOrgin(M1_BLANK_NEXT,NORMAL_NEXT_LIGHT,M1_NEXT_TO_BLANK, 5000);
	Normal_Pitch_Move_Next();
}

void Normal_Blank_Rest(void)
{
		Choose_Single_Motor_Speed_Config(M1_BLANK_NEXT,LOW_SPEED);
		RestSelectMotorOrgin(M1_BLANK_NEXT,BLANK_LIGHT,M1_NEXT_TO_BLANK, 8000);
	  Choose_Single_Motor_Speed_Config(M1_BLANK_NEXT,NORMAL_SPEED);
}	

/***********************************************************/
//time ms

void  Rest_Injection_Module_Motor(uint32_t up_Steps,uint32_t big_Steps,int time)
{
			int i=0;
			int flag1=0,flag2=0;

			PowerStep_Select_Motor_Baby(M10_BIG_IN_OUT);
			BSP_MotorControl_Move(0, M10_BIG_OUT, big_Steps);
		
			delay_ms(time);
			PowerStep_Select_Motor_Baby(M6_UP_DOWM);
			BSP_MotorControl_Move(0, M6_UP, up_Steps);
			
		while(1){
				i++;
				if(!Light_Sensor_Get(M10_LIGHT)&&!flag1)
				{		PowerStep_Select_Motor_Baby(M10_BIG_IN_OUT);				
						BSP_MotorControl_HardStop(0);	
						flag1=1;
				}
				if(!Light_Sensor_Get(M6_LIGHT)&&!flag2)
				{		PowerStep_Select_Motor_Baby(M6_UP_DOWM);			
						BSP_MotorControl_HardStop(0);
						flag2=1;					
						
				}	
				if(flag1&&flag2) break;
				
				if(i>=1500){
							PowerStep_Select_Motor_Baby(M10_BIG_IN_OUT);
							BSP_MotorControl_HardStop(0);
							PowerStep_Select_Motor_Baby(M6_UP_DOWM);
							BSP_MotorControl_HardStop(0);
							break;	
				}
				

				delay_ms(10);	
		}		
}	





uint8_t process_motor_command_receive(Command_Package_t command)
{
		uint8_t value=0;
	
		switch(command)
		{
			case SLEF_TEST:	
					printf("Nor command !\r\n");
					break;
			case FIRST_START_CHECK_MOTOR:
					First_Open_Motor_AutoCheck();
				break;
			case REST_ALL_MOTOR:
					RestAllMotorOrgin();
				break;
			case CLEAR_BLANK:
					value=ClearAndCheckBlankPosition();
				break;
			case CLEARL_WAIT:
					value=ClearAndCheckWaitPosition();
				break;
			case READY_NEXT:
					value=ReadyAndCheckNextPosition();
				break;
			case READY_LEFT:
					value=ReadyAndCheckLeftPosition();
				break;
			case LEFT_MOVE_TO_WAIT:
				printf("left to wait \r\n");
					value=LeftMoveTowardWaitPosition();
				break;
			case REST_C55_C52:
					value=Rest_C55_C52_Position();
				break;
			case REST_HIGH_WHEEL:
				Rest_high_wheel();
				break;
			case REST_TRANSPORTER_BELT:
				Rest_Transporter_Belt();
				break;
			case REST_DRAIN_AND_WASH_MOTOR:
				Rest_Drain_And_Wash_Motor_Orgin();
				break;
			case REACH_DEGREE_WAIT:
				value=Rearch_Degree_Wait();
				break;
			case MIX_SCAN_SLOW:
				Scan_Motor_Slow_Spin();
				break;
			case MIX_BLOOD_HIGH:
				Mix_Blood_High_Speed();
				break;
			case BELT_MOVE_SAMETIME:
				printf("same time move \r\n");
				value = Belt_Move_At_SameTime();
				break;
			case MIX_WORK_GOTO:
				Mix_Work_Goto_Postion();
				break;
			case NORMAL_PITCH_MOVE_NEXT:
				Normal_Pitch_Move_Next();
				break;
			case NORMAL_BLANK_REST:
				Normal_Blank_Rest();
				break;
			case REST_SAMPLE_MOTOR:
				Rest_Sample_Motor_Orgin();
				break;
			case NORMAL_FIRST_POSITION:
				Normal_Goto_First_Position();
				break;
			default:
					break;
		}	
	return value;
}


