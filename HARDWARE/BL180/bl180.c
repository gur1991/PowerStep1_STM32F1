#include "bl180.h"
#include "config.h"

#define SCAN_DEBUG 1

Uart_Receive_Data BL180_Read=NULL ;
Uart_Send_Data BL180_Write=NULL ;


Scan_t* ScanHandle=0;

Scan_t bl180=
{
	Init_BL180,
	Scan_Bar_Action
};

Scan_t* BL180_GetHandle(void)
{
  return (&bl180);
}


Scan_t fm100=
{
	Init_Scan_FM100,
	Obtain_Barcode_String,
};

Scan_t* FM100_GetHandle(void)
{
  return (&fm100);
}

void ScanChooseHandle(SCAN_type id)
{

  if (id == FM100)
  {
    ScanHandle = FM100_GetHandle();
	}else if(id == BL180)
	{
		ScanHandle = BL180_GetHandle();
	}	
}






void Config_BL80_Transfer(void)
{
	BL180_Read = GetUartReceive(BL180_UART_PORT,BL180_UART_CS);
	BL180_Write = GetUartSend(BL180_UART_PORT,BL180_UART_CS);
	
}	
void Wait_Ack(void)
{
	delay_ms(100);
}	


void Goto_Rest_Set(void)
{
	u8 tx_buf[6]="DELT\r\n";
	u8 rx_buf[128];
	int len=0;	
	
	BL180_Write(tx_buf,sizeof(tx_buf));
	Wait_Ack();
	BL180_Read(rx_buf,&len);
}	





void Goto_Setting_Mode(void)
{
	u8 tx_buf[6]="SSET\r\n";
	u8 rx_buf[128];
	int len=0;	
	int i=100;
	
	BL180_Write(tx_buf,sizeof(tx_buf));
	Wait_Ack();
	BL180_Read(rx_buf,&len);
#if(SCAN_DEBUG)	
	for(i=0;i<len;i++)
	{
		LOGD("%c",rx_buf[i]);
	}
	LOGD("\r\n");
#endif
}	

void Goto_Rest_Mode(void)
{
	Config_BL80_Transfer();
	//Goto_Setting_Mode();
	Goto_Rest_Set();
}	

void Exit_Setting_Mode(void)
{
	u8 tx_buf[6]="SEND\r\n";
	u8 rx_buf[128];
	int len=0;	
	int i;
	
	BL180_Write(tx_buf,sizeof(tx_buf));
	Wait_Ack();
	BL180_Read(rx_buf,&len);
#if(SCAN_DEBUG)		
	for(i=0;i<len;i++)
	{
		LOGD("%c",rx_buf[i]);
	}
	LOGD("\r\n");
#endif
}	

void Save_Setting_Commond(void)
{
	u8 tx_buf[6]="SAVE\r\n";
	u8 rx_buf[128];
	int len=0;	
	int i;
	
	BL180_Write(tx_buf,sizeof(tx_buf));
	Wait_Ack();
	BL180_Read(rx_buf,&len);
#if(SCAN_DEBUG)		
	for(i=0;i<len;i++)
	{
		LOGD("%c",rx_buf[i]);
	}
	LOGD("\r\n");
#endif
}	

void Set_Data_Bit(void)
{
	u8 tx_buf[7]="WP300\r\n";
	u8 rx_buf[128];
	int len=0;	
	int i;
	
	BL180_Write(tx_buf,sizeof(tx_buf));
	Wait_Ack();
	BL180_Read(rx_buf,&len);
#if(SCAN_DEBUG)		
	for(i=0;i<len;i++)
	{
		LOGD("%c",rx_buf[i]);
	}
	LOGD("\r\n");
#endif
}	

void Set_Check_Bit(void)
{
	u8 tx_buf[7]="WP310\r\n";
	u8 rx_buf[128];
	int len=0;	
	int i;
	
	BL180_Write(tx_buf,sizeof(tx_buf));
	Wait_Ack();
	BL180_Read(rx_buf,&len);
#if(SCAN_DEBUG)		
	for(i=0;i<len;i++)
	{
		LOGD("%c",rx_buf[i]);
	}
	LOGD("\r\n");
#endif
}	

	


void Start_BL180(void)
{
	u8 tx_buf[5];
	tx_buf[0]='L';
	tx_buf[1]='O';
	tx_buf[2]='N';
	tx_buf[3]='\r';
	tx_buf[4]='\n';
	Config_BL80_Transfer();
	BL180_Write(tx_buf,5);
	Wait_Ack();
#if(SCAN_DEBUG)		
	LOGD("SCAN START\r\n");	
#endif
}	


void End_BL180(void)
{
	u8 tx_buf[6];
	tx_buf[0]='L';
	tx_buf[1]='O';
	tx_buf[2]='F';
	tx_buf[3]='F';
	tx_buf[4]='\r';
	tx_buf[5]='\n';
	u8 rx_buf[128];
	int len=0;	
	int i;

	Config_BL80_Transfer();
	BL180_Write(tx_buf,sizeof(tx_buf));
	Wait_Ack();
	BL180_Read(rx_buf,&len);
		
#if(SCAN_DEBUG)	
	for(i=0;i<len;i++)
	{
		LOGD("%c",rx_buf[i]);
	
	}
	LOGD("\r\n");

	LOGD("SCAN END\r\n");
#endif
}	


int Scan_Bar_Action(u8* string,int* length, int TimeOut_S,bool check)
{
	int ret=0;
#if USE_AUTOMATIC_INJECTION_BOARD			
	u8 rx_buf[128];
	int len=0;
	int i= TimeOut_S*100;
#if(SCAN_DEBUG)		
	LOGD("start scan \r\n");
#endif
	FLAG_RECEIVE_ANSOWER_BL180=0;
	USART2_RX_CNT=0;
	Load_BL180_Uart_Config();
	//Wait_Ack();
	
	Start_BL180();
	while(i--){
			if(FLAG_RECEIVE_ANSOWER_BL180){
					FLAG_RECEIVE_ANSOWER_BL180=0;
					BL180_Read(rx_buf,&len);
					break;
			}	
			delay_ms(10);
	}
	
	if(len)
	{
		memcpy(string, rx_buf, len);
		*length=len-1;
	}else{
		*length=0;
		ret=-1;
		End_BL180();
		FLAG_RECEIVE_ANSOWER_BL180=0;
		USART2_RX_CNT=0;
	}
#if(SCAN_DEBUG)		
	LOGD("end scan \r\n");
#endif

	Exit_BL180_Uart_Config();
	
#endif	
	return ret;
}
void Init_BL180(bool status)
{
	
#if USE_AUTOMATIC_INJECTION_BOARD			
	Load_BL180_Uart_Config();
	Config_BL80_Transfer();
	End_BL180();
	Exit_BL180_Uart_Config();
#endif
	
/*	
	Goto_Setting_Mode();
	
	Set_Data_Bit();
	Set_Check_Bit();

	Save_Setting_Commond();
	
	Exit_Setting_Mode();
*/
}







