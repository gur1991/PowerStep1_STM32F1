#include "fm100.h"
#include "config.h"

Uart_Receive_Data FM100_Read=NULL ;
Uart_Send_Data FM100_Write=NULL ;

int FM100_Scan_Into_Configuration_State(void)
{
	u8 tx_buf[4]="$$$$";
	u8 tmp_buf[4]="@@@@";
	u8 rx_buf[10];
	int len=0;
	int i;
	int time =500;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_INTO)break;
		delay_ms(1);
	}
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_INTO=0;
	if(len==sizeof(tx_buf)&&!strncmp(tmp_buf, rx_buf, sizeof(tx_buf)))
	{
			return 0;
	}
	return -1;
}	

int FM100_Scan_Exit_Configuration_State(void)
{
	u8 tx_buf[4]="%%%%";
	u8 tmp_buf[4]="^^^^";
	u8 rx_buf[10];
	int len=0;
	int i;
	int time=500;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_EXIT)break;
		delay_ms(1);
	}
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_EXIT=0;
	
	if(len==sizeof(tx_buf)&&!strncmp(tmp_buf, rx_buf, sizeof(tx_buf)))
	{
			return 0;
	}
	return -1;
}	


int FM100_Scan_Control_Status(bool status)
{
		u8 tx_buf[10]="#99900035;";
	  u8 tx_buf2[10]="#99900036;";
	
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int i;
		int j=0;
		int time=500;
	
	memset(rx_buf, 0 ,sizeof(rx_buf));
	if(status)
			FM100_Write(tx_buf,sizeof(tx_buf));
	else
			FM100_Write(tx_buf2,sizeof(tx_buf2));
	
	while(time--){
		if(FLAG_UART_FM100_ACK)break;
		delay_ms(1);
	}
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_ACK=0;
	
	if(len==sizeof(tx_buf)&&rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return -1;
}	


int FM100_Scan_Continuous_Mode(void)
{
		u8 tx_buf[10]="#99900112;";
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int i;
		int j=0;
		int time=500;
	
	memset(rx_buf, 0 ,sizeof(rx_buf));
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_ACK)break;
		delay_ms(1);
	}
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_ACK=0;
	
	if(len==sizeof(tx_buf)&&rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return -1;
}	

int FM100_Scan_Command_Mode(void)
{
		u8 tx_buf[10]="#99900116;";
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int i;
		int ret;
		int j=0;
		int time=500;
	
	memset(rx_buf, 0 ,sizeof(rx_buf));
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_ACK)break;
		delay_ms(1);
	}
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_ACK=0;

	if(len==sizeof(tx_buf)&&rx_buf[0]==tmp_buf[0])
	{
			 return 0;
	}
	return -1;

}	

int FM100_Scan_Command_Mode_Start(void)
{
		u8 tx_buf[10]="#99900035;";
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int i;
		int time=500;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_ACK)break;
		delay_ms(1);
	}	
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_ACK=0;
	
	if(len==sizeof(tx_buf)&&rx_buf[0]==tmp_buf[0])
	{
			return 0;
	}
	return -1;

}	
int FM100_Scan_Command_Mode_Stop(void)
{
		u8 tx_buf[10]="#99900036;";
		u8 tmp_buf[]="!";
		u8 rx_buf[128];
		int len=0;
		int i;
		int time =500;
	memset(rx_buf, 0 ,sizeof(rx_buf));
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_ACK)break;
		delay_ms(1);
	}	
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_ACK=0;
	if(len==sizeof(tx_buf)&&rx_buf[0]==tmp_buf[0])
	{
			return 0;
	}
	return -1;

}	

int FM100_Scan_Security(int level)
{
		u8 tx_buf[10]="#99900120;";
	u8 rx_buf[128];
	int len=0;
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
	int time=500;
	
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_ACK)break;
		delay_ms(1);
	}	
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_ACK=0;

	return 0;
}	

int FM100_Scan_Open_Beeper(void)
{
	u8 tx_buf[10]="#99900041;";
	u8 rx_buf[128];
	int len=0;
	int time=500;
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_ACK)break;
		delay_ms(1);
	}	
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_ACK=0;
	return 0;
}	

int FM100_Scan_Rs232_Config(void)
{
	u8 tx_buf_baud_rate[]="#99902104;";
	u8 tx_buf_no_flow_control[]="#99902140;";
	u8 tx_buf_data_bit[]="#99902160;";
	
	FM100_Write(tx_buf_baud_rate,sizeof(tx_buf_baud_rate));
	FM100_Write(tx_buf_no_flow_control,sizeof(tx_buf_no_flow_control));
	FM100_Write(tx_buf_data_bit,sizeof(tx_buf_data_bit));
	USART2_RX_CNT=0;
	return 0;
}	


