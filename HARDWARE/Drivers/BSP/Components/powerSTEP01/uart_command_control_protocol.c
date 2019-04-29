#include "uart_command_control_protocol.h"
#include "stdio.h"


static volatile uint16_t gLastError;

/**
  * @brief  This function is the User handler for the flag interrupt
  * @param  None
  * @retval None
  */
void MyFlagInterruptHandler(void)
{
  /* Get the value of the status register via the command GET_STATUS */
  uint16_t statusRegister = BSP_MotorControl_CmdGetStatus(0);

  /* Check HIZ flag: if set, power brigdes are disabled */
  if ((statusRegister & POWERSTEP01_STATUS_HIZ) == POWERSTEP01_STATUS_HIZ)
  {
    // HIZ state
  }
  /* Check BUSY flag: if not set, a command is under execution */
  if ((statusRegister & POWERSTEP01_STATUS_BUSY) == 0)
  {
    // BUSY
  }
  /* Check SW_F flag: if not set, the SW input is opened */
  if ((statusRegister & POWERSTEP01_STATUS_SW_F ) != 0)
  {
    // SW CLOSED   
  }  
  /* Check SW_EN bit */
  if ((statusRegister & POWERSTEP01_STATUS_SW_EVN) == POWERSTEP01_STATUS_SW_EVN)
  {
    // switch turn_on event
  }  
  /* Check direction bit */
  if ((statusRegister & POWERSTEP01_STATUS_DIR) == 0)
  {
    // BACKWARD
  }
  else  
  {
    // FORWARD 
  }
  if ((statusRegister & POWERSTEP01_STATUS_MOT_STATUS) ==
      POWERSTEP01_STATUS_MOT_STATUS_STOPPED )
  {
       // MOTOR STOPPED
  }
  else  if ((statusRegister & POWERSTEP01_STATUS_MOT_STATUS) ==
            POWERSTEP01_STATUS_MOT_STATUS_ACCELERATION )
  {
           // MOTOR ACCELERATION
  }  
  else  if ((statusRegister & POWERSTEP01_STATUS_MOT_STATUS) ==
            POWERSTEP01_STATUS_MOT_STATUS_DECELERATION )
  {
           // MOTOR DECELERATION
  }
  else  if ((statusRegister & POWERSTEP01_STATUS_MOT_STATUS) ==
            POWERSTEP01_STATUS_MOT_STATUS_CONST_SPD )
  {
       // MOTOR RUNNING AT CONSTANT SPEED
  }  

  /* Check Command Error flag: if set, the command received by SPI can't be */
  /* performed. This occurs for instance when a move command is sent to the */
  /* Powerstep01 while it is already running */
  if ((statusRegister & POWERSTEP01_STATUS_CMD_ERROR) ==
      POWERSTEP01_STATUS_CMD_ERROR)
  {
       // Command Error
  }  

  /* Check Step mode clock flag: */
  /* if set, the device is working in step clock mode */
  if ((statusRegister & POWERSTEP01_STATUS_STCK_MOD) ==
      POWERSTEP01_STATUS_STCK_MOD)
  {
     //Step clock mode enabled
  }  

  /* Check UVLO flag: if not set, there is an undervoltage lock-out */
  if ((statusRegister & POWERSTEP01_STATUS_UVLO) == 0)
  {
     //undervoltage lock-out 
  }  

  /* Check UVLO ADC flag: if not set, there is an ADC undervoltage lock-out */
  if ((statusRegister & POWERSTEP01_STATUS_UVLO_ADC) == 0)
  { 
     //ADC undervoltage lock-out 
  } 
  
  /* Check thermal STATUS flags: if  set, the thermal status is not normal */
  if ((statusRegister & POWERSTEP01_STATUS_TH_STATUS) != 0)
  {
    //thermal status: 1: Warning, 2: Bridge shutdown, 3: Device shutdown
  }    

  /* Check OCD  flag: if not set, there is an overcurrent detection */
  if ((statusRegister & POWERSTEP01_STATUS_OCD) == 0)
  {
    //overcurrent detection 
  }      

  /* Check STALL_A flag: if not set, there is a Stall condition on bridge A */
  if ((statusRegister & POWERSTEP01_STATUS_STALL_A) == 0)
  {
    //Bridge A stalled 
  }    

  /* Check STALL_B flag: if not set, there is a Stall condition on bridge B */
  if ((statusRegister & POWERSTEP01_STATUS_STALL_B) == 0)
  {
    //Bridge B stalled  
  }      

}

