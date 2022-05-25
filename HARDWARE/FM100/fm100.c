#include "fm100.h"
#include "config.h"
#include "uart2.h"
#include "factory_many.h"
#include "drv8434_gpio.h"

Uart_Receive_Data FM100_Read=NULL ;
Uart_Send_Data FM100_Write=NULL ;

int FM100_Scan_Into_Configuration_State(void)
{
	u8 tx_buf[4]="$$$$";
	u8 tmp_buf[4]="@@@@";
	u8 rx_buf[10];
	int len=0;
	int time =40;
	int temp_a=0,temp_b=0;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				LOGD("%d %s \r\n", len, rx_buf);
				break;
		}	
	}
	return len>3?0:1;//strncmp((const char *)tmp_buf, (const char *)rx_buf, 4);
}	

int FM100_Scan_Exit_Configuration_State(void)
{
	u8 tx_buf[4]="%%%%";
	u8 tmp_buf[4]="^^^^";
	u8 rx_buf[128];
	int len=0;
	int time=40;
	int temp_a=0,temp_b=0;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				LOGD("%d %s \r\n", len, rx_buf);
				break;
		}	
	}
	return len>3?0:1;//strncmp((const char *)tmp_buf, (const char *)rx_buf, 4);
}	


int FM100_Scan_Control_Status(bool status)
{
		u8 tx_buf[10]="#99900035;";
	  u8 tx_buf2[10]="#99900036;";
	
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int time=40;
		int temp_a=0,temp_b=0;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	if(status)
			FM100_Write(tx_buf,sizeof(tx_buf));
	else
			FM100_Write(tx_buf2,sizeof(tx_buf2));
	
	while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				LOGD("%d %s \r\n", len, rx_buf);
				break;
		}	
	}
	
	return len>3?0:1;
	
	
	
	
	if(rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return 1;
}	


