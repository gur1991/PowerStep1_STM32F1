#include "step01.h"
#include <string.h>

powerstep01_Init_u_t motor_config_array[SIZE_MOTOR_ARRAY];
init_motor_speed_tension_type_t TempMotor;


//#ifdef CURRENT_MODE
/* Initialization parameters for current mode */
union powerstep01_Init_u init_current =
{
  .cm.cp.cmVmSelection = POWERSTEP01_CM_VM_CURRENT, // enum powerstep01_CmVm_t
  582, // Acceleration rate in step/s2, range 14.55 to 59590 steps/s^2
  582, // Deceleration rate in step/s2, range 14.55 to 59590 steps/s^2
  488, // Maximum speed in step/s, range 15.25 to 15610 steps/s
  0, // Minimum speed in step/s, range 0 to 976.3 steps/s
  POWERSTEP01_LSPD_OPT_OFF, // Low speed optimization bit, enum powerstep01_LspdOpt_t
  244.16, // Full step speed in step/s, range 7.63 to 15625 steps/s
  POWERSTEP01_BOOST_MODE_OFF, // Boost of the amplitude square wave, enum powerstep01_BoostMode_t
  281.25, // Overcurrent threshold settings via enum powerstep01_OcdTh_t
  STEP_MODE_1_16, // Step mode settings via enum motorStepMode_t
  POWERSTEP01_SYNC_SEL_DISABLED, // Synch. Mode settings via enum powerstep01_SyncSel_t
  (POWERSTEP01_ALARM_EN_OVERCURRENT|
   POWERSTEP01_ALARM_EN_THERMAL_SHUTDOWN|
   POWERSTEP01_ALARM_EN_THERMAL_WARNING|
   POWERSTEP01_ALARM_EN_UVLO|
   POWERSTEP01_ALARM_EN_STALL_DETECTION|
   POWERSTEP01_ALARM_EN_SW_TURN_ON|
   POWERSTEP01_ALARM_EN_WRONG_NPERF_CMD), // Alarm settings via bitmap enum powerstep01_AlarmEn_t
  POWERSTEP01_IGATE_64mA, // Gate sink/source current via enum powerstep01_Igate_t 
  POWERSTEP01_TBOOST_0ns, // Duration of the overboost phase during gate turn-off via enum powerstep01_Tboost_t
  POWERSTEP01_TCC_500ns, // Controlled current time via enum powerstep01_Tcc_t
  POWERSTEP01_WD_EN_DISABLE, // External clock watchdog, enum powerstep01_WdEn_t  
  POWERSTEP01_TBLANK_375ns, // Duration of the blanking time via enum powerstep01_TBlank_t
  POWERSTEP01_TDT_125ns, // Duration of the dead time via enum powerstep01_Tdt_t
  /* current mode parameters */
  328.12, // Hold torque in mV, range from 7.8mV to 1000 mV
  328.12, // Running torque in mV, range from 7.8mV to 1000 mV 
  328.12, // Acceleration torque in mV, range from 7.8mV to 1000 mV
  328.12, // Deceleration torque in mV, range from 7.8mV to 1000 mV
  POWERSTEP01_TOFF_FAST_8us, //Maximum fast decay time , enum powerstep01_ToffFast_t 
  POWERSTEP01_FAST_STEP_12us, //Maximum fall step time , enum powerstep01_FastStep_t 
  3.0, // Minimum on-time in us, range 0.5us to 64us
  21.0, // Minimum off-time in us, range 0.5us to 64us 
  POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_2MHZ, // Clock setting , enum powerstep01_ConfigOscMgmt_t
  POWERSTEP01_CONFIG_SW_HARD_STOP, // External switch hard stop interrupt mode, enum powerstep01_ConfigSwMode_t
  POWERSTEP01_CONFIG_TQ_REG_TVAL_USED, // External torque regulation enabling , enum powerstep01_ConfigEnTqReg_t
  POWERSTEP01_CONFIG_VS_COMP_DISABLE, // Motor Supply Voltage Compensation enabling , enum powerstep01_ConfigEnVscomp_t 
  POWERSTEP01_CONFIG_OC_SD_DISABLE, // Over current shutwdown enabling, enum powerstep01_ConfigOcSd_t
  POWERSTEP01_CONFIG_UVLOVAL_LOW, // UVLO Threshold via powerstep01_ConfigUvLoVal_t
  POWERSTEP01_CONFIG_VCCVAL_15V, // VCC Val, enum powerstep01_ConfigVccVal_t
  POWERSTEP01_CONFIG_TSW_048us, // Switching period, enum powerstep01_ConfigTsw_t
  POWERSTEP01_CONFIG_PRED_DISABLE, // Predictive current enabling , enum powerstep01_ConfigPredEn_t 
};
//#endif //CURRENT_MODE

