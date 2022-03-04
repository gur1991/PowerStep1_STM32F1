#ifndef __CONFIG_H
#define __CONFIG_H
#include <stdio.h>



//新的结构设计定义
#define USE_NEW_DESIGN 1
//new start for dev branch

#define USE_GRADIENT_CONTROL_BOARD 0//Hummingbird--梯度控制，输液泵，电磁阀ABC，脱气机，薄而贴，风扇

#define USE_CLEANING_DILUTION_BOARD 0//Pecker----C55 C52 两个注射器  控制上下和控制远近的电机

#define USE_AUTOMATIC_INJECTION_BOARD 0//Camel---其他温度重力等模块    旋转混匀 传送带 扫码器 RFID

#define USE_KEEP_TEMPERATURE_BOARD 1// mini control temperature

//#define USE_DRV8434_PECKER 
//#define USE_DRV8434_CAMEL 




//#define USE_NEW_PID_CONTROL_HEATER 



#define _SHARK_HEADER_ "VG1-"
#define _SHARK_VERSION_ "-4.4.2201110.D"

#if USE_GRADIENT_CONTROL_BOARD
	#define _STM_BOARD_  "SH"
#endif
#if USE_CLEANING_DILUTION_BOARD
	#define _STM_BOARD_  "SP"
#endif
#if USE_AUTOMATIC_INJECTION_BOARD
	#define _STM_BOARD_  "SC"
#endif
#if USE_KEEP_TEMPERATURE_BOARD
	#define _STM_BOARD_  "ST"
#endif

//#define STM_VERSION #_SHARK_HEADER_###_STM_BOARD_###_SHARK_VERSION_


#define LOGD(format,...)  do { printf("[%s:%d#]" format,__func__,__LINE__,##__VA_ARGS__); }while(0)
#define LOGE(format,...)  do { printf("[ERROR %s:%d#]" format,__func__,__LINE__,##__VA_ARGS__); }while(0)
				
/*****************************************************/
//UART4 ----- ARM 

#define COUNT_TIME (10*1000-1) //4s

#define  C52_UART_PORT UART2_RS232
#define  C52_UART_CS  CS_ONE  

#define  C55_UART_PORT UART2_RS232
#define  C55_UART_CS CS_ZERO


#define  FM100_UART_PORT UART2_RS232
#define  FM100_UART_CS CS_ZERO

#define  BL180_UART_PORT UART2_RS232
#define  BL180_UART_CS CS_ZERO


#define  PUMP_UART_PORT UART2_RS232
#define  PUMP_UART_CS CS_THREE

#define  RFID_UART_PORT UART2_RS232
#define  RFID_UART_CS CS_ONE


/*
#define CLK_COUNT 50
#define DUTY_CYCLE 0.25

#define HIGH_TIME 40
#define HIGH_LOW_TIME 2000
#define LOW_TIME 1000

	//HIGH
	TIM_SetTIM3Compare4(0);
	delay_ms(HIGH_TIME);
	
	//PWM
	TIM_SetTIM3Compare4((int)(CLK_COUNT*(1-DUTY_CYCLE)));
	delay_ms(HIGH_LOW_TIME);
	
	//LOW
	TIM_SetTIM3Compare4(CLK_COUNT);
	delay_ms(LOW_TIME);
*/





#endif