int FM100_Scan_Continuous_Mode(void)
{
		u8 tx_buf[10]="#99900112;";
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int time=40;
		int temp_a=0,temp_b=0;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	
	if(rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return 1;
}	

int FM100_Scan_Command_Mode(void)
{
		u8 tx_buf[10]="#99900116;";
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int time=40;
		int temp_a=0,temp_b=0;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	
	if(rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return 1;

}	

int FM100_Scan_Command_Mode_Start(void)
{
		u8 tx_buf[10]="#99900035;";
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int time=40;
	int temp_a=0,temp_b=0;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	
	if(rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return 1;

}	
int FM100_Scan_Command_Mode_Stop(void)
{
		u8 tx_buf[10]="#99900036;";
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int time =40;
	int temp_a=0,temp_b=0;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	
	if(rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return 1;

}	

int FM100_Scan_Security(int level)
{

		u8 tx_buf[10]="#99900120;";
	switch(level)
	{
		case 1:
			break;
		case 2:
			memcpy(tx_buf, "#99900121;",sizeof(tx_buf));
			break;
		case 3:
			memcpy(tx_buf, "#99900122;",sizeof(tx_buf));
			break;
		case 4:
			memcpy(tx_buf, "#99900123;",sizeof(tx_buf));
			break;
		default:
			break;
	}	
	
	FM100_Write(tx_buf,sizeof(tx_buf));

	return 0;
}	


int FM100_Scan_Close_Beeper(void)
{
	u8 tx_buf[10]="#99900040;";
	u8 tmp_buf[]="!";
	u8 rx_buf[128];
	int len=0;
	int time=40;
	int temp_a=0,temp_b=0;
	FM100_Write(tx_buf,sizeof(tx_buf));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	
	if(rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return 1;
}	

int FM100_Scan_Open_Beeper(void)
{
	u8 tx_buf[10]="#99900041;";
	u8 rx_buf[128];
	int len=0;
	int time=40;
	u8 tmp_buf[]="!";
	int temp_a=0,temp_b=0;
	FM100_Write(tx_buf,sizeof(tx_buf));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	
	if(rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return 1;
}	

int FM100_Scan_Rs232_Config(void)
{
	u8 tx_buf_baud_rate[]="#99902104;";
	u8 tx_buf_no_flow_control[]="#99902140;";
	u8 tx_buf_data_bit[]="#99902160;";
	int time=40;
	u8 rx_buf[128];
	int len=0;
	int temp_a=0,temp_b=0;
	FM100_Write(tx_buf_baud_rate,sizeof(tx_buf_baud_rate));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
  if(0==len)return 1;
	time=500;
	FM100_Write(tx_buf_no_flow_control,sizeof(tx_buf_no_flow_control));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
  if(0==len)return 1;
	
	time=500;
	FM100_Write(tx_buf_data_bit,sizeof(tx_buf_data_bit));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
  if(0==len)return 1;
	else return 0;
}	


int FM100_Scan_Video_Reverse(bool status)
{
	u8 tx_buf[]="#99900171;";
	u8 rx_buf[128];
	int len=0;
	int time=200;
	int temp_a=0,temp_b=0;
	if(!status)memcpy(tx_buf, "#99900170;",sizeof(tx_buf));
	FM100_Write(tx_buf,sizeof(tx_buf));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	if(0==len)return 1;
	else return 0;
}	

//默认扫码编程，false 切换成代码编程
int FM100_Scan_Code_Programming(bool status)
{
	u8 tx_buf[10]="#99900032;";		
	if(!status)memcpy(tx_buf, "#99900031;",sizeof(tx_buf));
	FM100_Write(tx_buf,sizeof(tx_buf));	
	return 0;
}	



int FM100_Scan_Short_Intervel(void)
{
	u8 tx_buf[10]="#99900151;";
	u8 tx_buf0[10]="#99900000;";
	u8 tx_buf1[10]="#99900001;";
	u8 rx_buf[128];
	int len=0;
	int i;
	int time=200;
	int temp_a=0,temp_b=0;
	FM100_Write(tx_buf,sizeof(tx_buf));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	if(0==len)return 1;
	
	time=200;
	FM100_Write(tx_buf0,sizeof(tx_buf0));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	if(0==len)return 1;
	
	time=200;
	FM100_Write(tx_buf1,sizeof(tx_buf1));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	if(0==len)return 1;
	else return 0;
	
}	
int FM100_Scan_Read_Barcode_Time(void)
{
	u8 tx_buf[10]="#99900150;";
	u8 tx_buf0[10]="#99900001;";
	u8 tx_buf1[10]="#99900005;";
	u8 rx_buf[128];
	int len=0;
	int time=200;
	int temp_a=0,temp_b=0;
	
	FM100_Write(tx_buf,sizeof(tx_buf));
  while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	if(0==len)return 1;
	
	time=200;
	FM100_Write(tx_buf0,sizeof(tx_buf0));
while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	if(0==len)return 1;
	
	time=200;
	FM100_Write(tx_buf1,sizeof(tx_buf1));
  while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(3);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a!=0)
		{	
				FM100_Read(rx_buf,&len);
				break;
		}	
	}
	if(0==len)return 1;
	else return 0;
	
}	
/************************************************/
int Control_Scan_FM100_Beeper(bool status)
{
		int ret=FM100_Scan_Into_Configuration_State();
		if(ret)return ret;
	
	  if(!status){
			ret=FM100_Scan_Close_Beeper();
		}else{	
			ret=FM100_Scan_Open_Beeper();
		}	
		if(ret)return ret;
		
		return FM100_Scan_Exit_Configuration_State();
}	


int FM100_Scan_Goto_Command_Mode(void)
{
	int ret=FM100_Scan_Into_Configuration_State();
	if(ret)return ret;

	ret=FM100_Scan_Command_Mode();
	if(ret)return ret;
	ret=FM100_Scan_Exit_Configuration_State();
	return ret;
}

int Start_Scan_FM100(void)
{
	int ret=0;

		ret=FM100_Scan_Into_Configuration_State();
		if(ret)return ret;
	
	  ret=FM100_Scan_Control_Status(true);
		if(ret)return ret;	
	  
	  ret=FM100_Scan_Exit_Configuration_State();		
	return ret;
}	

int Stop_Scan_FM100(void)
{
	int ret=0;
		ret=FM100_Scan_Into_Configuration_State();
		if(ret)return ret;
	
		ret=FM100_Scan_Control_Status(false);
		if(ret)return ret;	
	  
	  ret=FM100_Scan_Exit_Configuration_State();
	return ret;
}	

void Init_Scan_FM100(bool status)
{
	FM100_Read = GetUartReceive(FM100_UART_PORT,FM100_UART_CS);
	FM100_Write = GetUartSend(FM100_UART_PORT,FM100_UART_CS);

#if USE_AUTOMATIC_INJECTION_BOARD		
	int ret=FM100_Scan_Code_Programming(false);
	if(ret)return ;
	
	//FM100_Scan_Short_Intervel();
	//FM100_Scan_Read_Barcode_Time();
	
  ret=FM100_Scan_Goto_Command_Mode();
	if(ret)return ;
	
	//Start_Scan_FM100();
	ret=Control_Scan_FM100_Beeper(status);
	if(ret)return ;
	
	FM100_Scan_Security(2);
	
	Stop_Scan_FM100();
#endif	
}	

int Obtain_Barcode_String(u8* string,int* length, int TimeOut_S	,bool check)
{
	int ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD	
	u8 buf[128];
#ifndef USE_DRV8434_CAMEL	
	int time=TimeOut_S*10;
#else
	int time=TimeOut_S/4;
#endif	
	int len=0;
	int temp_a=0,temp_b=0;
	FM100_Read = GetUartReceive(FM100_UART_PORT,FM100_UART_CS);
	FM100_Write = GetUartSend(FM100_UART_PORT,FM100_UART_CS);
	delay_ms(10);
	
	ret=Start_Scan_FM100();
	if(ret)return ret;
	memset(buf, 0 ,sizeof(buf));
  LOGD("start \r\n");
	while(time--)
	{	
		temp_a=USART2_RX_CNT;
		delay_ms(10);
		temp_b=USART2_RX_CNT;

		if(temp_a==temp_b && temp_a>3 && FLAG_SCAN_OK)
		{	
				FM100_Read(buf,&len);
				LOGD("%d %s %dms\r\n", len, buf, 10*(TimeOut_S*10-time));
				break;
		}
#ifdef USE_DRV8434_CAMEL
		GP_DRV8434_Motor_Move_Steps_Single_Soft_Stop(M2_MIX,  M2_MIX_LEFT,  2000);
#endif
		//最后200ms停止电机，防止还在解码的过程中切换了命令
	}
	LOGD("end \r\n");
	if(len>2)len-=2;
	else len=0;
	
	memcpy(string, buf, len);
	*length=len;

	ret=Stop_Scan_FM100();
	if(ret)return ret;	
#endif	
	return ret;
}


uint32_t scan_connect_test(void)
{
	uint32_t ret=0;
#if USE_AUTOMATIC_INJECTION_BOARD	
	FM100_Read = GetUartReceive(FM100_UART_PORT,FM100_UART_CS);
	FM100_Write = GetUartSend(FM100_UART_PORT,FM100_UART_CS);
	ret=Control_Scan_FM100_Beeper(true);
	
#endif
	return ret;
}	
