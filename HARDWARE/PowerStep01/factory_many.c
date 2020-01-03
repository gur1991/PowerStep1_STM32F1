
#include "factory_many.h"
#include "delay.h"
#include "slave_uart_control_interface.h"
//**********************************************plan A**********************************************
void Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
	
#if (USE_CLEANING_DILUTION_BOARD|USE_AUTOMATIC_INJECTION_BOARD)	
	PowerStep_Select_Motor_Baby(deviceId);	
	BSP_MotorControl_Move(0, direction, stepCount);
	BSP_MotorControl_WaitWhileActive(0);
#endif
	
}
/*
//右下角第一位为blank position 位
//第一步一定要确保此为没有试管
//转动后保证
//无 1   有 0
*/
uint8_t ClearAndCheckBlankPosition(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD				
	int i=0;
	uint8_t value=0;
	while(1)
	{
		i++;
		PowerStep_Select_Motor_Baby(M6_BLANK_LEFT);
		BSP_MotorControl_Move(0, M6_BLANK_TO_LEFT, 1500);
		BSP_MotorControl_WaitWhileActive(0);
		value=Light_Sensor_Get(BLANK_LIGHT);

		if(value||i>=2)break;
	}
	return value;
#endif
		return 0;		
}


/*
//右下角第一位为wait position 位
//第一步一定要确保此为没有试管
//转动后保证
//无 1   有 0

*/
uint8_t ClearAndCheckWaitPosition(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD				
	int i=0;
	uint8_t value=0;
	while(1)
	{
		i++;
		PowerStep_Select_Motor_Baby(M5_WAIT_NEXT);	
		BSP_MotorControl_Move(0, M5_WAIT_TO_NEXT, 1500);
		BSP_MotorControl_WaitWhileActive(0);
		value=Light_Sensor_Get(WAIT_LIGHT);

		if(value||i>=2)break;
	}
	return value;
	
#endif
		return 0;			
}

//左下角第一位为ready next position 位
//第一步一定要确保此位有试管
//转动后保证
//无 1   有 0
uint8_t ReadyAndCheckNextPosition(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
	int i=0;
	uint8_t value=0;
	PowerStep_Select_Motor_Baby(M5_WAIT_NEXT);
	BSP_MotorControl_Move(0, M5_WAIT_TO_NEXT, 20000);

	while(1)
	{
		i++;
		value=Light_Sensor_Get(NEXT_LIGHT);
		if(!value)
		{
				BSP_MotorControl_HardStop(0);
				BSP_MotorControl_Move(0, M5_WAIT_TO_NEXT, 5000);
				BSP_MotorControl_WaitWhileActive(0);
				break;
		}else if(i>=450)
		{
				BSP_MotorControl_HardStop(0);
				break;
		}
		delay_ms(10);
	}
	
	return value;
#endif
		return 0;			
}


//右上角第一位为left position 位
//操作后此位置并不一定有试管
//转动后保证
//无 1   有 0
uint8_t ReadyAndCheckLeftPosition(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD		
		int i=0;
		PowerStep_Select_Motor_Baby(M6_BLANK_LEFT);
		BSP_MotorControl_Move(0, M6_BLANK_TO_LEFT, 20000);
		while(1)
		{
				  i++;
					if(!Light_Sensor_Get(LEFT_LIGHT))
					{			
								BSP_MotorControl_HardStop(0);
								BSP_MotorControl_Move(0, M6_BLANK_TO_LEFT, 5000);
								BSP_MotorControl_WaitWhileActive(0);
								break;
					}else if(i>=450)
					{
								BSP_MotorControl_HardStop(0);
								break;
					}
					delay_ms(10);
		}
		return Light_Sensor_Get(LEFT_LIGHT);
#endif
		return 0;		
}

//左上角 wait  右上角 left
//left ->wait
//如果left有试管就确保转给wait
//转动后保证-->电机复位
//无 1   有 0
uint8_t LeftMoveTowardWaitPosition(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
		int i=0;
		PowerStep_Select_Motor_Baby(M3_LEFT_WAIT);
		BSP_MotorControl_Move(0, M3_LEFT_TO_WAIT, 23000);
		while(1)
		{
				i++;
				if(!Light_Sensor_Get(WAIT_LIGHT))
				{
						BSP_MotorControl_HardStop(0);
						break;
				}else if(i>=800)
				{
						BSP_MotorControl_HardStop(0);
						break;
				}
				delay_ms(10);
		}
		//RestSelectMotorOrgin(M4_LEFT_WAIT,M4_LIGHT,M4_WAIT_TO_LEFT, 40*1000);
		return Light_Sensor_Get(WAIT_LIGHT);
		
#endif
		return 0;
}

