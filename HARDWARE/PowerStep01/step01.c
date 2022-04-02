#include "step01.h"
#include <string.h>
#include "delay.h"
#include "config.h"
#include "factory_many.h"
#include "motor.h"

static powerstep01_Init_u_t MotorConfig;
static init_motor_speed_tension_type_t TempMotor;
static mix_motor_config_type_t sMix;


void init_mix_motor_config(void)
{
		sMix.max_slow=300;
		sMix.acc_slow=300;
		sMix.dec_slow=300;
		
		sMix.max_normal=2000;
		sMix.acc_normal=15000;
		sMix.dec_normal=15000;
}

void set_mix_motor_config(mix_motor_config_type_t p)
{
		memcpy(&sMix, &p, sizeof(sMix));
}





//#ifdef CURRENT_MODE
/* Initialization parameters for current mode */
static powerstep01_Init_u_t init_current =
{
  .cm.cp.cmVmSelection = POWERSTEP01_CM_VM_CURRENT, // enum powerstep01_CmVm_t
  582, // Acceleration rate in step/s2, range 14.55 to 59590 steps/s^2
  582, // Deceleration rate in step/s2, range 14.55 to 59590 steps/s^2
  488, // Maximum speed in step/s, range 15.25 to 15610 steps/s
  0, // Minimum speed in step/s, range 0 to 976.3 steps/s
  POWERSTEP01_LSPD_OPT_OFF, // Low speed optimization bit, enum powerstep01_LspdOpt_t
  2500,//244.16, // Full step speed in step/s, range 7.63 to 15625 steps/s
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

static powerstep01_Init_u_t init_voltage =
{
  /* common parameters */
  .vm.cp.cmVmSelection = POWERSTEP01_CM_VM_VOLTAGE, // enum powerstep01_CmVm_t
  582, // Acceleration rate in step/s2, range 14.55 to 59590 steps/s^2
  582, // Deceleration rate in step/s2, range 14.55 to 59590 steps/s^2
  488, // Maximum speed in step/s, range 15.25 to 15610 steps/s
  0, // Minimum speed in step/s, range 0 to 976.3 steps/s
  POWERSTEP01_LSPD_OPT_OFF, // Low speed optimization bit, enum powerstep01_LspdOpt_t
  2500,//244.16, // Full step speed in step/s, range 7.63 to 15625 steps/s
  POWERSTEP01_BOOST_MODE_OFF, // Boost of the amplitude square wave, enum powerstep01_BoostMode_t
  281.25, // Overcurrent threshold settings via enum powerstep01_OcdTh_t
 // STEP_MODE_1_16, // Step mode settings via enum motorStepMode_t
  STEP_MODE_1_64,
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
	
		memset(&MotorConfig,0,sizeof(MotorConfig));
	
	
		if(data.request.init_motor.ModeSelection==POWERSTEP01_CM_VM_CURRENT)
		{
				memcpy(&MotorConfig.cm, &init_current.cm,sizeof(init_current.cm));
				
				MotorConfig.cm.cp.acceleration=data.request.init_motor.Speed.acceleration;
				MotorConfig.cm.cp.deceleration=data.request.init_motor.Speed.deceleration;
				MotorConfig.cm.cp.minSpeed=data.request.init_motor.Speed.minSpeed;
				MotorConfig.cm.cp.maxSpeed=data.request.init_motor.Speed.maxSpeed;
			
				MotorConfig.cm.tvalAcc=data.request.init_motor.config.acc;
				MotorConfig.cm.tvalDec=data.request.init_motor.config.dec;
				MotorConfig.cm.tvalHold=data.request.init_motor.config.hold;
				MotorConfig.cm.tvalRun=data.request.init_motor.config.run;
				
		
		}else if(data.request.init_motor.ModeSelection==POWERSTEP01_CM_VM_VOLTAGE)
		{
				memcpy(&MotorConfig.vm, &init_voltage.vm,sizeof(init_voltage.vm));
				
				MotorConfig.vm.cp.acceleration=data.request.init_motor.Speed.acceleration;
				MotorConfig.vm.cp.deceleration=data.request.init_motor.Speed.deceleration;
				MotorConfig.vm.cp.minSpeed=data.request.init_motor.Speed.minSpeed;
				MotorConfig.vm.cp.maxSpeed=data.request.init_motor.Speed.maxSpeed;
			
				MotorConfig.vm.kvalAcc=data.request.init_motor.config.acc;
				MotorConfig.vm.kvalDec=data.request.init_motor.config.dec;
				MotorConfig.vm.kvalHold=data.request.init_motor.config.hold;
				MotorConfig.vm.kvalRun=data.request.init_motor.config.run;
			
		}else {
				return -1;
		}	

		return ret;
}


int ConfigMotorAllDevice(int chip, MOTOR_SPEED_type_t speed_type)
{
	int ret=0;
	
	memset(&TempMotor, 0, sizeof(TempMotor));
	
	
	TempMotor.request.devices=chip;
	TempMotor.request.init_motor.Speed.minSpeed=0;
	
	switch(chip)
	{
		case 1:

			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.Speed.acceleration=15000;
			TempMotor.request.init_motor.Speed.deceleration=15000;
		
		  TempMotor.request.init_motor.config.acc=100;
		  TempMotor.request.init_motor.config.hold=10;
		
		  TempMotor.request.init_motor.config.dec=100;
			TempMotor.request.init_motor.config.run=100;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=2000;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=6000;
				
   		break;			

		
		case 2:
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.Speed.acceleration=15000;
			TempMotor.request.init_motor.Speed.deceleration=15000;
		
		  TempMotor.request.init_motor.config.acc=100;
		  TempMotor.request.init_motor.config.hold=10;
		
		  TempMotor.request.init_motor.config.dec=100;
			TempMotor.request.init_motor.config.run=100;
			TempMotor.request.init_motor.Speed.maxSpeed=4000;
		
		
			if(speed_type==NORMAL_SPEED)
			{
				TempMotor.request.init_motor.Speed.acceleration=sMix.acc_normal;//15000;
				TempMotor.request.init_motor.Speed.deceleration=sMix.dec_normal;//15000;
		
				TempMotor.request.init_motor.config.acc=100;
				TempMotor.request.init_motor.config.hold=10;
		
				TempMotor.request.init_motor.config.dec=100;
				TempMotor.request.init_motor.config.run=100;
				TempMotor.request.init_motor.Speed.maxSpeed=sMix.max_normal;//2000;
				
			}
			else if (speed_type==LOW_SPEED)
			{
				TempMotor.request.init_motor.config.acc=100;
				TempMotor.request.init_motor.config.hold=10;
				TempMotor.request.init_motor.config.dec=100;
				TempMotor.request.init_motor.config.run=100;
				TempMotor.request.init_motor.Speed.acceleration=sMix.acc_slow;
				TempMotor.request.init_motor.Speed.deceleration=sMix.dec_slow;
				TempMotor.request.init_motor.Speed.maxSpeed=sMix.max_slow;
			}
				
   		break;	


		case 3:
			
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
		
			TempMotor.request.init_motor.config.acc=10;
		  TempMotor.request.init_motor.config.hold=1;
		
		  TempMotor.request.init_motor.config.dec=10;
			TempMotor.request.init_motor.config.run=10;
		
			TempMotor.request.init_motor.Speed.acceleration=5000;
			TempMotor.request.init_motor.Speed.deceleration=5000;
			TempMotor.request.init_motor.Speed.maxSpeed=0;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;

			break;

		case 4:
			
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
		
			TempMotor.request.init_motor.config.acc=20;
		  TempMotor.request.init_motor.config.hold=1;
		
		  TempMotor.request.init_motor.config.dec=20;
			TempMotor.request.init_motor.config.run=20;
		
			TempMotor.request.init_motor.Speed.acceleration=5000;
			TempMotor.request.init_motor.Speed.deceleration=5000;
			TempMotor.request.init_motor.Speed.maxSpeed=1000;
		
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=200;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;
   		break;
		

		case 5:
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.config.acc=20;
		  TempMotor.request.init_motor.config.hold=1;
		
		  TempMotor.request.init_motor.config.dec=20;
			TempMotor.request.init_motor.config.run=20;
		
			TempMotor.request.init_motor.Speed.acceleration=5000;
			TempMotor.request.init_motor.Speed.deceleration=5000;
			TempMotor.request.init_motor.Speed.maxSpeed=200;
	
   		break;

		case 6:
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
			TempMotor.request.init_motor.config.acc=20;
		  TempMotor.request.init_motor.config.hold=1;
		
		  TempMotor.request.init_motor.config.dec=20;
			TempMotor.request.init_motor.config.run=20;
		
			TempMotor.request.init_motor.Speed.acceleration=5000;
			TempMotor.request.init_motor.Speed.deceleration=5000;
			TempMotor.request.init_motor.Speed.maxSpeed=200;
		
   		break;
	
		case 7:
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		
		
			TempMotor.request.init_motor.config.acc=20;
		  TempMotor.request.init_motor.config.hold=5;
		
		  TempMotor.request.init_motor.config.dec=20;
			TempMotor.request.init_motor.config.run=20;
		
			TempMotor.request.init_motor.Speed.acceleration=700;
			TempMotor.request.init_motor.Speed.deceleration=700;
			TempMotor.request.init_motor.Speed.maxSpeed=50;

		
			//if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=100;
			//else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=100;
			//else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=500;
   		break;
		case 8:
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.config.acc=190;
		  TempMotor.request.init_motor.config.hold=10;
		
		  TempMotor.request.init_motor.config.dec=190;
			TempMotor.request.init_motor.config.run=190;
		
			TempMotor.request.init_motor.Speed.acceleration=3000;
			TempMotor.request.init_motor.Speed.deceleration=3000;
			TempMotor.request.init_motor.Speed.maxSpeed=1500;
	

		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1500;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1500;

			/*TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.config.acc=100;
		  TempMotor.request.init_motor.config.hold=100;
		
		  TempMotor.request.init_motor.config.dec=100;
			TempMotor.request.init_motor.config.run=100;
		
			TempMotor.request.init_motor.Speed.acceleration=3000;
			TempMotor.request.init_motor.Speed.deceleration=3000;
			TempMotor.request.init_motor.Speed.maxSpeed=150;
		

			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=800;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=2000;*/
   		break;	
			
	case 9:

			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.config.acc=100;
		  TempMotor.request.init_motor.config.hold=10;
		
		  TempMotor.request.init_motor.config.dec=100;
			TempMotor.request.init_motor.config.run=100;
		
			TempMotor.request.init_motor.Speed.acceleration=3000;
			TempMotor.request.init_motor.Speed.deceleration=3000;
			TempMotor.request.init_motor.Speed.maxSpeed=1500;
	

		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=750;//1500
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=750;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=750;
	
   		break;		
	case 10:

			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
	
			TempMotor.request.init_motor.config.acc=150;
		  TempMotor.request.init_motor.config.hold=10;
		
		  TempMotor.request.init_motor.config.dec=150;
			TempMotor.request.init_motor.config.run=150;
		
			TempMotor.request.init_motor.Speed.acceleration=5000;
			TempMotor.request.init_motor.Speed.deceleration=5000;
			//TempMotor.request.init_motor.Speed.maxSpeed=2000;
	

			if(speed_type==LOW_SPEED){
					TempMotor.request.init_motor.config.acc=120;
		      TempMotor.request.init_motor.config.hold=100;
					TempMotor.request.init_motor.config.dec=120;
					TempMotor.request.init_motor.config.run=120;
					TempMotor.request.init_motor.Speed.maxSpeed=600;
				
			}else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1200;//新丝杠用1000,旧丝杠用2400
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=3000;
			
   		break;	
	
		
		case 11:
			
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_VOLTAGE;
		  
		
			TempMotor.request.init_motor.config.acc=20;
		  TempMotor.request.init_motor.config.hold=5;
		
		  TempMotor.request.init_motor.config.dec=20;
			TempMotor.request.init_motor.config.run=20;
		
			TempMotor.request.init_motor.Speed.acceleration=3000;
			TempMotor.request.init_motor.Speed.deceleration=3000;
			TempMotor.request.init_motor.Speed.maxSpeed=100;
		
		
			if(speed_type==LOW_SPEED){
				TempMotor.request.init_motor.Speed.maxSpeed=100;
			}else if(speed_type==NORMAL_SPEED)
				TempMotor.request.init_motor.Speed.maxSpeed=1000;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;

   		break;

//	 case 12:
//			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
//		
//	 
//	 		TempMotor.request.init_motor.config.acc=350;
//		  TempMotor.request.init_motor.config.hold=10;
//		
//		  TempMotor.request.init_motor.config.dec=100;
//			TempMotor.request.init_motor.config.run=100;
//		
//			TempMotor.request.init_motor.Speed.acceleration=3000;
//			TempMotor.request.init_motor.Speed.deceleration=3000;
//			TempMotor.request.init_motor.Speed.maxSpeed=1000;
//	 
//	 	 
//			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;
//			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1500;
//			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=2000;
//		
//   		break;			
		/*case 13:
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.config.acc=350;
		  TempMotor.request.init_motor.config.hold=100;
		
		  TempMotor.request.init_motor.config.dec=100;
			TempMotor.request.init_motor.config.run=100;
		
			TempMotor.request.init_motor.Speed.acceleration=3000;
			TempMotor.request.init_motor.Speed.deceleration=3000;
			TempMotor.request.init_motor.Speed.maxSpeed=1000;
		

		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;
			else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=2000;
		
   		break;
		
		
		case 14:
			TempMotor.request.init_motor.ModeSelection=POWERSTEP01_CM_VM_CURRENT;
		
			TempMotor.request.init_motor.config.acc=350;
		  TempMotor.request.init_motor.config.hold=10;
		
		  TempMotor.request.init_motor.config.dec=100;
			TempMotor.request.init_motor.config.run=100;
		
			TempMotor.request.init_motor.Speed.acceleration=3000;
			TempMotor.request.init_motor.Speed.deceleration=3000;
			TempMotor.request.init_motor.Speed.maxSpeed=1000;
		
			if(speed_type==LOW_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1000;
		  else if(speed_type==NORMAL_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=1500;
			else if(speed_type==HIGH_SPEED)TempMotor.request.init_motor.Speed.maxSpeed=2000;
		
   		break;*/
		default:
			ret=1;
	}	
	return ret;
}

//discard
void StopALLMotorMotion(void)
{

}


uint8_t __Normal_Move_Blank__(void)
{
		uint8_t ret=0;
		int i=0;

#if (defined USE_DRV8434_CAMEL)
		DRV8434_Motor_Move_And_Wait(M4_BLANK_NEXT, M4_NEXT_TO_BLANK, 2000);//这里需要测试，
		//step1：命令下发后先延迟一段时间，防止检测的灯本来就是灭的
	  Light_Set_Delay_MS(100);
		Light_Control_Int_Enable(NORMAL_CHECK_DRAIN_LIGHT , 1);
		DRV8434_Motor_Move_Steps_Enable_Acc(M4_BLANK_NEXT, M4_NEXT_TO_BLANK, 40000);
	
	  //step1：命令下发后先延迟一段时间，防止检测的灯本来就是灭的		
	  //step2:持续检测，等待指示灯灭掉
		while(1)
		{
				i++;
				if(Light_Sensor_Get(NORMAL_CHECK_DRAIN_LIGHT))
				{	
							break;
				}else if(i>=5*1000){
							LOGE("motor can't move.\r\n");
							return 2;
				}
				delay_ms(1);	
		}
		DRV8434_Motor_HardStop_And_Goto_Sleep(M4_BLANK_NEXT);
		DRV8434_Motor_Move_Steps_Enable_Acc(M4_BLANK_NEXT, M4_NEXT_TO_BLANK, 40000);
		
		//step3:持续检测，等待指示灯亮起
		i=0;
		while(1)
		{
				i++;
				if(!Light_Sensor_Get(NORMAL_CHECK_DRAIN_LIGHT))
				{								
							break;
				}else if(i>=5*1000)
				{
							ret=1;
							LOGE("motor no find light \r\n");
							break;	
				}
				delay_ms(1);	
		}
		DRV8434_Motor_HardStop_And_Goto_Sleep(M4_BLANK_NEXT);
		DRV8434_Motor_Move_Steps_Enable_Acc(M4_BLANK_NEXT, M4_NEXT_TO_BLANK, 40000);
		
		//step4:持续检测，等待指示灯灭掉
 		i=0;
		while(1)
		{
				i++;
				if(Light_Sensor_Get(NORMAL_CHECK_DRAIN_LIGHT))
				{		
							break;
				}else if(i>=5*1000){
							LOGE("motor can't move.\r\n");
							return 2;
				}
				delay_ms(1);	
		}
		
		DRV8434_Motor_HardStop_And_Goto_Sleep(M4_BLANK_NEXT);
		Light_Control_Int_Enable(NORMAL_CHECK_DRAIN_LIGHT , 0);
#else	
		PowerStep_Select_Motor_Baby(M4_BLANK_NEXT);
		BSP_MotorControl_Move(0, M4_NEXT_TO_BLANK, 40000);
	
	  //step1：命令下发后先延迟一段时间，防止检测的灯本来就是灭的
		delay_ms(100);	
		
	  //step2:持续检测，等待指示灯灭掉
		while(1)
		{
				i++;
				if(Light_Sensor_Get(NORMAL_CHECK_DRAIN_LIGHT))
				{	
							delay_ms(100);
							break;
				}else if(i>=5*1000){
							LOGE("motor can't move.\r\n");
							return 2;
				}
				delay_ms(1);	
		}
	
		
		//step3:持续检测，等待指示灯亮起
		i=0;
		while(1)
		{
				i++;
				if(!Light_Sensor_Get(NORMAL_CHECK_DRAIN_LIGHT))
				{			delay_ms(100);						
							BSP_MotorControl_HardStop(0);	
							break;
				}else if(i>=5*1000)
				{
							ret=1;
							LOGE("motor no find light \r\n");
							BSP_MotorControl_HardStop(0);	
							break;	
				}
				delay_ms(1);	
		}
		
		//step4:持续检测，等待指示灯灭掉
 		i=0;
		while(1)
		{
				i++;
				if(Light_Sensor_Get(NORMAL_CHECK_DRAIN_LIGHT))
				{			delay_ms(100);	
							break;
				}else if(i>=5*1000){
							LOGE("motor can't move.\r\n");
							return 2;
				}
				delay_ms(1);	
		}
		
#endif
		
		return ret;
}


uint8_t __Normal_Pitch_Move_Next__(int motorNum,int lightNum, motorDir_t motorDir,uint32_t steps, uint16_t delayMs)
{
		uint8_t ret=0;
		int i=0;
	
#if (defined USE_DRV8434_CAMEL)
		DRV8434_Motor_Move_And_Wait(motorNum, motorDir, 2000);//这里需要测试，
	
		//step1：命令下发后先延迟一段时间，防止检测的灯本来就是灭的
	  Light_Set_Delay_MS(20);
		Light_Control_Int_Enable(lightNum , 1);
		DRV8434_Motor_Move_Steps_Enable_Acc(motorNum, motorDir, steps);
	 //step2:持续检测，等待指示灯灭掉
		while(1)
		{
				i++;
				if(Light_Sensor_Get(lightNum))
				{				
							break;
				}else if(i>=2*1000){
							LOGE("motor can't move.\r\n");
							return 2;
				}
				delay_ms(1);	
		}
		DRV8434_Motor_HardStop_And_Goto_Sleep(motorNum);
		Light_Set_Delay_MS(delayMs);
		DRV8434_Motor_Move_Steps_Enable_Acc(motorNum, motorDir, steps);
		//step3:持续检测，等待指示灯亮起
		i=0;
		while(1)
		{
				i++;
				if(!Light_Sensor_Get(lightNum))
				{			
							break;
				}else if(i>=2*1000)
				{
							ret=1;
							LOGE("motor no find light \r\n");
							break;	
				}
				delay_ms(1);	
		}
		
		DRV8434_Motor_HardStop_And_Goto_Sleep(motorNum);
		Light_Control_Int_Enable(lightNum , 0);
#else	
		PowerStep_Select_Motor_Baby(motorNum);
		BSP_MotorControl_Move(0, motorDir, steps);
	
		//step1：命令下发后先延迟一段时间，防止检测的灯本来就是灭的
	  delay_ms(100);	
	
	 //step2:持续检测，等待指示灯灭掉
		while(1)
		{
				i++;
				if(Light_Sensor_Get(lightNum))
				{				
							break;
				}else if(i>=2*1000){
							LOGE("motor can't move.\r\n");
							return 2;
				}
				delay_ms(1);	
		}
	
		//step3:持续检测，等待指示灯亮起
		i=0;
		while(1)
		{
				i++;
				if(!Light_Sensor_Get(lightNum))
				{			
							delay_ms(delayMs);
					
							BSP_MotorControl_HardStop(0);	
							break;
				}else if(i>=2*1000)
				{
							ret=1;
							LOGE("motor no find light \r\n");
							BSP_MotorControl_HardStop(0);	
							break;	
				}
				delay_ms(1);	
		}
#endif
		

	return ret;
}	

uint32_t get_motor_rest_steps(int motorNum)
{
	uint32_t steps=0;
	switch(motorNum)
	{
		case M1_MIX_V: steps=110000;break;
		case M3_LEFT_WAIT: steps=110000;break;
		case M4_BLANK_NEXT: steps=110000;break;
		case M7_HIGH_TURN: steps=11000;break;
		case M8_BIG_IN_OUT: steps=120000;break;
		case M9_IN_OUT: steps=120000;break;
		case M10_UP_DOWM: steps=60000;break;
		case M11_FAR_NEAR: steps=50000;break;
		default:
			;
	}
  return steps;
}


uint8_t RestSelectMotorOrgin(int motorNum,int lightNum, motorDir_t motorDir,uint32_t steps)
{
	int status=0;
	int i=0;
  uint8_t light=1;
	
	if(!Light_Sensor_Get(lightNum))return 0;
	
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
	
	if(M11_FAR_NEAR==motorNum)DRV8434_Motor_Select_Speed(M11_FAR_NEAR,LOW_SPEED);
	if(M4_BLANK_NEXT==motorNum)DRV8434_Motor_Select_Speed(M4_BLANK_NEXT,HIGH_SPEED);
	
		steps=get_motor_rest_steps(motorNum);
	
		if(motorNum==M11_FAR_NEAR || motorNum==M7_HIGH_TURN )
		{
			Light_Set_Delay_MS(30);
		}else if(motorNum==M3_LEFT_WAIT || motorNum==M4_BLANK_NEXT)
		{
			Light_Set_Delay_MS(10);
		}else if(motorNum==M1_MIX_V)
		{
			Light_Set_Delay_MS(20);
		}else{
			Light_Set_Delay_MS(20);
		}
		
		Light_Control_Int_Enable(lightNum , 1);
		DRV8434_Motor_Move_Steps_Enable_Acc(motorNum, motorDir, steps);
		while(1){
				i++;
				light=Light_Sensor_Get(lightNum);
				if(!light)
				{				
							light=0;
							break;
				}else if(i>=6*1000){
							light=1;
							LOGE("motor no find light \r\n");
							break;	
				}
				delay_ms(1);	
		}
	DRV8434_Motor_HardStop_And_Goto_Sleep(motorNum);
	Light_Control_Int_Enable(lightNum , 0);		
	if(M11_FAR_NEAR==motorNum)DRV8434_Motor_Select_Speed(M11_FAR_NEAR,NORMAL_SPEED);
	if(M4_BLANK_NEXT==motorNum)DRV8434_Motor_Select_Speed(M4_BLANK_NEXT,NORMAL_SPEED);

#else	
	if(M11_FAR_NEAR==motorNum)Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);
		
		steps=get_motor_rest_steps(motorNum);
	
		PowerStep_Select_Motor_Baby(motorNum);
		BSP_MotorControl_Move(0, motorDir, steps);
		while(1){
				i++;
				light=Light_Sensor_Get(lightNum);
				if(!light)
				{			
							light=0;
							if(motorNum==M11_FAR_NEAR || motorNum==M7_HIGH_TURN )
							{
								delay_ms(30);
							}else if(motorNum==M3_LEFT_WAIT || motorNum==M4_BLANK_NEXT)
							{
								delay_ms(10);
							}else{
								delay_ms(20);
							}
							
							
							BSP_MotorControl_HardStop(0);	
							break;
				}else if(i>=6*1000){
							light=1;
							LOGE("motor no find light \r\n");
							BSP_MotorControl_HardStop(0);	
							break;	
				}
				delay_ms(1);	
		}		
	if(M11_FAR_NEAR==motorNum)Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
#endif
		
		
	return light;		
}


