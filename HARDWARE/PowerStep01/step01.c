#include "step01.h"
#include <string.h>
#include "delay.h"
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


int init_motor_device(init_motor_speed_tension_type_t data)
{	
		int ret=0;
	
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



int ConfigMotorAllDevice(int chip, MOTOR_SPEED_type_t speed_type)
{
	int ret=0;
	switch(chip)
	{
		case 1:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=700;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=700;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=120;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=600;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=800;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
		
   		break;
		
		case 2:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=250;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=25;
   		break;
		case 3:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=250;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=25;
   		break;
		case 4:
			
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=120;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;

/*		
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=582;
			TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=250;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=20;		
*/	
			break;
		case 5:
			TempMotor.request.devices=chip;
		/*
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=500;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
		*/
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=2000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=2000;
		
			if(speed_type==LOW_SPEED){
				TempMotor.request.init_motor.motor_commonSpeed.acceleration=580;
				TempMotor.request.init_motor.motor_commonSpeed.deceleration=580;
				TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=250;
			}else if(speed_type==NORMAL_SPEED)
				TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
		
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		
			TempMotor.request.init_motor.motor_config.voltage.duty_cycle=30;	
   		break;
		case 6:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
   		break;	
		case 7:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
   		break;			
		case 8:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=50;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
   		break;		
	case 9:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
   		break;		
	case 10:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
   		break;		
		case 11:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=1000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=1000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=120;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
   		break;
	 case 12:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
		
   		break;			
		case 13:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
		
   		break;
		case 14:
			TempMotor.request.devices=chip;
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.motor_commonSpeed.acceleration=3000;
			TempMotor.request.init_motor.motor_commonSpeed.deceleration=3000;
		  TempMotor.request.init_motor.motor_commonSpeed.minSpeed=0;
		  TempMotor.request.init_motor.motor_config.current.current_value=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.motor_commonSpeed.maxSpeed=2000;
		
		
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

//motor 编号
//light 编号
//复位时步伐
//复位时的电机方向
//是否先wait steps
void RestSelectMotorOrgin(int motorNum,int lightNum, motorDir_t motorDir,uint32_t steps)
{
	int status=0;
	int i=0;
	int count_delay=0;
	
		if(steps<5000)count_delay=10;
		else if(steps<15000)count_delay=20;
		else if(steps<25000)count_delay=30;
		else if(steps<35000)count_delay=35;
		else if(steps<45000)count_delay=40;
		else if(steps<55000)count_delay=45;
		else count_delay=50;
		
		
		PowerStep_Select_Motor_Baby(motorNum);
		BSP_MotorControl_Move(0, motorDir, (int)(steps*1.2));
		while(1){
				i++;
				if(!Light_Sensor_Get(lightNum))
					{						
							BSP_MotorControl_HardStop(0);	
							break;
				}else if(i>=count_delay*10){
							BSP_MotorControl_HardStop(0);
							break;	
				}
				delay_ms(10);	
		}				
}


/*
*HAL 层用于设置电机参数的接口
*/
void Set_Single_Motor_Config(init_motor_speed_tension_type_t data)
{

	init_motor_device(data);//把电机参数保存在数组里
	PowerStep_Select_Motor_Baby(data.request.devices);
	Powerstep01_Init_Register(&motor_config_array[data.request.devices]);
	printf("devices:%d\r\n",data.request.devices);
	printf("mode:%d\r\n",data.request.init_motor.ModeSelection);
	
	printf("MAXspeed:%f\r\n", data.request.init_motor.motor_commonSpeed.maxSpeed);
	printf("MINspeed:%f\r\n", data.request.init_motor.motor_commonSpeed.minSpeed);
	printf("ACCspeed:%f\r\n", data.request.init_motor.motor_commonSpeed.acceleration);
	printf("DECspeed:%f\r\n", data.request.init_motor.motor_commonSpeed.deceleration);
}

/*
*单片机可调用的变速接口
*也可用于Hal 选择
*/
void Choose_Single_Motor_Speed_Config( int motor_chip, MOTOR_SPEED_type_t speed_type)
{
			ConfigMotorAllDevice(motor_chip,speed_type);
			init_motor_device(TempMotor);
			PowerStep_Select_Motor_Baby(motor_chip);
			Powerstep01_Init_Register(&motor_config_array[motor_chip]);
}	


//初始化
void BSP_Motor_Control_Init(void)
{
int i,result,value;
	BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, 1);
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);//此处NULL只能是NULL，无需传参数
	
	
		
	for(i=1;i<SIZE_MOTOR_ARRAY;i++){
			result=ConfigMotorAllDevice(i,NORMAL_SPEED);//配置电机参数
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

}	