//**********************************************plan B**********************************************

uint8_t Belt_Move_At_SameTime(void)
{
	uint8_t status=0x00;
	
#if USE_AUTOMATIC_INJECTION_BOARD		
	PowerStep_Select_Motor_Baby(M5_WAIT_NEXT);
	BSP_MotorControl_Move(0, M5_WAIT_TO_NEXT, 11000);
	PowerStep_Select_Motor_Baby(M6_BLANK_LEFT);
	BSP_MotorControl_Move(0, M6_BLANK_TO_LEFT, 11000);
	BSP_MotorControl_WaitWhileActive(0);
	
	if(Light_Sensor_Get(LEFT_LIGHT)==0)status|=0x01;
	if(Light_Sensor_Get(NEXT_LIGHT)==0)status|=0x10;
#endif
	
	return status;
}






/**********************************************************/

void Rest_Sample_Motor_Orgin(void)
{
#if USE_CLEANING_DILUTION_BOARD	
	RestSelectMotorOrgin(M10_UP_DOWM,M10_LIGHT,M10_UP, 600*1000);
	
	Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);
	RestSelectMotorOrgin(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 15*1000);
	Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
	
#endif
}


void Rest_Drain_And_Wash_Motor_Orgin(void)
{
#if USE_CLEANING_DILUTION_BOARD		
	RestSelectMotorOrgin(M8_BIG_IN_OUT,M8_LIGHT,M8_BIG_OUT, 800*1000);
	RestSelectMotorOrgin(M9_IN_OUT,M9_LIGHT,M9_OUT, 800*1000);
#endif	
}

void March_Drain_And_Wash_Motor_Orgin(void)
{
#if USE_CLEANING_DILUTION_BOARD			
	if(Light_Sensor_Get(M8_LIGHT)==0)
		Motor_Move_And_Wait(M8_BIG_IN_OUT, M8_BIG_IN, 10000);
	
	if(Light_Sensor_Get(M9_LIGHT)==0)
		Motor_Move_And_Wait(M9_IN_OUT, M9_IN, 10000);
#endif
}	


void Rest_Transporter_Belt(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD		
	RestSelectMotorOrgin(M4_BLANK_NEXT,M4_LIGHT,M4_BLANK_TO_NEXT, 40*1000);
	RestSelectMotorOrgin(M3_LEFT_WAIT,M3_LIGHT,M3_WAIT_TO_LEFT, 40*1000);
	RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 60*10000);
#endif
}

void March_Transporter_Belt(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	if(Light_Sensor_Get(M4_LIGHT)==0)
			Motor_Move_And_Wait(M4_BLANK_NEXT, M4_NEXT_TO_BLANK, 2000);
	
	if(Light_Sensor_Get(M3_LIGHT)==0)
			Motor_Move_And_Wait(M3_LEFT_WAIT, M3_LEFT_TO_WAIT, 2000);
	
	if(Light_Sensor_Get(M1_LIGHT)==0)
		Motor_Move_And_Wait(M1_MIX_V, M1_MIX_V_DOWN, 10000);
#endif	
}	
void Rest_high_wheel(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	RestSelectMotorOrgin(M7_HIGH_TURN,M7_LIGHT,M7_BACK_TURN, 13500);
#endif
}
void March_high_wheel(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	if(Light_Sensor_Get(M7_LIGHT)==0)
		  Motor_Move_And_Wait(M7_HIGH_TURN, M7_FRONT_TURN, 600);
#endif
}	


