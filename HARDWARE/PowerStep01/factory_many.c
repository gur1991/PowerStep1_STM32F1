
#include "factory_many.h"
#include "delay.h"
#include "slave_uart_control_interface.h"


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
		value=Light_Sensor_Get(WAIT_LIGHT);
		if(!value)
		{
				BSP_MotorControl_HardStop(0);
				BSP_MotorControl_Move(0, M3_WAIT_TO_NEXT, 5000);
				BSP_MotorControl_HardStop(0);
				break;
		}else if(i>=1000)
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
		int i;
		printf("ready left position \r\n");
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
					}else if(i>=1000)
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
		int i;
		printf("left ->wait\r\n");
		PowerStep_Select_Motor_Baby(M4_LEFT_WAIT);		
	
		BSP_MotorControl_Move(0, M4_LEFT_TO_WAIT, 20000);
		BSP_MotorControl_WaitWhileActive(0);
	
		Choose_Single_Motor_Speed_Config(M4_LEFT_WAIT, LOW_SPEED);
		BSP_MotorControl_Move(0, M4_LEFT_TO_WAIT, 600);
		while(1)
		{
				i++;
				if(!Light_Sensor_Get(WAIT_LIGHT))
				{
						BSP_MotorControl_HardStop(0);
						break;
				}else if(i>=200)
				{
						BSP_MotorControl_HardStop(0);
						break;
				}
				delay_ms(10);
		}
		Choose_Single_Motor_Speed_Config(M4_LEFT_WAIT, HIGH_SPEED);
		RestSelectMotorOrgin(M4_LEFT_WAIT, M4_LIGHT, M4_WAIT_TO_LEFT, 20600);
		
		return Light_Sensor_Get(M4_LIGHT);
}	
/**********************************************************/

void Rest_Drain_And_Wash_Motor_Orgin(void)
{
	RestSelectMotorOrgin(M10_BIG_IN_OUT,M10_LIGHT,M10_BIG_OUT, 60*1000);
	RestSelectMotorOrgin(M9_IN_OUT,M9_LIGHT,M9_OUT, 60*1000);
	RestSelectMotorOrgin(M6_UP_DOWM,M6_LIGHT,M6_UP, 60*1000);
	RestSelectMotorOrgin(M5_FAR_NEAR,M5_LIGHT,M5_NEAR, 60*1000);
	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT,M7_MIX_V_UP, 60*1000);
}

void Rest_Transporter_Belt(void)
{
	RestSelectMotorOrgin(M1_BLANK_NEXT,M1_LIGHT,M1_BLANK_TO_NEXT, 60*1000);
	RestSelectMotorOrgin(M4_LEFT_WAIT,M4_LIGHT,M4_WAIT_TO_LEFT, 60*1000);
}

void Rest_high_wheel(void)
{
	RestSelectMotorOrgin(M11_HIGH_TURN,M11_LIGHT,M11_RIGHT_TURN, 60*1000);
}

void RestAllMotorOrgin(void)
{
	Rest_Drain_And_Wash_Motor_Orgin();
	Rest_Transporter_Belt();
	Rest_high_wheel();
}

void Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
	PowerStep_Select_Motor_Baby(deviceId);	
	BSP_MotorControl_Move(0, direction, stepCount);
	BSP_MotorControl_WaitWhileActive(0);
}


void First_Open_Motor_AutoCheck(void)
{
	RestAllMotorOrgin();
	
	
	Motor_Move_And_Wait(M1_BLANK_NEXT, M1_NEXT_TO_BLANK,20600);
	RestSelectMotorOrgin(M1_BLANK_NEXT,M1_LIGHT,M1_BLANK_TO_NEXT, 20600);
	Motor_Move_And_Wait(M2_BLANK_LEFT, M2_BLANK_TO_LEFT,11000);
	
	Motor_Move_And_Wait(M4_LEFT_WAIT, M4_LEFT_TO_WAIT,20600);
	RestSelectMotorOrgin(M4_LEFT_WAIT,M4_LIGHT,M4_WAIT_TO_LEFT, 20600);
	
	Motor_Move_And_Wait(M3_WAIT_NEXT, M3_WAIT_TO_NEXT,11000);
	
	Motor_Move_And_Wait(M5_FAR_NEAR, M5_FAR,5000);
	Motor_Move_And_Wait(M6_UP_DOWM, M6_DOWM,5000);
	RestSelectMotorOrgin(M5_FAR_NEAR,M5_LIGHT,M5_NEAR, 5000);
	RestSelectMotorOrgin(M6_UP_DOWM,M6_LIGHT,M6_UP, 5000);

	
	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT_WORK,M7_MIX_V_DOWN, 5000);
	Motor_Move_And_Wait(M8_MIX, M8_MIX_LEFT,15000);
	Motor_Move_And_Wait(M8_MIX, M8_MIX_RIGHT,15000);
	RestSelectMotorOrgin(M7_MIX_V,M7_LIGHT_WORK,M7_MIX_V_UP, 5000);


	Motor_Move_And_Wait(M9_IN_OUT, M9_IN,6000);
	Motor_Move_And_Wait(M10_BIG_IN_OUT, M10_BIG_IN,6000);
	RestSelectMotorOrgin(M9_IN_OUT,M9_LIGHT,M9_OUT, 6000);
	RestSelectMotorOrgin(M10_BIG_IN_OUT,M10_LIGHT,M10_BIG_OUT, 6000);

	Motor_Move_And_Wait(M11_HIGH_TURN, M11_LEFT_TURN,6000);
	RestSelectMotorOrgin(M11_HIGH_TURN,M11_LIGHT,M11_RIGHT_TURN, 6000);

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
			
			default:
					break;
		}	
	return value;
}