/**
  * @brief  This function is the User handler for the busy interrupt
  * @param  None
  * @retval None
  */
void MyBusyInterruptHandler(void)
{

   if (BSP_MotorControl_CheckBusyHw())
   {
      /* Busy pin is low, so at list one Powerstep01 chip is busy */
     /* To be customized (for example Switch on a LED) */
   }
   else
   {
     /* To be customized (for example Switch off a LED) */
   }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param[in] error Number of the error
  * @retval None
  */
void MyErrorHandler(uint16_t error)
{
  /* Backup error number */
  gLastError = error;
  
  /* Infinite loop */
  while(1)
  {
  }
}


uint8_t motorInit(void){
		uint8_t ret =0;
  uint32_t myMaxSpeed;
  uint32_t myMinSpeed;
  uint16_t myAcceleration;
  uint16_t myDeceleration;
  uint32_t readData;

  BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, 3);
	BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);
	BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);
	BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_POWERSTEP01, NULL);
  BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);
  BSP_MotorControl_AttachBusyInterrupt(MyBusyInterruptHandler); 
  BSP_MotorControl_AttachErrorHandler(MyErrorHandler);
  BSP_MotorControl_CmdResetPos(0);
  BSP_MotorControl_CmdResetPos(1);
  BSP_MotorControl_CmdResetPos(2);
  while(1)
  {
    /* device 0 is using 1/16 microstepping mode */ 
    /* device 1 is using 1/8 microstepping mode */
    /* device 2 is using full step mode */
    /* position is in microsteps */
    BSP_MotorControl_QueueCommands(0,POWERSTEP01_GO_TO,-3200);
  //  BSP_MotorControl_QueueCommands(1,POWERSTEP01_GO_TO,1600);
    BSP_MotorControl_QueueCommands(2,POWERSTEP01_GO_TO,-3200);
    BSP_MotorControl_SendQueuedCommands();
    
		
		//HAL_Delay(3000);
	//	BSP_MotorControl_CmdSoftStop(0);
  
  /* Wait for device 1 end moving */
		//BSP_MotorControl_WaitWhileActive(0); 
	//	BSP_MotorControl_QueueCommands(0,POWERSTEP01_HARD_STOP,0);
	//	BSP_MotorControl_SendQueuedCommands();
    /* Wait for all device ends moving */ 
    BSP_MotorControl_WaitForAllDevicesNotBusy();

    BSP_MotorControl_QueueCommands(0,POWERSTEP01_GO_TO,3200);
 //   BSP_MotorControl_QueueCommands(1,POWERSTEP01_GO_TO,-1600);
    BSP_MotorControl_QueueCommands(2,POWERSTEP01_GO_TO,3200);
    BSP_MotorControl_SendQueuedCommands();
    
    /* Wait for all device ends moving */ 
    BSP_MotorControl_WaitForAllDevicesNotBusy();
//		BSP_MotorControl_CmdResetPos(0);
		//HAL_Delay(3000);
		printf("spi end \r\n");
  }

		return ret;
}




uint8_t test_move(){
		uint8_t ret =0;
		uint16_t len=0;
		move_type_t motor;
	
		motor.request.devices=0;
		motor.request.steps=6400;
		motor.request.types=POWERSTEP01_GO_TO;
	
		len=sizeof(motor.request);
  
	/*
	uint8_t type =MOVE_TYPE;
	USART1_Send((u8*)&type,1);
	
	switch(type){
		case MOVE_TYPE:
				call_back(send_cmd);
				wait_recevie()
				USART1_Send((u8*)&motor,len);
				call_back(para);
	}
	
	
	*/
	
	
	return ret;
}