//F5 //F6
void RestFarAndDownMotorOrgin(void)
{
#if USE_CLEANING_DILUTION_BOARD	
	if(!Light_Sensor_Get(M11_LIGHT)&&!Light_Sensor_Get(M10_LIGHT))
	{
			Motor_Move_And_Wait(M11_FAR_NEAR, M11_FAR, 750);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);
			RestSelectMotorOrgin(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 10000);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
		
			Motor_Move_And_Wait(M11_FAR_NEAR, M11_FAR, 750);
			Motor_Move_And_Wait(M10_UP_DOWM, M10_DOWM, 60000);
			RestSelectMotorOrgin(M10_UP_DOWM,M10_LIGHT,M10_UP, 80*10000);
		
		  Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);
			RestSelectMotorOrgin(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 10000);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
	
	}else	if(!Light_Sensor_Get(M10_LIGHT)&&Light_Sensor_Get(M11_LIGHT))
	{
		  Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);
			RestSelectMotorOrgin(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 60000);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
		
		  Motor_Move_And_Wait(M11_FAR_NEAR, M11_FAR, 750);
			Motor_Move_And_Wait(M10_UP_DOWM, M10_DOWM, 60000);
			RestSelectMotorOrgin(M10_UP_DOWM,M10_LIGHT,M10_UP, 80*10000);
		
		  Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);
			RestSelectMotorOrgin(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 10000);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
		
	}else	if(Light_Sensor_Get(M10_LIGHT)&&!Light_Sensor_Get(M11_LIGHT))
	{
			RestSelectMotorOrgin(M10_UP_DOWM,M10_LIGHT,M10_UP, 80*10000);
			Motor_Move_And_Wait(M11_FAR_NEAR, M11_FAR, 750);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);
			RestSelectMotorOrgin(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 10000);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
	}else	if(Light_Sensor_Get(M10_LIGHT)&&Light_Sensor_Get(M11_LIGHT))
	{
		  RestSelectMotorOrgin(M10_UP_DOWM,M10_LIGHT,M10_UP, 80*10000);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);
			RestSelectMotorOrgin(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 12000);
			Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
	}
#endif	
}	

void First_Open_Motor_AutoCheck_Sensor(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD
	March_Transporter_Belt();
	March_high_wheel();
	Rest_Transporter_Belt();
	Rest_high_wheel();
#endif 
}
//5 6 9 10
void First_Open_Motor_AutoCheck_Motor(void)
{
#if USE_CLEANING_DILUTION_BOARD	
	RestFarAndDownMotorOrgin();
	March_Drain_And_Wash_Motor_Orgin();
	Rest_Drain_And_Wash_Motor_Orgin();
#endif
}	



void Scan_Motor_Slow_Spin(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD		
	Choose_Single_Motor_Speed_Config(M2_MIX,LOW_SPEED);
	PowerStep_Select_Motor_Baby(M2_MIX);	
	BSP_MotorControl_Move(0, M2_MIX_RIGHT, 86000);
#endif	
}

void Mix_Blood_High_Speed(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
	int i=0;
	Choose_Single_Motor_Speed_Config(M2_MIX,NORMAL_SPEED);
	PowerStep_Select_Motor_Baby(M2_MIX);	
	
	//for(i=0;i<4;i++)
	{
		BSP_MotorControl_Move(0, M2_MIX_RIGHT, 2000);
		BSP_MotorControl_WaitWhileActive(0);
		BSP_MotorControl_Move(0, M2_MIX_LEFT, 2000);
		BSP_MotorControl_WaitWhileActive(0);
	}
	RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 60*10000);
#endif	
}	
void Mix_Work_Goto_Postion(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD		
	RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT_WORK,M1_MIX_V_DOWN, 600*1000);
	//Motor_Move_And_Wait(M7_MIX_V, M7_MIX_V_DOWN, 3000);
#endif
}



//有->无-有
void Normal_Pitch_Move_Next_The_Last_Two(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD		
	Motor_Move_And_Wait(M4_BLANK_NEXT, M4_NEXT_TO_BLANK,800);
	RestSelectMotorOrgin(M4_BLANK_NEXT,NORMAL_CHECK_DRAIN_LIGHT,M4_NEXT_TO_BLANK, 5000);
#endif
}

//有->无-有
void Normal_Pitch_Move_Next(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD		
	Motor_Move_And_Wait(M4_BLANK_NEXT, M4_NEXT_TO_BLANK,800);
	RestSelectMotorOrgin(M4_BLANK_NEXT,NORMAL_NEXT_LIGHT,M4_NEXT_TO_BLANK, 5000);
#endif
}

void Normal_Goto_First_Position(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
	RestSelectMotorOrgin(M4_BLANK_NEXT,NORMAL_NEXT_LIGHT,M4_NEXT_TO_BLANK, 5000);
	//Normal_Pitch_Move_Next();
#endif	
}

void Normal_Move_Blank(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
		Choose_Single_Motor_Speed_Config(M4_BLANK_NEXT,LOW_SPEED);
		RestSelectMotorOrgin(M4_BLANK_NEXT,BLANK_LIGHT,M4_NEXT_TO_BLANK, 8000);
	  Choose_Single_Motor_Speed_Config(M4_BLANK_NEXT,NORMAL_SPEED);
#endif	
}	

