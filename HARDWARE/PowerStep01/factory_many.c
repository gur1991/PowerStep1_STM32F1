
#include "factory_many.h"
#include "delay.h"
#include "slave_uart_control_interface.h"
#include "config.h"

//**********************************************plan A**********************************************
void Motor_Move_And_Wait(uint8_t deviceId, motorDir_t direction, uint32_t stepCount)
{
	
#if (USE_CLEANING_DILUTION_BOARD||USE_AUTOMATIC_INJECTION_BOARD)	
	
	LOGD("motor move:%d \r\n",deviceId);
	PowerStep_Select_Motor_Baby(deviceId);	
	BSP_MotorControl_Move(0, direction, stepCount);
	__BSP_MotorControl_WaitWhileActive__(deviceId);
#endif
	
}

//hal 新接口函数
void Motor_Move_And_Wait_Select(uint8_t deviceId, motorDir_t direction, uint32_t stepCount,MOTOR_SPEED_type_t speed_type)
{
	
#if (USE_CLEANING_DILUTION_BOARD||USE_AUTOMATIC_INJECTION_BOARD)	
	
	LOGD("motor move:%d \r\n",deviceId);
	__BSP_MotorControl_Move__(deviceId, direction, stepCount,speed_type);
	__BSP_MotorControl_WaitWhileActive__(deviceId);
#endif
	
}


uint8_t ClearAndCheckBlankPosition(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD				
	int i=0;
	uint8_t value=0;
	while(1)
	{
		i++;
		__BSP_MotorControl_Move__(M6_BLANK_LEFT, M6_BLANK_TO_LEFT, 6000,NORMAL_SPEED);
		__BSP_MotorControl_WaitWhileActive__(M6_BLANK_LEFT);
		value=Light_Sensor_Get(BLANK_LIGHT);

		if(value||i>=2)break;
	}
	return value;
#endif
		return 0;		
}



