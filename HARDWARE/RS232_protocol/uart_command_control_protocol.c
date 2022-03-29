#include "uart_command_control_protocol.h"
#include "stdio.h"
#include "config.h"

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

	LOGD("MyFlagInterruptHandler\r\n");
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
		LOGD("MyBusyInterruptHandler\r\n");
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
  
	LOGD("MyErrorHandler\r\n");
  /* Infinite loop */
  while(1)
  {
		break ;
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
		LOGD("spi end \r\n");
  }

		return ret;
}


void master_device_send(void){
/*	
	uint8_t ret=0;
		Command_type_t types= MOVE_TYPE;
		u8 TX_CMD_BUF[5]={1,2,3,4,5};
		u8 RX_CMD_BUF[5];
		u8 len=0;
		
		UART2_Send_Data(&types,sizeof(Command_type_t));
		LOGD("master len: %d, types: %d \r\n",sizeof(Command_type_t),types);
 */  
}
void master_device_receive(void){
/*		
	uint8_t ret=0;
		Command_type_t types= MOVE_TYPE;
		u8 TX_CMD_BUF[5]={1,2,3,4,5};
		u8 RX_CMD_BUF[5];
		u8 len=0;
		
		UART2_Send_Data(&types,sizeof(Command_type_t));
		LOGD("master len: %d, types: %d \r\n",sizeof(Command_type_t),types);
 */   
}
void slave_device_send(void){
/*		
	uint8_t ret=0;
		Command_type_t types= MOVE_TYPE;
		u8 TX_CMD_BUF[5]={1,2,3,4,5};
		u8 RX_CMD_BUF[5];
		int len=0;
		
		UART3_Receive_Data(RX_CMD_BUF,&len);
		
		LOGD("slave len: %d, types: %d \r\n",len,RX_CMD_BUF[0]);
*/
	}
void slave_device_receive(void){
/*	
	uint8_t ret=0;
		Command_type_t types= MOVE_TYPE;
		u8 TX_CMD_BUF[5]={1,2,3,4,5};
		u8 RX_CMD_BUF[5];
		int len=0;
		
		UART3_Receive_Data(RX_CMD_BUF,&len);
		
		LOGD("slave len: %d, types: %d \r\n",len,RX_CMD_BUF[0]);
*/
}

uint8_t test_protocl(void){
#if 0
	uint8_t ret=0;
		Command_type_t types= MOVE_TYPE;
		u8 TX_CMD_BUF[5]={1,2,3,4,5};
		u8 RX_CMD_BUF[5];
		
		u8 TX_CMD_BUF_SLAVE[5]={1,2,3,4,5};
		u8 RX_CMD_BUF_SLAVE[5];
			
		u8 RX_PARA_BUF[100];
		int len=0;
		move_type_t motor_slave;
		move_type_t motor_master;
	
		motor_master.request.devices=2;
		motor_master.request.steps=6400;
		motor_master.request.types=POWERSTEP01_GO_TO;
	
	//	len=sizeof(motor.request);
		
		
		UART2_Send_Data(&types,sizeof(Command_type_t));
		LOGD("master send len: %d, types: %d \r\n",sizeof(Command_type_t),types);
						UART3_Receive_Data(RX_CMD_BUF_SLAVE,&len);
						LOGD("slave receive len: %d, types: %d \r\n",len,RX_CMD_BUF_SLAVE[0]);
						TX_CMD_BUF_SLAVE[0]=0;
						UART3_Send_Data(TX_CMD_BUF_SLAVE,1);
						LOGD("slave send len: %d, result: %d \r\n",1,TX_CMD_BUF_SLAVE[0]);
		UART2_Receive_Data(RX_CMD_BUF,&len);
		LOGD("master receive len: %d, result: %d \r\n",len,RX_CMD_BUF[0]);
		
		UART2_Send_Data((u8*)(&motor_master.request),sizeof(motor_master.request));
		LOGD("master send request \r\n");
						
		switch(RX_CMD_BUF_SLAVE[0]){
			case MOVE_TYPE:
					
									UART3_Receive_Data((u8*)(&motor_slave.request),&len);
									LOGD("slave receive request \r\n");
									LOGD("devices:%d \r\n",motor_slave.request.devices);
									LOGD("steps:%d \r\n",motor_slave.request.steps);
									LOGD("types:%d \r\n",motor_slave.request.types);
			
									/*
											slave motor run cmd
									*/
									motor_slave.response.ret=100;
									UART3_Send_Data((u8*)&motor_slave.response,sizeof(motor_slave.response));
									LOGD("slave send response \r\n");
					UART2_Receive_Data((u8*)(&motor_master.response),&len);
					LOGD("master receive len: %d, result: %d \r\n",len,motor_master.response.ret);
					LOGD("\r\n\r\n");
					break;
			default:
					LOGD("no found this cmd ! \r\n");

		}
		
		return ret;
#endif
return 0;		
}


uint8_t test_protocl_new(Command_type_t types){
		uint8_t ret =0;

	
	
		return ret;
}

uint8_t master_powerStep01_move(move_type_t tx_move){
		uint8_t ret =0;
/*		
	int len=0;
		move_type_t rx_move;
	
		Powerstep1_contorl_motor_command_t master_motorCommand,slave_motorCommand;
		master_motorCommand.type = MOVE_TYPE;
		master_motorCommand.CommandPowerStep1.move=tx_move;
		
		UART2_Send_Data((u8*)(&master_motorCommand),sizeof(Powerstep1_contorl_motor_command_t));
		UART3_Receive_Data((u8*)&slave_motorCommand,&len);
		
		switch(slave_motorCommand.type){
			case MOVE_TYPE:
						rx_move=slave_motorCommand.CommandPowerStep1.move;
						LOGD("devices %d \r\n",rx_move.request.devices);
						LOGD("steps %d \r\n",rx_move.request.steps);
						LOGD("types %d \r\n",rx_move.request.types);
						break;
			default:
					LOGE("no found this cmd ! \r\n");
		}
*/	
		return ret;
}


void test_run(void){
/*	
	move_type_t tx_move;
	tx_move.request.devices=2;
	tx_move.request.steps=6400;
	tx_move.request.types=POWERSTEP01_GO_TO;
	
	master_powerStep01_move(tx_move);
*/
}
























