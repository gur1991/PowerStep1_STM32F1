#include "S1125.h"

Uart_Receive_Data S1125_Read=NULL ;
Uart_Send_Data S1125_Write=NULL ;
static S1125_protocl_type pump;
static u8 S1125_rx_buf [30];

void S1125_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

		__HAL_RCC_GPIOB_CLK_ENABLE();         
	
    GPIO_Initure.Pin=GPIO_PIN_9; 			
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);	
}



void init_s1125_pump(void)
{
	S1125_Read = GetUartReceive(PUMP_UART_PORT,PUMP_UART_CS);
	S1125_Write = GetUartSend(PUMP_UART_PORT,PUMP_UART_CS);
	
	pump.start=':';
	pump.check[0]=pump.check[1]='?';
	memcpy(pump.end,"\r\n", 2);
}	

int transfer_s1125(void)
{
	int len=0;
	memset(S1125_rx_buf, 0, sizeof(S1125_rx_buf));
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);	
	S1125_Write((u8*)&pump,sizeof(S1125_protocl_type));
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);	
	delay_ms(100);
	S1125_Read(S1125_rx_buf,&len);
	return len;
}

int Run_S1125_Pump(void)
{
	int len=0;
	init_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	
	memcpy(pump.address,"00CC", 4);
	memcpy(pump.value,"0011", 4);
	len=transfer_s1125();
	
	memcpy(pump.address,"012C", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	
	
	return len;
}	


int Stop_S1125_Pump(void)
{
	
	int len=0;
	init_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"012D", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	return len;
}

int Read_Press_S1125_Pump(void)
{
	
	int len=0;
	int press=0;
	int i=0;
	init_s1125_pump();
	memcpy(pump.device,"0F04", 4);
	memcpy(pump.address,"0065", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	
	for(i=7;i<11;i++)
	{
		//printf("char %c\r\n",S1125_rx_buf[i]);
		
			if(S1125_rx_buf[i]<'A')S1125_rx_buf[i]-='0';
			else S1125_rx_buf[i]-=55;
		
			//printf("%d\r\n",S1125_rx_buf[i]);
		
	}
	
	press=S1125_rx_buf[10]+S1125_rx_buf[9]*16+S1125_rx_buf[8]*16*16+S1125_rx_buf[7]*16*16*16;

	return press;
}	

int Write_FlowSpeed_s1125_pump(int SpeedFlow)
{
	int len=0;
	int i=0;
	pump.value[3]=SpeedFlow%16;
	SpeedFlow/=16;
	pump.value[2]=SpeedFlow%16;
	SpeedFlow/=16;
	pump.value[1]=SpeedFlow%16;
	SpeedFlow/=16;
	pump.value[0]=SpeedFlow%16;
	
	for(i=0;i<4;i++)
	{
			if(pump.value[i]<10)pump.value[i]+='0';
			else pump.value[i]+=55;
	}
	init_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"00C8", 4);
	memcpy(pump.value,"07D0", 4);
	len=transfer_s1125();
	return len;
}	



int Write_MinPress_s1125_pump(int MinPress)
{
	
		int len=0;
		int i=0;
	pump.value[3]=MinPress%16;
	MinPress/=16;
	pump.value[2]=MinPress%16;
	MinPress/=16;
	pump.value[1]=MinPress%16;
	MinPress/=16;
	pump.value[0]=MinPress%16;
	
	for(i=0;i<4;i++)
	{
			if(pump.value[i]<10)pump.value[i]+='0';
			else pump.value[i]+=55;
	}
	
	init_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"00C9", 4);
	memcpy(pump.value,"0000", 4);
	len=transfer_s1125();
	return len;
	
}	


int Write_MaxPress_s1125_pump(int MaxPress)
{
	int len=0;
	int i=0;
	pump.value[3]=MaxPress%16;
	MaxPress/=16;
	pump.value[2]=MaxPress%16;
	MaxPress/=16;
	pump.value[1]=MaxPress%16;
	MaxPress/=16;
	pump.value[0]=MaxPress%16;
	
	for(i=0;i<4;i++)
	{
			if(pump.value[i]<10)pump.value[i]+='0';
			else pump.value[i]+=55;
	}
	
	init_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"00CA", 4);
	memcpy(pump.value,"0000", 4);
	len=transfer_s1125();
	return len;
	
}	




int pump_s1125_process_cmd(pump_s1125_type_t pump_s1125)
{
	int value=0;
	switch(pump_s1125.request.type)
	{
		case RUN_S1125:
			Run_S1125_Pump();
			break;
	
		case STOP_S1125:
			Stop_S1125_Pump();
			break;
		
		case READ_PRESS:
			value=Read_Press_S1125_Pump();
			break;
		
		case WRITE_MAX_PRESS:
			Write_MaxPress_s1125_pump(pump_s1125.request.para);
			break;
		
		case WRITE_MIN_PRESS:
			Write_MinPress_s1125_pump(pump_s1125.request.para);
			break;
		
		case WRITE_FLOW_SPEED:
			Write_FlowSpeed_s1125_pump(pump_s1125.request.para);
			break;
		
		case WRITE_PRESS:
			Write_MinPress_s1125_pump(pump_s1125.request.para);
			Write_MaxPress_s1125_pump(pump_s1125.request.para_nor);
			break;
		
		default:
			break;
	}	

	return value;
}	