uint8_t ClearAndCheckWaitPosition(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD				
	int i=0;
	uint8_t value=0;
	while(1)
	{
		i++;
		__BSP_MotorControl_Move__(M5_WAIT_NEXT, M5_WAIT_TO_NEXT, 6000,NORMAL_SPEED);
		__BSP_MotorControl_WaitWhileActive__(M5_WAIT_NEXT);
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
	
	__BSP_MotorControl_Move__(M5_WAIT_NEXT, M5_WAIT_TO_NEXT, 6000,NORMAL_SPEED);

	while(1)
	{
		i++;
		value=Light_Sensor_Get(NEXT_LIGHT);
		if(!value)
		{
				BSP_MotorControl_HardStop(0);
				BSP_MotorControl_Move(0, M5_WAIT_TO_NEXT, 20000);
				__BSP_MotorControl_WaitWhileActive__(M5_WAIT_NEXT);
				break;
		}else if(i>=450)
		{
				__BSP_MotorControl_HardStop__(M5_WAIT_NEXT);
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
		__BSP_MotorControl_Move__(M6_BLANK_LEFT, M6_BLANK_TO_LEFT, 80000,NORMAL_SPEED);

		while(1)
		{
				  i++;
					if(!Light_Sensor_Get(LEFT_LIGHT))
					{			
								BSP_MotorControl_HardStop(0);
								BSP_MotorControl_Move(0, M6_BLANK_TO_LEFT, 20000);
								__BSP_MotorControl_WaitWhileActive__(M6_BLANK_LEFT);
								break;
					}else if(i>=450)
					{
								__BSP_MotorControl_HardStop__(M6_BLANK_LEFT);
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
	
		__BSP_MotorControl_Move__(M3_LEFT_WAIT, M3_LEFT_TO_WAIT, 92000,HIGH_SPEED);
	

		while(1)
		{
				i++;
				if(!Light_Sensor_Get(WAIT_LIGHT))
				{
						__BSP_MotorControl_HardStop__(M3_LEFT_WAIT);
						break;
				}else if(i>=800)
				{
						__BSP_MotorControl_HardStop__(M3_LEFT_WAIT);
						break;
				}
				delay_ms(10);
		}
		return Light_Sensor_Get(WAIT_LIGHT);
		
#endif
		return 0;
}

//**********************************************plan B**********************************************

uint8_t Belt_Move_At_SameTime(void)
{
	uint8_t status=0x00;
	
#if USE_AUTOMATIC_INJECTION_BOARD	

	
	__BSP_MotorControl_Move__(M5_WAIT_NEXT, M5_WAIT_TO_NEXT, 44000,NORMAL_SPEED);
	__BSP_MotorControl_Move__(M6_BLANK_LEFT, M6_BLANK_TO_LEFT, 44000,NORMAL_SPEED);
		
	__BSP_MotorControl_WaitWhileActive__(M6_BLANK_LEFT);
	__BSP_MotorControl_WaitWhileActive__(M5_WAIT_NEXT);

	
	
	if(Light_Sensor_Get(LEFT_LIGHT)==0)status|=0x01;
	if(Light_Sensor_Get(NEXT_LIGHT)==0)status|=0x10;
#endif
	
	return status;
}






/**********************************************************/

void Rest_Sample_Motor_Orgin(void)
{
#if USE_CLEANING_DILUTION_BOARD
	
	RestSelectMotorOrginSelect(M10_UP_DOWM,M10_LIGHT,M10_UP, 600*1000,NORMAL_SPEED);
	RestSelectMotorOrgin(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 15*1000);	
#endif
}


void Rest_Drain_And_Wash_Motor_Orgin(void)
{
#if USE_CLEANING_DILUTION_BOARD
	test_actuator(CHEMINERT_C55_CC4);
	
	RestSelectMotorOrginSelect(M8_BIG_IN_OUT,M8_LIGHT,M8_BIG_OUT, 600*1000,NORMAL_SPEED);
	RestSelectMotorOrginSelect(M9_IN_OUT,M9_LIGHT,M9_OUT, 600*1000,NORMAL_SPEED);
	
#endif	
}

void March_Drain_And_Wash_Motor_Orgin(void)
{
#if USE_CLEANING_DILUTION_BOARD	
	test_actuator(CHEMINERT_C55_CC4);	
	if(Light_Sensor_Get(M8_LIGHT)==0)
			Motor_Move_And_Wait_Select(M8_BIG_IN_OUT, M8_BIG_IN, 40000,NORMAL_SPEED);
	
	if(Light_Sensor_Get(M9_LIGHT)==0)
			Motor_Move_And_Wait_Select(M9_IN_OUT, M9_IN, 40000,NORMAL_SPEED);
#endif
}	


void Rest_Transporter_Belt(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD
	
	RestSelectMotorOrginSelect(M4_BLANK_NEXT,M4_LIGHT,M4_BLANK_TO_NEXT, 20*1000,NORMAL_SPEED);
	
	RestSelectMotorOrginSelect(M3_LEFT_WAIT,M3_LIGHT,M3_WAIT_TO_LEFT, 40*1000,NORMAL_SPEED);
	
	RestSelectMotorOrginSelect(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 60*10000,NORMAL_SPEED);
	
#endif
}

void March_Transporter_Belt(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	
	if(Light_Sensor_Get(M4_LIGHT)==0)
			Motor_Move_And_Wait_Select(M4_BLANK_NEXT, M4_NEXT_TO_BLANK, 8000,NORMAL_SPEED);
	
	if(Light_Sensor_Get(M3_LIGHT)==0)
			Motor_Move_And_Wait_Select(M3_LEFT_WAIT, M3_LEFT_TO_WAIT, 8000,NORMAL_SPEED);
	
	if(Light_Sensor_Get(M1_LIGHT)==0)
		Motor_Move_And_Wait_Select(M1_MIX_V, M1_MIX_V_DOWN, 40000,NORMAL_SPEED);
	


#endif	
}	
void Rest_high_wheel(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD
	
	RestSelectMotorOrginSelect(M7_HIGH_TURN,M7_LIGHT,M7_BACK_TURN, 8000,NORMAL_SPEED);
#endif
}
void March_high_wheel(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	if(Light_Sensor_Get(M7_LIGHT)==0)
		  Motor_Move_And_Wait_Select(M7_HIGH_TURN, M7_FRONT_TURN, 2400,NORMAL_SPEED);
#endif
}	


//F5 //F6
void RestFarAndDownMotorOrgin(void)
{
#if USE_CLEANING_DILUTION_BOARD	
	if(!Light_Sensor_Get(M11_LIGHT)&&!Light_Sensor_Get(M10_LIGHT))
	{
			Motor_Move_And_Wait_Select(M11_FAR_NEAR, M11_FAR, 3000,NORMAL_SPEED);
			RestSelectMotorOrginSelect(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 80*10000,NORMAL_SPEED);
		
			Motor_Move_And_Wait_Select(M11_FAR_NEAR, M11_FAR, 640,NORMAL_SPEED);
			Motor_Move_And_Wait_Select(M10_UP_DOWM, M10_DOWM, 40000,NORMAL_SPEED);
			RestSelectMotorOrginSelect(M10_UP_DOWM,M10_LIGHT,M10_UP, 80*10000,NORMAL_SPEED);
			RestSelectMotorOrginSelect(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 80*10000,NORMAL_SPEED);
	
	}else	if(!Light_Sensor_Get(M10_LIGHT)&&Light_Sensor_Get(M11_LIGHT))
	{
			RestSelectMotorOrginSelect(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 80*10000,NORMAL_SPEED);
		
		  Motor_Move_And_Wait_Select(M11_FAR_NEAR, M11_FAR, 640,NORMAL_SPEED);
			Motor_Move_And_Wait_Select(M10_UP_DOWM, M10_DOWM, 40000,NORMAL_SPEED);
			RestSelectMotorOrginSelect(M10_UP_DOWM,M10_LIGHT,M10_UP, 80*10000,NORMAL_SPEED);
		
			RestSelectMotorOrginSelect(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 80*10000,NORMAL_SPEED);
		
	}else	if(Light_Sensor_Get(M10_LIGHT)&&!Light_Sensor_Get(M11_LIGHT))
	{
			RestSelectMotorOrginSelect(M10_UP_DOWM,M10_LIGHT,M10_UP, 80*10000,NORMAL_SPEED);
			Motor_Move_And_Wait_Select(M11_FAR_NEAR, M11_FAR, 640,NORMAL_SPEED);
			RestSelectMotorOrginSelect(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 80*10000,NORMAL_SPEED);
	}else	if(Light_Sensor_Get(M10_LIGHT)&&Light_Sensor_Get(M11_LIGHT))
	{
		  RestSelectMotorOrginSelect(M10_UP_DOWM,M10_LIGHT,M10_UP, 80*10000,NORMAL_SPEED);
			RestSelectMotorOrginSelect(M11_FAR_NEAR,M11_LIGHT,M11_NEAR, 80*10000,NORMAL_SPEED);
	}
#endif	
}	

void First_Open_Motor_AutoCheck_Sensor(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD
	
	March_Transporter_Belt();
	Rest_Transporter_Belt();
	
	March_high_wheel();
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



void Scan_Motor_Slow_Spin(void)  //扫码
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	__BSP_MotorControl_Move__(M2_MIX, M2_MIX_LEFT, 320000,LOW_SPEED);
#endif	
}

void Mix_Blood_High_Speed(void) //混匀
{
#if USE_AUTOMATIC_INJECTION_BOARD			
	int i=0;
	Choose_Single_Motor_Speed_Config(M2_MIX,NORMAL_SPEED);
	PowerStep_Select_Motor_Baby(M2_MIX);	
	
	for(i=0;i<4;i++)
	{
		BSP_MotorControl_Move(0, M2_MIX_RIGHT, 60000); //以前是12000
		BSP_MotorControl_WaitWhileActive(0);
		BSP_MotorControl_Move(0, M2_MIX_LEFT, 60000);
		BSP_MotorControl_WaitWhileActive(0);
	}
	__BSP_MotorControl_HardStop__(M2_MIX);
	RestSelectMotorOrginSelect(M1_MIX_V,M1_LIGHT,M1_MIX_V_UP, 60*10000,NORMAL_SPEED);
#endif	
}	
void Mix_Work_Goto_Postion(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD		
	RestSelectMotorOrginSelect(M1_MIX_V,M1_LIGHT_WORK,M1_MIX_V_DOWN, 600*1000,NORMAL_SPEED);
#endif
}



//有->无-有
void Normal_Pitch_Move_Next_The_Last_Two(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD		
	Motor_Move_And_Wait_Select(M4_BLANK_NEXT, M4_NEXT_TO_BLANK,3200,NORMAL_SPEED);
	RestSelectMotorOrginSelect(M4_BLANK_NEXT,NORMAL_CHECK_DRAIN_LIGHT,M4_NEXT_TO_BLANK, 20000,NORMAL_SPEED);
#endif
}



//有->无-有
void Normal_Pitch_Move_Next(void)
{

#if USE_AUTOMATIC_INJECTION_BOARD		
	Motor_Move_And_Wait_Select(M4_BLANK_NEXT, M4_NEXT_TO_BLANK,4800,NORMAL_SPEED);
	RestSelectMotorOrginSelect(M4_BLANK_NEXT,NORMAL_NEXT_LIGHT,M4_NEXT_TO_BLANK, 32000,NORMAL_SPEED);
#endif
}

void Normal_Goto_First_Position(void)
{

#if USE_AUTOMATIC_INJECTION_BOARD			
	RestSelectMotorOrginSelect(M4_BLANK_NEXT,NORMAL_NEXT_LIGHT,M4_NEXT_TO_BLANK, 32000,NORMAL_SPEED);
#endif	
}

void Normal_Move_Blank(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD			
		RestSelectMotorOrginSelect(M4_BLANK_NEXT,BLANK_LIGHT,M4_NEXT_TO_BLANK, 32000,LOW_SPEED);
#endif	
}	

/***********************************************************/
//time ms

void  Rest_Injection_Module_Motor(uint32_t up_Steps,uint32_t big_Steps,int time)
{
#if USE_CLEANING_DILUTION_BOARD		
			int i=0;
			int flag1=0,flag2=0;


			__BSP_MotorControl_Move__(M8_BIG_IN_OUT, M8_BIG_OUT, big_Steps,NORMAL_SPEED);
		
			delay_ms(time);

			__BSP_MotorControl_Move__(M10_UP_DOWM, M10_UP, up_Steps,NORMAL_SPEED);
			
		while(1){
				i++;
				if(!Light_Sensor_Get(M10_LIGHT)&&!flag1)
				{		
						__BSP_MotorControl_HardStop__(M8_BIG_IN_OUT);				
						flag1=1;
				}
				if(!Light_Sensor_Get(M10_LIGHT)&&!flag2)
				{		__BSP_MotorControl_HardStop__(M10_UP_DOWM);			
						flag2=1;					
						
				}	
				if(flag1&&flag2) break;
				
				if(i>=1500){
					
							__BSP_MotorControl_HardStop__(M8_BIG_IN_OUT);
					
							__BSP_MotorControl_HardStop__(M10_UP_DOWM);
							break;	
				}
				

				delay_ms(10);	
		}
#endif			
}	


//dischard
void mix_and_reach_position(void)	
{

}	

uint8_t C55_C52_connect_check(void)
{
	uint8_t ret=0;
	
	if(C52_connect_check())ret|=0x10;
	if(C55_connect_check())ret|=0x01;
	
	LOGD("C55 C52 0x%x \r\n",ret);
	
	return ret;
}

#define NUM2STR(x) case x: return #x
static char* _num2string_(uint8_t num)
{

    switch ( num )
    {
        NUM2STR(SLEF_TEST);
        NUM2STR(FIRST_START_CHECK_SENSOR_BOARD);
        NUM2STR(REST_SENSOR_BOARD_MOTOR);
        NUM2STR(FIRST_START_CHECK_MOTOR_BOARD);
        NUM2STR(REST_MOTOR_BOARD_MOTOR);
        NUM2STR(REST_C55_C52);
        NUM2STR(REST_DRAIN_AND_WASH_MOTOR);
        NUM2STR(REST_SAMPLE_MOTOR);
        NUM2STR(CLEAR_BLANK);
        NUM2STR(CLEARL_WAIT);
        NUM2STR(READY_NEXT);
        NUM2STR(READY_LEFT);
        NUM2STR(LEFT_MOVE_TO_WAIT);
        NUM2STR(REST_HIGH_WHEEL);
        NUM2STR(REST_TRANSPORTER_BELT);
        NUM2STR(REACH_DEGREE_WAIT);
        NUM2STR(MIX_SCAN_SLOW);
        NUM2STR(MIX_BLOOD_HIGH);

        NUM2STR(BELT_MOVE_SAMETIME);
        NUM2STR(MIX_WORK_GOTO);
        NUM2STR(NORMAL_PITCH_MOVE_NEXT);
        NUM2STR(NORMAL_BLANK_REST);
        NUM2STR(NORMAL_FIRST_POSITON);
        NUM2STR(MIX_AND_REACH_POSITION);
        NUM2STR(BIG_IN_OUT_SLOW);
        NUM2STR(BIG_IN_OUT_NORMAL);
        NUM2STR(NORMAL_PITCH_MOVE_NEXT_THE_LAST_TWO);
        NUM2STR(C55_C52_CONNECT);


        NUM2STR(MOTOR_UP_DOWN_SLOW);
        NUM2STR(MOTOR_UP_DOWN_NORMAL);
        NUM2STR(LITTLE_IN_OUT_SLOW);
        NUM2STR(LITTLE_IN_OUT_NORMAL);
        NUM2STR(BLANK_NEXT_MOTOR_HIGH);
        NUM2STR(BLANK_NEXT_MOTOR_NORMAL);
    default:
        return "UNKNOW COMMAND";
    }
}			
			

uint8_t process_motor_command_receive(Command_Package_t command)
{
		uint8_t value=0;
	
	LOGD("type command:%s\r\n", _num2string_(command));
	
		switch(command)
		{
			case SLEF_TEST:	
					LOGE("Nor command !\r\n");
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
			case C55_C52_CONNECT:
					value = C55_C52_connect_check();
				break;
			case MOTOR_UP_DOWN_SLOW:
#if USE_CLEANING_DILUTION_BOARD
					Choose_Single_Motor_Speed_Config(M10_UP_DOWM,LOW_SPEED);
#endif					
				break;
			case MOTOR_UP_DOWN_NORMAL:
#if USE_CLEANING_DILUTION_BOARD						
					Choose_Single_Motor_Speed_Config(M10_UP_DOWM,NORMAL_SPEED);
#endif					
					break;	
			case LITTLE_IN_OUT_SLOW:
					Choose_Single_Motor_Speed_Config(M9_IN_OUT,LOW_SPEED);
				break;
			case	LITTLE_IN_OUT_NORMAL:
					Choose_Single_Motor_Speed_Config(M9_IN_OUT,NORMAL_SPEED);
					
					break;	
			case BLANK_NEXT_MOTOR_HIGH:
				Choose_Single_Motor_Speed_Config(M4_BLANK_NEXT,HIGH_SPEED);
				break;
		case	BLANK_NEXT_MOTOR_NORMAL:
				Choose_Single_Motor_Speed_Config(M4_BLANK_NEXT,NORMAL_SPEED);
			
				break;		
			
			default:
					break;
		}	
	return value;
}


