

#include "uart3.h"
#include "tm_reader.h"
#include <string.h>
//#include "STM32F10x.h"
#include "delay.h"

typedef struct MessageBuf {
	u8 gMessage[260];
	int length;
	int currentPos;
}MessageBuf_t;

static MessageBuf_t Msg;



/****************** Stub implementation of serial transport layer routines. ****************/  
static TMR_Status
s_open(TMR_SR_SerialTransport *this)
{
	
    return TMR_SUCCESS;
}

static TMR_Status
s_sendBytes(TMR_SR_SerialTransport *this, uint32_t length, 
                uint8_t* message, const uint32_t timeoutMs)
{
	
		int i=0;
	UART3_Send_Data(message,length);
	
	printf("send length . \r\n");
	/*
	for(i=0;i<length;i++)
	{
		printf("0x%x ",message[i]);
	}
	printf("\r\n");
	*/
	//	
	//message+=length;
	//length=0;
  
	return TMR_SUCCESS;
}




static TMR_Status
s_receiveBytes(TMR_SR_SerialTransport *this, uint32_t length,
uint32_t* messageLength, uint8_t* message, const uint32_t timeoutMs)
{
  int index = 0;
	u8  tmp_buf[260];
		int i=0;
	memset(tmp_buf,0,sizeof(tmp_buf));
	
	 printf("s_receiveBytes start.\r\n");

				while(1)
				{	

					//delay_ms(5);	
					if(length<=Msg.length)break;
			
					while(1){
						//if(UART3_RX_CNT>=length)break;
						if(FLAG_RECEIVE_RFID)
						{
							FLAG_RECEIVE_RFID=0;
							break;
						}
						delay_ms(5);
					}
				
					UART3_Receive_Data(tmp_buf,&index);
					if(index){
							memcpy(Msg.gMessage+Msg.length, tmp_buf, index);		
							Msg.length+=index;
					}
			}
	
		memcpy(message, Msg.gMessage+Msg.currentPos, length);
		Msg.currentPos+=length;
		Msg.length-=length;
		
		if(!Msg.length)Msg.currentPos=0;
		/*
		for(i=0;i<length;i++)
		{
			printf("0x%x ",message[i]);
		}
		printf("\r\n");
		*/		
		message+=length;
		*messageLength=length;
		
		printf("s_receiveBytes end. \r\n");
	return TMR_SUCCESS;
		
}


static TMR_Status
s_setBaudRate(TMR_SR_SerialTransport *this, uint32_t rate)
{
  return TMR_SUCCESS;
}

static TMR_Status
s_shutdown(TMR_SR_SerialTransport *this)
{
	return TMR_SUCCESS;
}

static TMR_Status
s_flush(TMR_SR_SerialTransport *this)
{
  return TMR_SUCCESS;
}

TMR_Status
TMR_SR_SerialTransportNativeInit(TMR_SR_SerialTransport *transport,
                                 TMR_SR_SerialPortNativeContext *context,
                                 const char *device)
{

	//printf("SerialTransportNativeInit.\r\n");
	
	memset(&Msg,0,sizeof(Msg));
	
  transport->cookie = context;
  transport->open = s_open;
  transport->sendBytes = s_sendBytes;
  transport->receiveBytes = s_receiveBytes;
  transport->setBaudRate = s_setBaudRate;
  transport->shutdown = s_shutdown;
  transport->flush = s_flush;

return s_open(transport);

}

