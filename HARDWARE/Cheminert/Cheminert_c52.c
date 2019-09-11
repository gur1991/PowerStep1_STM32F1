#include "Cheminert_c52.h"
#include <string.h>
#include "uart_choose.h" 
#include "config.h"

u8 cheminert_c52_c55_transfer(u8*tx_buf,u8 tx_size,u8*rx_buf, u8*rx_size,int timeout,bool wait_flag,int type_flag)
{
	 u8 ret=0;
		int len;
	 u8 rx_buf_tmp[64];
	 Uart_Receive_Data R232_Read=NULL;
	 Uart_Send_Data R232_Write=NULL;
	
	//printf("timeout %d \r\n",timeout);
	if(type_flag){
			printf("c52\r\n");
			R232_Read = GetUartReceive(C52_UART_PORT, C52_UART_CS);
			R232_Write = GetUartSend(C52_UART_PORT, C52_UART_CS);
	}else{
			printf("c55\r\n");
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
	 
	 while(1){
				//timeout, must return
		 		if(!timeout){
						ret=1;
						printf("cheminert_c52_c55_transfer fuck timeout! \r\n");
						break;
				}
				if(FLAG_UART_CHEMINERT)
				{
							delay_ms(10);
							R232_Read(rx_buf_tmp,&len);
							//rm in the end 0d 0a from buf array
							memcpy(rx_buf,rx_buf_tmp,len-2);
							*rx_size=len-2;
							FLAG_UART_CHEMINERT=0;
							break;
					}
				timeout--;
				delay_ms(1);
		}
		return ret;
}