//#ifdef VOLTAGE_MODE
/* Initialization parameters for voltage mode */

union powerstep01_Init_u init_voltage =
{
  /* common parameters */
  .vm.cp.cmVmSelection = POWERSTEP01_CM_VM_VOLTAGE, // enum powerstep01_CmVm_t
  582, // Acceleration rate in step/s2, range 14.55 to 59590 steps/s^2
  582, // Deceleration rate in step/s2, range 14.55 to 59590 steps/s^2
  488, // Maximum speed in step/s, range 15.25 to 15610 steps/s
  0, // Minimum speed in step/s, range 0 to 976.3 steps/s
  POWERSTEP01_LSPD_OPT_OFF, // Low speed optimization bit, enum powerstep01_LspdOpt_t
  244.16, // Full step speed in step/s, range 7.63 to 15625 steps/s
  POWERSTEP01_BOOST_MODE_OFF, // Boost of the amplitude square wave, enum powerstep01_BoostMode_t
  281.25, // Overcurrent threshold settings via enum powerstep01_OcdTh_t
 // STEP_MODE_1_16, // Step mode settings via enum motorStepMode_t
  STEP_MODE_1_16,
	POWERSTEP01_SYNC_SEL_DISABLED, // Synch. Mode settings via enum powerstep01_SyncSel_t
  (POWERSTEP01_ALARM_EN_OVERCURRENT|
   POWERSTEP01_ALARM_EN_THERMAL_SHUTDOWN|
   POWERSTEP01_ALARM_EN_THERMAL_WARNING|
   POWERSTEP01_ALARM_EN_UVLO|
   POWERSTEP01_ALARM_EN_STALL_DETECTION|
   POWERSTEP01_ALARM_EN_SW_TURN_ON|
   POWERSTEP01_ALARM_EN_WRONG_NPERF_CMD), // Alarm settings via bitmap enum powerstep01_AlarmEn_t
  POWERSTEP01_IGATE_64mA, // Gate sink/source current via enum powerstep01_Igate_t 
  POWERSTEP01_TBOOST_0ns, // Duration of the overboost phase during gate turn-off via enum powerstep01_Tboost_t
  POWERSTEP01_TCC_500ns, // Controlled current time via enum powerstep01_Tcc_t
  POWERSTEP01_WD_EN_DISABLE, // External clock watchdog, enum powerstep01_WdEn_t  
  POWERSTEP01_TBLANK_375ns, // Duration of the blanking time via enum powerstep01_TBlank_t
  POWERSTEP01_TDT_125ns, // Duration of the dead time via enum powerstep01_Tdt_t
  /* voltage mode parameters */
  16.02, // Hold duty cycle (torque) in %, range 0 to 99.6%
  16.02, // Run duty cycle (torque) in %, range 0 to 99.6%
  16.02, // Acceleration duty cycle (torque) in %, range 0 to 99.6%
  16.02, // Deceleration duty cycle (torque) in %, range 0 to 99.6%
  61.512, // Intersect speed settings for BEMF compensation in steps/s, range 0 to 3906 steps/s
  0.03815, // BEMF start slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
  0.06256, // BEMF final acc slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
  0.06256, // BEMF final dec slope settings for BEMF compensation in % step/s, range 0 to 0.4% s/step
  1, // Thermal compensation param, range 1 to 1.46875
  531.25, // Stall threshold settings in mV, range 31.25mV to 1000mV
  POWERSTEP01_CONFIG_INT_16MHZ_OSCOUT_2MHZ, // Clock setting , enum powerstep01_ConfigOscMgmt_t
  POWERSTEP01_CONFIG_SW_HARD_STOP, // External switch hard stop interrupt mode, enum powerstep01_ConfigSwMode_t
  POWERSTEP01_CONFIG_VS_COMP_DISABLE, // Motor Supply Voltage Compensation enabling , enum powerstep01_ConfigEnVscomp_t
  POWERSTEP01_CONFIG_OC_SD_DISABLE, // Over current shutwdown enabling, enum powerstep01_ConfigOcSd_t
  POWERSTEP01_CONFIG_UVLOVAL_LOW, // UVLO Threshold via powerstep01_ConfigUvLoVal_t
  POWERSTEP01_CONFIG_VCCVAL_15V, // VCC Val, enum powerstep01_ConfigVccVal_t
  POWERSTEP01_CONFIG_PWM_DIV_2, // PWM Frequency Integer division, enum powerstep01_ConfigFPwmInt_t
  POWERSTEP01_CONFIG_PWM_MUL_1, // PWM Frequency Integer Multiplier, enum powerstep01_ConfigFPwmDec_t
};
//#endif

