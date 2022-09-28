#include "S1125.h"
#include "electromagnetic.h"
#include "config.h"


Uart_Receive_Data S1125_Read=NULL ;
Uart_Send_Data S1125_Write=NULL ;
static S1125_protocl_type pump;
static u8 S1125_rx_buf [30];


void S1125_Init(void)
{
	LOGD("init S1125 pump\r\n");
}



void test_gpio(void)
{
}	


void config_s1125_pump(void)
{
	S1125_Read = GetUartReceive(PUMP_UART_PORT,PUMP_UART_CS);
	S1125_Write = GetUartSend(PUMP_UART_PORT,PUMP_UART_CS);
	Uart_Select_Baby(PUMP_UART_PORT, PUMP_UART_CS);
	
	pump.start=':';
	pump.check[0]=pump.check[1]='?';
	memcpy(pump.end,"\r\n", 2);
}	

int transfer_s1125(void)
{
	int len=0;

#if USE_GRADIENT_CONTROL_BOARD
		int temp_a=0,temp_b=0;
	int i=200;
	memset(S1125_rx_buf, 0, sizeof(S1125_rx_buf));
	
	uart_rts_control(PUMP_UART_CS, 1);
	
	S1125_Write((u8*)&pump,sizeof(S1125_protocl_type));
	//delay_ms(150);
	while(i--)
	{
		temp_a=USART2_RX_CNT;
		delay_ms(1);
		temp_b=USART2_RX_CNT;
		if(temp_b!=0 && temp_b==temp_a)
		{
			break;
		}	
	}	
	S1125_Read(S1125_rx_buf,&len);
	
	uart_rts_control(PUMP_UART_CS, 0);
#endif
	
	return len;
}

uint8_t Run_S1125_Pump(void)
{
#if USE_GRADIENT_CONTROL_BOARD	
	int len=0;
	config_s1125_pump();
	
	memcpy(pump.device,"0F06", 4);
/*
	memcpy(pump.address,"00CC", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();

	if(0==len)return 1;
*/
	
	memcpy(pump.address,"012C", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	
	if(0==len)return 1;
	else return 0;
#endif
	
	return 0;
}	

uint8_t Stop_S1125_Pump(void)
{
#if USE_GRADIENT_CONTROL_BOARD	
	int len=0;
	config_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"012D", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	
	if(0==len)return 1;
	else return 0;
#endif
	
	return 0;
}

uint8_t Start_S1125_Purge(void)
{
#if USE_GRADIENT_CONTROL_BOARD	
	int len=0;
	config_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"012E", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	
	if(0==len)return 1;
	else return 0;
#endif
	
	return 0;
}
uint8_t Stop_S1125_Purge(void)
{
#if USE_GRADIENT_CONTROL_BOARD	
	int len=0;
	config_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"012F", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	
	if(0==len)return 1;
	else return 0;
#endif
	
	return 0;
}




//test connect :0F06006F00  --6F00
uint8_t Connect_S1125_Pump(void)
{
#if USE_GRADIENT_CONTROL_BOARD	
	int len=0;
	config_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"006F", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	
	if(0==len)return 1;
	else return 0;
#endif

	return 0;
}




int Read_Press_S1125_Pump(void)
{
	int press=0;
#if USE_GRADIENT_CONTROL_BOARD	
		int i=0;
		int len=0;
	config_s1125_pump();
	
	memcpy(pump.device,"0F04", 4);
	memcpy(pump.address,"0065", 4);
	memcpy(pump.value,"0001", 4);
	len=transfer_s1125();
	
	if(len<12)return -1;	

	for(i=7;i<11;i++)
	{		
			if(S1125_rx_buf[i]<'A')S1125_rx_buf[i]-='0';
			else S1125_rx_buf[i]-=55;		
	}

	press=S1125_rx_buf[10]+S1125_rx_buf[9]*16+S1125_rx_buf[8]*16*16+S1125_rx_buf[7]*16*16*16;

#endif	
	return press;
}	

uint8_t Write_FlowSpeed_s1125_pump(int SpeedFlow)
{
#if USE_GRADIENT_CONTROL_BOARD
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
			//LOGD("[%d]%c\r\n",i,pump.value[i]);
	}
	config_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"00C8", 4);
	memcpy(pump.value,pump.value, 4);
	len=transfer_s1125();

	if(0==len)return 1;
	else return 0;
	
#endif	
	return 0;
}	



uint8_t Write_MinPress_s1125_pump(int MinPress)
{
#if USE_GRADIENT_CONTROL_BOARD	
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
	
	config_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"00C9", 4);
	memcpy(pump.value,"0000", 4);
	len=transfer_s1125();
	
	if(0==len)return 1;
	else return 0;
#endif
	
	return 0;
	
}	


uint8_t Write_MaxPress_s1125_pump(int MaxPress)
{
	
#if USE_GRADIENT_CONTROL_BOARD	
	uint8_t len=0;
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
	
	config_s1125_pump();
	memcpy(pump.device,"0F06", 4);
	memcpy(pump.address,"00CA", 4);
	memcpy(pump.value,pump.value, 4);
	len=transfer_s1125();
	
	if(0==len)return 1;
	else return 0;
#endif
	
	return 0;
	
}	
uint8_t Write_Press_s1125_pump(int MinPress, int MaxPress)
{
	 uint8_t ret=0;
	
#if USE_GRADIENT_CONTROL_BOARD				
	ret=Write_MaxPress_s1125_pump(MaxPress);	
	if(ret)return ret;
	ret=Write_MinPress_s1125_pump(MinPress);
#endif
	return ret;
}	



