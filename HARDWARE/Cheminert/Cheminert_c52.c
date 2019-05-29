#include "Cheminert_c52.h"

u8 cheminert_c52_c55_transfer(u8*tx_buf,u8 tx_size,u8*rx_buf, u8*rx_size,uint16_t timeout,bool wait_flag){
	 u8 ret=0,i;
		int len;
	 u8 rx_buf_tmp[64];
	
	 RS485_Send_Data(tx_buf,tx_size);
		
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
						printf(" cheminert_c52_transfer fuck timeout! \r\n");
						break;
				}
				if(FLAG_UART_MASTER){	
							RS485_Receive_Data(rx_buf_tmp,&len);
							//rm in the end 0d 0a from buf array
							memcpy(rx_buf,rx_buf_tmp,len-2);
							*rx_size=len-2;
							FLAG_UART_MASTER=0;
							break;
					}
				timeout--;
				delay_ms(10);
		}
		return ret;
}