/***********************************************************/
//time ms

void  Rest_Injection_Module_Motor(uint32_t up_Steps,uint32_t big_Steps,int time)
{
#if USE_CLEANING_DILUTION_BOARD		
			int i=0;
			int flag1=0,flag2=0;

			PowerStep_Select_Motor_Baby(M8_BIG_IN_OUT);
			BSP_MotorControl_Move(0, M8_BIG_OUT, big_Steps);
		
			delay_ms(time);
			PowerStep_Select_Motor_Baby(M10_UP_DOWM);
			BSP_MotorControl_Move(0, M10_UP, up_Steps);
			
		while(1){
				i++;
				if(!Light_Sensor_Get(M10_LIGHT)&&!flag1)
				{		PowerStep_Select_Motor_Baby(M8_BIG_IN_OUT);				
						BSP_MotorControl_HardStop(0);	
						flag1=1;
				}
				if(!Light_Sensor_Get(M10_LIGHT)&&!flag2)
				{		PowerStep_Select_Motor_Baby(M10_UP_DOWM);			
						BSP_MotorControl_HardStop(0);
						flag2=1;					
						
				}	
				if(flag1&&flag2) break;
				
				if(i>=1500){
							PowerStep_Select_Motor_Baby(M8_BIG_IN_OUT);
							BSP_MotorControl_HardStop(0);
							PowerStep_Select_Motor_Baby(M10_UP_DOWM);
							BSP_MotorControl_HardStop(0);
							break;	
				}
				

				delay_ms(10);	
		}
#endif			
}	


//dischard
void mix_and_reach_position(void)	
{
/*
		//下发混匀动作
		Choose_Single_Motor_Speed_Config(M2_MIX,NORMAL_SPEED);
		PowerStep_Select_Motor_Baby(M2_MIX);
		BSP_MotorControl_Move(0, M2_MIX_RIGHT, 120000);
	
		//复位电机
		Rest_Sample_Motor_Orgin();

	  //执行混匀剩余动作
		PowerStep_Select_Motor_Baby(M2_MIX_LEFT);
		BSP_MotorControl_HardStop(0);
		
		PowerStep_Select_Motor_Baby(M2_MIX_LEFT);
		BSP_MotorControl_Move(0, M2_MIX_LEFT, 66000);
		BSP_MotorControl_WaitWhileActive(0);
		
		RestSelectMotorOrgin(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 60*1000);
*/
}	




uint8_t process_motor_command_receive(Command_Package_t command)
{
		uint8_t value=0;
	
	printf("type command:%d\r\n",command);
	
		switch(command)
		{
			case SLEF_TEST:	
					printf("Nor command !\r\n");
					break;
			
			
			case FIRST_START_CHECK_SENSOR_BOARD:
					First_Open_Motor_AutoCheck_Sensor();
				  break;
			case FIRST_START_CHECK_MOTOR_BOARD:
					First_Open_Motor_AutoCheck_Motor();
				  break;
			case REST_SENSOR_BOARD_MOTOR:
					Rest_Transporter_Belt();
					Rest_high_wheel();
				break;
			case REST_MOTOR_BOARD_MOTOR:
					Rest_Sample_Motor_Orgin();
					Rest_Drain_And_Wash_Motor_Orgin();
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
			case MIX_WORK_GOTO:
				Mix_Work_Goto_Postion();
				break;
			case NORMAL_PITCH_MOVE_NEXT:
				Normal_Pitch_Move_Next();
				break;
			case NORMAL_BLANK_REST:
				Normal_Move_Blank();
				break;
			case REST_SAMPLE_MOTOR:
				Rest_Sample_Motor_Orgin();
				break;
			case NORMAL_FIRST_POSITON:
				Normal_Goto_First_Position();
				break;
			case MIX_AND_REACH_POSITION:
				mix_and_reach_position();//已被废弃的指令
				break;
			case BIG_IN_OUT_SLOW:
				Choose_Single_Motor_Speed_Config(M8_BIG_IN_OUT,LOW_SPEED);
				break;
			case BIG_IN_OUT_NORMAL:
				Choose_Single_Motor_Speed_Config(M8_BIG_IN_OUT,NORMAL_SPEED);
				break;
			
			case NORMAL_PITCH_MOVE_NEXT_THE_LAST_TWO:
					Normal_Pitch_Move_Next_The_Last_Two();
				break;
			
			default:
					break;
		}	
	return value;
}