/*********************************interface motor*******************************************************/

//每执行一次命令都要重新初始化?
//不用，把參數保存，指針調用即可
//flag busy 要处理在此
int init_motor_bsp(int device)
{
  /* Attach the function MyFlagInterruptHandler (defined below) to the flag interrupt */
  //BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);

  /* Attach the function MyBusyInterruptHandler (defined below) to the busy interrupt */
  //BSP_MotorControl_AttachBusyInterrupt(MyBusyInterruptHandler);
  
  /* Attach the function Error_Handler (defined below) to the error Handler*/
  //BSP_MotorControl_AttachErrorHandler(MyErrorHandler);
	
	return 0;
}


int init_motor_device(init_motor_speed_tension_type_t data)
{	
		int ret=0;
		bool isok;
	
		if(data.request.init_motor.ModeSelection==POWERSTEP01_CM_VM_CURRENT)
		{
				memcpy(&motor_config_array[data.request.devices], &init_current,sizeof(init_current));
				
				motor_config_array[data.request.devices].cm.cp.acceleration=data.request.init_motor.motor_commonSpeed.acceleration;
				motor_config_array[data.request.devices].cm.cp.deceleration=data.request.init_motor.motor_commonSpeed.deceleration;
				motor_config_array[data.request.devices].cm.cp.minSpeed=data.request.init_motor.motor_commonSpeed.minSpeed;
				motor_config_array[data.request.devices].cm.cp.maxSpeed=data.request.init_motor.motor_commonSpeed.maxSpeed;//速度
			
				motor_config_array[data.request.devices].cm.tvalAcc=data.request.init_motor.motor_config.current.current_value;
				motor_config_array[data.request.devices].cm.tvalDec=data.request.init_motor.motor_config.current.current_value;
				motor_config_array[data.request.devices].cm.tvalHold=data.request.init_motor.motor_config.current.current_value;
				motor_config_array[data.request.devices].cm.tvalRun=data.request.init_motor.motor_config.current.current_value;//拉力
				
		
		}else if(data.request.init_motor.ModeSelection==POWERSTEP01_CM_VM_VOLTAGE)
		{
				memcpy(&motor_config_array[data.request.devices], &init_voltage,sizeof(init_voltage));
				
				motor_config_array[data.request.devices].vm.cp.acceleration=data.request.init_motor.motor_commonSpeed.acceleration;
				motor_config_array[data.request.devices].vm.cp.deceleration=data.request.init_motor.motor_commonSpeed.deceleration;
				motor_config_array[data.request.devices].vm.cp.minSpeed=data.request.init_motor.motor_commonSpeed.minSpeed;
				motor_config_array[data.request.devices].vm.cp.maxSpeed=data.request.init_motor.motor_commonSpeed.maxSpeed;
			
				motor_config_array[data.request.devices].vm.kvalAcc=data.request.init_motor.motor_config.voltage.duty_cycle;
				motor_config_array[data.request.devices].vm.kvalDec=data.request.init_motor.motor_config.voltage.duty_cycle;
				motor_config_array[data.request.devices].vm.kvalHold=data.request.init_motor.motor_config.voltage.duty_cycle;
				motor_config_array[data.request.devices].vm.kvalRun=data.request.init_motor.motor_config.voltage.duty_cycle;
			
		}else {
				return -1;
		}	

		return ret;
}



