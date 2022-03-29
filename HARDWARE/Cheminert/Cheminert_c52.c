#include "Cheminert_c52.h"
#include <string.h>
#include "uart_choose.h" 
#include "config.h"

u8 cheminert_c52_c55_transfer(u8*tx_buf,u8 tx_size,u8*rx_buf, u8*rx_size,int timeout,bool wait_flag,int type_flag)
{
	 u8 ret=0;
		int len=0;
	int temp_a=0,temp_b=0;
	
	 u8 rx_buf_tmp[64]={0};
	 Uart_Receive_Data R232_Read=NULL;
	 Uart_Send_Data R232_Write=NULL;
	
	
	//LOGD("start .\r\n");
	if(type_flag){
			//LOGD("c52\r\n");
			R232_Read = GetUartReceive(C52_UART_PORT, C52_UART_CS);
			R232_Write = GetUartSend(C52_UART_PORT, C52_UART_CS);
	}else{
			//LOGD("c55\r\n");
			R232_Read = GetUartReceive(C55_UART_PORT, C55_UART_CS);
			R232_Write = GetUartSend(C55_UART_PORT, C55_UART_CS);
	}
	 R232_Write(tx_buf,tx_size);
		
	 //SDCC SDCW return null,need to add OK	
	 if(wait_flag==false){
				rx_buf_tmp[0]='O';
		    rx_buf_tmp[1]='K';
				memcpy(rx_buf,rx_buf_tmp,2);
				*rx_size=2;
				return ret;
	 } 	

	 timeout=800;
	 
	 while(1){
				temp_a=USART2_RX_CNT;
				delay_ms(1);
				temp_b=USART2_RX_CNT;
			
				if(temp_a==temp_b && temp_a!=0)
				{
							delay_ms(1);
							R232_Read(rx_buf_tmp,&len);
							//rm in the end 0d 0a from buf array
							if(len){
								//memcpy(rx_buf,rx_buf_tmp,len-2);
								//LOGD("c55 c52 :%c \r\n",rx_buf[0]);
								//*rx_size=len-2;
								memcpy(rx_buf,rx_buf_tmp,len);	
							}
							*rx_size=len;	
							break;
					}
			
				if(!timeout--){
					ret=1;
					LOGE("cheminert_c52_c55_transfer fuck timeout! \r\n");
					*rx_size=0;
					break;
				}
		}

		//LOGD("end .\r\n");
		return ret;
}
