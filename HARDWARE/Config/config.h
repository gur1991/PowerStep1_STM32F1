#ifndef __CONFIG_H
#define __CONFIG_H


//new start for dev branch

#define USE_GRADIENT_CONTROL_BOARD 0 //Hummingbird--梯度控制，输液泵，电磁阀ABC，脱气机，薄而贴，风扇

#define USE_CLEANING_DILUTION_BOARD 0 //Pecker----C55 C52 两个注射器 控制上下和控制远近的电机

#define USE_AUTOMATIC_INJECTION_BOARD 1 //Camel---其他温度重力等模块

#define USE_KEEP_TEMPERATURE_BOARD 0// mini control temperature



/*****************************************************/
//UART4 ----- ARM 
//#define USE_SENSOR_BOARD 1

//#define USE_MOTOR_BOARD 0

#define COUNT_TIME (10*1000-1) //4s

#define  C52_UART_PORT UART2_RS232
#define  C52_UART_CS  CS_ZERO  

#define  C55_UART_PORT UART2_RS232
#define  C55_UART_CS CS_TWO


#define  FM100_UART_PORT UART2_RS232
#define  FM100_UART_CS CS_THREE

#define  BL180_UART_PORT UART2_RS232
#define  BL180_UART_CS CS_THREE



#define  PUMP_UART_PORT UART2_RS232
#define  PUMP_UART_CS CS_ONE

#define  RFID_UART_PORT UART2_RS232
#define  RFID_UART_CS CS_ZERO


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