int ConfigMotorAllDevice(int chip)
{
	int ret=0;
	switch(chip)
	{
		case 1:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=60;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=16;
		
   		break;
		case 2:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=100;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=16;
		
   		break;
		case 3:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=100;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=16;
		
   		break;
		case 4:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=60;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=16;
		
   		break;
		case 5:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=120;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			//TempMotor.request.init_motor.motor_config.voltage.duty_cycle=20;
		  TempMotor.request.init_motor.motor_config.current.current_value=150;
   		break;
		case 6:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			//TempMotor.request.init_motor.motor_config.voltage.duty_cycle=25;
			TempMotor.request.init_motor.motor_config.current.current_value=150;
		
   		break;	
		case 7:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			//TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
			TempMotor.request.init_motor.motor_config.current.current_value=150;
		
   		break;			
		case 8:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			//TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		  TempMotor.request.init_motor.motor_config.current.current_value=150;
		
   		break;		
	case 9:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=30000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=30000;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=15000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			//TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		  TempMotor.request.init_motor.motor_config.current.current_value=50;
		
   		break;		
	case 10:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			//TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		  TempMotor.request.init_motor.motor_config.current.current_value=150;
		
   		break;		
		case 13:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=100;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=10;
		
   		break;
		case 14:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=30;
		
   		break;
		default:
			ret=-1;
	}	
	return ret;
}


void StopALLMotorMotion(void)
{	
	int i;
	for(i=1;i<SIZE_MOTOR_ARRAY;i++){
			PowerStep_Select_Motor_Baby(i);
			BSP_MotorControl_HardStop(0);
	}		
}


//此函数调用后会按照设定的速度和step走
//完成后，速度会恢复之前的
void ChangeSpeedMotorRun(int motorNum ,uint32_t steps ,uint32_t setSpeed, motorDir_t motorDir)
{
	uint32_t myMaxSpeed;
	int status=0;

	PowerStep_Select_Motor_Baby(motorNum);		
	myMaxSpeed= BSP_MotorControl_CmdGetParam(0, POWERSTEP01_MAX_SPEED);
	BSP_MotorControl_CmdSetParam(0, POWERSTEP01_MAX_SPEED, setSpeed);
	BSP_MotorControl_Move(0, motorDir, steps);
	BSP_MotorControl_WaitWhileActive(0);		
	BSP_MotorControl_CmdSetParam(0, POWERSTEP01_MAX_SPEED, myMaxSpeed);
}


//motor 编号
//light 编号
//复位时用的速度
//复位时的电机方向