int FM100_Scan_Video_Reverse(bool status)
{
	u8 tx_buf[]="#99900171;";
	u8 rx_buf[128];
	int len=0;
	int time=500;
	if(!status)memcpy(tx_buf, "#99900170;",sizeof(tx_buf));
	FM100_Write(tx_buf,sizeof(tx_buf));
	while(time--){
		if(FLAG_UART_FM100_ACK)break;
		delay_ms(1);
	}	
	FM100_Read(rx_buf,&len);
	FLAG_UART_FM100_ACK=0;
	return 0;
}	

//Ä¬ÈÏÉ¨Âë±à³Ì£¬false ÇÐ»»³É´úÂë±à³Ì
int FM100_Scan_Code_Programming(bool status)
{
	u8 tx_buf[10]="#99900032;";	
	u8 rx_buf[128];
	int len=0;
	int i;
	
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
	FM100_Write(tx_buf,sizeof(tx_buf));
	FM100_Write(tx_buf0,sizeof(tx_buf0));
	FM100_Write(tx_buf1,sizeof(tx_buf1));

	return 0;

}	
int FM100_Scan_Read_Barcode_Time(void)
{
	u8 tx_buf[10]="#99900150;";
	u8 tx_buf0[10]="#99900001;";
	u8 tx_buf1[10]="#99900005;";
	u8 rx_buf[128];
	int len=0;
	int i;
	FM100_Write(tx_buf,sizeof(tx_buf));
	FM100_Write(tx_buf0,sizeof(tx_buf0));
	FM100_Write(tx_buf1,sizeof(tx_buf1));

	return 0;

}	
/************************************************/
void Control_Scan_FM100_Beeper(bool status)
{
		FM100_Scan_Into_Configuration_State();
		if(!status){
			FM100_Scan_Close_Beeper();
		}else{	
			FM100_Scan_Open_Beeper();
		}	
		FM100_Scan_Exit_Configuration_State();
}	


void FM100_Scan_Goto_Command_Mode(void)
{
		FM100_Scan_Into_Configuration_State();
		FM100_Scan_Command_Mode();
	  FM100_Scan_Exit_Configuration_State();
}

int Start_Scan_FM100(void)
{
	int ret=0;

		ret=FM100_Scan_Into_Configuration_State();
	  FM100_Scan_Control_Status(true);
		ret=FM100_Scan_Exit_Configuration_State();

		
	return ret;
}	

int Stop_Scan_FM100(void)
{
	int ret=0;
	

		ret=FM100_Scan_Into_Configuration_State();
		FM100_Scan_Control_Status(false);
	  FM100_Scan_Exit_Configuration_State();

	
	return ret;
}	

void Init_Scan_FM100(bool status)
{
	FM100_Read = GetUartReceive(FM100_UART_PORT,FM100_UART_CS);
	FM100_Write = GetUartSend(FM100_UART_PORT,FM100_UART_CS);

#if USE_AUTOMATIC_INJECTION_BOARD		
	FM100_Scan_Code_Programming(false);
	
	//FM100_Scan_Short_Intervel();
	//FM100_Scan_Read_Barcode_Time();
	
  FM100_Scan_Goto_Command_Mode();
	Stop_Scan_FM100();
	//Start_Scan_FM100();
	Control_Scan_FM100_Beeper(status);
#endif	
}	

int Obtain_Barcode_String(u8* string,int* length, int TimeOut_S	,bool check)
{
	int ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD	
	static u8 buf[30];
	u8 buf_cmp[30];
	int timeout=TimeOut_S*100;
	int len=0;
	int len_cmp=0;
	int i;
	FM100_Read = GetUartReceive(FM100_UART_PORT,FM100_UART_CS);
	FM100_Write = GetUartSend(FM100_UART_PORT,FM100_UART_CS);
	LOGD("start scan. \r\n");
	Start_Scan_FM100();
	memset(buf, 0 ,sizeof(buf));
	memset(buf_cmp, 0 ,sizeof(buf_cmp));
	while(timeout--){
		if(FLAG_UART_FM100)
		{
				LOGD("scanning ... \r\n");	
				FM100_Read( (u8*)buf ,&len);
				FLAG_UART_FM100=0;
				if(check)
				{	
						if(len==len_cmp&&!strncmp( buf_cmp, buf,len))break;
						memcpy(buf_cmp, buf, len);
						len_cmp=len;
				}else break;
		}
		delay_ms(10);
	}
	if(len>5){
		len=len-2;
		memcpy(string, buf, len);
	}else{
	  len=0;
		Stop_Scan_FM100();
	}
	*length=len;
	
	LOGD("stop scan. \r\n");
	
#endif	
	return ret;
}



