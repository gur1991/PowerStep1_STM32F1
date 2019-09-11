
#include "factory_many.h"
#include "delay.h"
#include "slave_uart_control_interface.h"
//**********************************************plan A**********************************************

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
		
		PowerStep_Select_Motor_Baby( M4_LEFT_WAIT);		
		BSP_MotorControl_Move(0, M4_LEFT_TO_WAIT, 23000);
	
		while(1)
		{
				i++;
				if(!Light_Sensor_Get(WAIT_LIGHT))
				{
						BSP_MotorControl_HardStop(0);
						break;
				}else if(i>=250)
				{
						BSP_MotorControl_HardStop(0);
						break;
				}
				delay_ms(10);
		}
	//	Choose_Single_Motor_Speed_Config(M4_LEFT_WAIT,HIGH_SPEED);
		RestSelectMotorOrgin(M4_LEFT_WAIT, M4_LIGHT, M4_WAIT_TO_LEFT, 23000);
		return Light_Sensor_Get(M4_LIGHT);
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
	if(Light_Sensor_Get(NEXT_LIGHT)==0)status|=0x02;
	
	return status;
}






/**********************************************************/
void Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
	PowerStep_Select_Motor_Baby(deviceId);	
	BSP_MotorControl_Move(0, direction, stepCount);
	BSP_MotorControl_WaitWhileActive(0);
}

void Rest_Drain_And_Wash_Motor_Orgin(void)
{
	RestSelectMotorOrgin(M10_BIG_IN_OUT,M10_LIGHT,M10_BIG_OUT, 60*1000);
	RestSelectMotorOrgin(M9_IN_OUT,M9_LIGHT,M9_OUT, 60*1000);
	Choose_Single_Motor_Speed_Config(M5_FAR_NEAR,LOW_SPEED);
	RestSelectMotorOrgin(M5_FAR_NEAR,M5_LIGHT,M5_NEAR, 15*1000);
	RestSelectMotorOrgin(M6_UP_DOWM,M6_LIGHT,M6_UP, 80*1000);
	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT,M7_MIX_V_UP, 60*1000);
}

void March_Drain_And_Wash_Motor_Orgin(void)
{
	Motor_Move_And_Wait(M10_BIG_IN_OUT, M10_BIG_IN, 25000);
	Motor_Move_And_Wait(M9_IN_OUT, M9_IN, 25000);
	Motor_Move_And_Wait(M6_UP_DOWM, M6_DOWM, 10000);
	Choose_Single_Motor_Speed_Config(M5_FAR_NEAR,NORMAL_SPEED);
	Motor_Move_And_Wait(M5_FAR_NEAR, M5_FAR, 2500);
	Motor_Move_And_Wait(M7_MIX_V, M7_MIX_V_DOWN, 25000);
}	


void Rest_Transporter_Belt(void)
{
	RestSelectMotorOrgin(M1_BLANK_NEXT,M1_LIGHT,M1_BLANK_TO_NEXT, 30*1000);
	RestSelectMotorOrgin(M4_LEFT_WAIT,M4_LIGHT,M4_WAIT_TO_LEFT, 30*1000);
}

void March_Transporter_Belt(void)
{
	Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK, 2000);
	Motor_Move_And_Wait(M4_LEFT_WAIT, M4_LEFT_TO_WAIT, 2000);
	
}	
void Rest_high_wheel(void)
{
	RestSelectMotorOrgin(M11_HIGH_TURN,M11_LIGHT,M11_RIGHT_TURN, 1500);
}
void March_high_wheel(void)
{
		Motor_Move_And_Wait(M11_HIGH_TURN, M11_LEFT_TURN, 100);

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
	
	Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK,20600);
	RestSelectMotorOrgin(M1_BLANK_NEXT,M1_LIGHT,M1_BLANK_TO_NEXT, 20600);
	Motor_Move_And_Wait(M2_BLANK_LEFT, M2_BLANK_TO_LEFT,11000);
	
	Motor_Move_And_Wait(M4_LEFT_WAIT, M4_LEFT_TO_WAIT,20600);
	RestSelectMotorOrgin(M4_LEFT_WAIT,M4_LIGHT,M4_WAIT_TO_LEFT, 20600);
	
	Motor_Move_And_Wait(M3_WAIT_NEXT, M3_WAIT_TO_NEXT,11000);

/*		
	Motor_Move_And_Wait(M5_FAR_NEAR, M5_FAR,5000);
	Motor_Move_And_Wait(M6_UP_DOWM, M6_DOWM,60000);
	RestSelectMotorOrgin(M6_UP_DOWM,M6_LIGHT,M6_UP, 60000);
	RestSelectMotorOrgin(M5_FAR_NEAR,M5_LIGHT,M5_NEAR, 5000);
	

	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT_WORK,M7_MIX_V_DOWN, 65000);
	Motor_Move_And_Wait(M8_MIX, M8_MIX_LEFT,15000);
	Motor_Move_And_Wait(M8_MIX, M8_MIX_RIGHT,15000);
	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT,M7_MIX_V_UP, 65000);


	Motor_Move_And_Wait(M9_IN_OUT, M9_IN,36000);
	Motor_Move_And_Wait(M10_BIG_IN_OUT, M10_BIG_IN,36000);
	RestSelectMotorOrgin(M9_IN_OUT,M9_LIGHT,M9_OUT, 36000);
	RestSelectMotorOrgin(M10_BIG_IN_OUT,M10_LIGHT,M10_BIG_OUT, 36000);

	Motor_Move_And_Wait(M11_HIGH_TURN, M11_LEFT_TURN,6000);
	RestSelectMotorOrgin(M11_HIGH_TURN,M11_LIGHT,M11_RIGHT_TURN, 6000);
*/
}



void Scan_Motor_Slow_Spin(void)
{
	Choose_Single_Motor_Speed_Config(M8_MIX,LOW_SPEED);
	PowerStep_Select_Motor_Baby(M8_MIX);	
	BSP_MotorControl_Move(0, M8_MIX_LEFT, 16000);
	
}

void Mix_Blood_High_Speed(void)
{
	Choose_Single_Motor_Speed_Config(M8_MIX,NORMAL_SPEED);
	PowerStep_Select_Motor_Baby(M8_MIX);	
	BSP_MotorControl_Move(0, M8_MIX_LEFT, 36000);
	BSP_MotorControl_WaitWhileActive(0);
	BSP_MotorControl_Move(0, M8_MIX_RIGHT, 36000);
	BSP_MotorControl_WaitWhileActive(0);
}	


/***********************************************************/










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
				value = Belt_Move_At_SameTime();
				break;
			
			default:
					break;
		}	
	return value;
}