void RestSelectMotorPosition(int motorNum,int lightNum,uint32_t rstSpeed, motorDir_t motorDir)
{
	uint32_t myMaxSpeed;
	int status=0;
	
		if(Light_Sensor_Get(lightNum)){
					PowerStep_Select_Motor_Baby(motorNum);
			
					myMaxSpeed= BSP_MotorControl_CmdGetParam(0, POWERSTEP01_MAX_SPEED);
					BSP_MotorControl_CmdSetParam(0, POWERSTEP01_MAX_SPEED, rstSpeed);
			
					BSP_MotorControl_Move(0, motorDir, 600000);
					status=1;
		}
		while(status){
				if(!Light_Sensor_Get(lightNum)){
							PowerStep_Select_Motor_Baby(motorNum);
							BSP_MotorControl_HardStop(0);
							BSP_MotorControl_CmdSetParam(0, POWERSTEP01_MAX_SPEED, myMaxSpeed);
							status=0;
				}
		}				
}

void RestAllMotorPosition2(void)
{
	StopALLMotorMotion();
	RestSelectMotorPosition(4,3,150,BACKWARD);
	RestSelectMotorPosition(1,6,150,BACKWARD);
}	


//开机自检
void FirstOpenMotorCheckPosition(void)
{
		//第一次复位
		//推履带
		RestAllMotorPosition2();
		PowerStep_Select_Motor_Baby(2);
		BSP_MotorControl_Move(0, BACKWARD, 5000);
		//BSP_MotorControl_WaitWhileActive(0);
		PowerStep_Select_Motor_Baby(3);
		BSP_MotorControl_Move(0, BACKWARD, 5000);
		BSP_MotorControl_WaitWhileActive(0);
	
		//第二次复位
		//拉杆
		RestAllMotorPosition2();
		PowerStep_Select_Motor_Baby(1);
		BSP_MotorControl_Move(0, FORWARD, 20700);
		//BSP_MotorControl_WaitWhileActive(0);
		PowerStep_Select_Motor_Baby(4);
		BSP_MotorControl_Move(0, FORWARD, 20700);
		BSP_MotorControl_WaitWhileActive(0);

		//第三次复位
		RestAllMotorPosition2();
}


void RestAllMotorPosition(void)
{	
		int status1=0,status2=0;
	
		
		StopALLMotorMotion();
		if(Light_Sensor_Get(6)){
					PowerStep_Select_Motor_Baby(1);
					BSP_MotorControl_Move(0, BACKWARD, 600000);
					status1=1;

		}
		if(Light_Sensor_Get(3)){
					PowerStep_Select_Motor_Baby(4);
					BSP_MotorControl_Move(0, BACKWARD, 600000);
				  status2=1;
		}
		
		while(status2||status1){
					if(!Light_Sensor_Get(6)){
							PowerStep_Select_Motor_Baby(1);
							BSP_MotorControl_HardStop(0);
							status1=0;
					}	
					if(!Light_Sensor_Get(3)){
							PowerStep_Select_Motor_Baby(4);
							BSP_MotorControl_HardStop(0);
							status2=0;
					}	
		}
		
		StopALLMotorMotion();
		
}

//************************below no-useful************************
//array==0xff 代表为无效
//只下发指令给powerstep 不去等待执行结束
void move_many_motor(move_many_motor_type_t data)
{
		int i;
		for(i=0;i<ALLOW_MANY_MOTOR;i++)
		{		
				//如果无效，则检测下一个
				if(data.request.move_motor_data[i].array==0xff)continue;
		//		BSP_MotorControl_Select_Handle(data.request.move_motor_data[i].array);	
				BSP_MotorControl_Move(0,data.request.move_motor_data[i].direction,data.request.move_motor_data[i].stepCount);
		}
}

//此函数直接调用即可 select 内部会识别当前device
//BSP_MotorControl_WaitWhileActive(0);

void wait_many_motor(wait_many_motor_type_t data)
{
		int i;
		for(i=0;i<ALLOW_MANY_MOTOR;i++)
		{		
				//如果无效，则检测下一个
				if(data.request.wait_motor_data[i].array==0xff)continue;
	//			BSP_MotorControl_Select_Handle(data.request.wait_motor_data[i].array);
				BSP_MotorControl_WaitWhileActive(0);
		}
}