uint8_t RestSelectMotorOrginDelay(int motorNum,int lightNum, motorDir_t motorDir,uint32_t steps,uint16_t delayMs)
{
	int status=0;
	int i=0;
  uint8_t light=1;
	
	if(!Light_Sensor_Get(lightNum))return 0;
	
#if (defined USE_DRV8434_CAMEL) || (defined USE_DRV8434_PECKER)	
	if(M11_FAR_NEAR==motorNum)DRV8434_Motor_Select_Speed(M11_FAR_NEAR,LOW_SPEED);

		steps=get_motor_rest_steps(motorNum);
		Light_Set_Delay_MS(delayMs);
		Light_Control_Int_Enable(lightNum , 1);
		DRV8434_Motor_Move_Steps_Enable_Acc(motorNum, motorDir, steps);

		while(1){
				i++;
				if(!Light_Sensor_Get(lightNum))
					{				
							light=0;
							break;
				}else if(i>=6*1000){
							light=1;
							LOGE("motor no find light \r\n");
							break;	
				}
				delay_ms(1);	
		}
	DRV8434_Motor_HardStop_And_Goto_Sleep(motorNum);		
	Light_Control_Int_Enable(lightNum , 0);
	if(M11_FAR_NEAR==motorNum)DRV8434_Motor_Select_Speed(M11_FAR_NEAR,NORMAL_SPEED);


#else	
	
	if(M11_FAR_NEAR==motorNum)Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,LOW_SPEED);

		
		steps=get_motor_rest_steps(motorNum);
	
		PowerStep_Select_Motor_Baby(motorNum);
		BSP_MotorControl_Move(0, motorDir, steps);
	  //delay_ms(100);
		while(1){
				i++;
				if(!Light_Sensor_Get(lightNum))
					{				
							light=0;
						  delay_ms(delayMs);
							BSP_MotorControl_HardStop(0);	
							break;
				}else if(i>=6*1000){
							light=1;
							LOGE("motor no find light \r\n");
							BSP_MotorControl_HardStop(0);	
							break;	
				}
				delay_ms(1);	
		}		
	if(M11_FAR_NEAR==motorNum)Choose_Single_Motor_Speed_Config(M11_FAR_NEAR,NORMAL_SPEED);
#endif
		
	return light;		
}



/*
*HAL 层用于设置电机参数的接口
*/
void Set_Single_Motor_Config(init_motor_speed_tension_type_t data)
{

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
			Powerstep01_Init_Register((void*)&MotorConfig);
			BSP_MotorControl_HardStop(0);
}	




//初始化
void BSP_Motor_Control_Init(void)
{
int i=0,result=0,value=0;
int start=0,end=0;	
	
#if USE_AUTOMATIC_INJECTION_BOARD	
		start=1;end=7;
#endif		
#if USE_CLEANING_DILUTION_BOARD
		start=8;end=11;
#endif
		init_mix_motor_config();
	
	
		
#if (USE_AUTOMATIC_INJECTION_BOARD||USE_CLEANING_DILUTION_BOARD)	
		BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, 1);
		BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);
	for(i=start;i<=end;i++)
	{
			Choose_Single_Motor_Speed_Config(i,NORMAL_SPEED);
	}	
#endif

			
}	










