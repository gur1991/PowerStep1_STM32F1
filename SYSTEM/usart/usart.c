#include "sys.h"
#include "usart.h"	
#include "uart3.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//Èç¹ûÊ¹ÓÃucos,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼ş¼´¿É.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos Ê¹ÓÃ	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ĞŞ¸ÄÈÕÆÚ:2012/8/18
//°æ±¾£ºV1.5
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ĞŞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ĞŞÕıÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
//V1.4ĞŞ¸ÄËµÃ÷
//1,ĞŞ¸Ä´®¿Ú³õÊ¼»¯IOµÄbug
//2,ĞŞ¸ÄÁËUSART_RX_STA,Ê¹µÃ´®¿Ú×î´ó½ÓÊÕ×Ö½ÚÊıÎª2µÄ14´Î·½
//3,Ôö¼ÓÁËUSART_REC_LEN,ÓÃÓÚ¶¨Òå´®¿Ú×î´óÔÊĞí½ÓÊÕµÄ×Ö½ÚÊı(²»´óÓÚ2µÄ14´Î·½)
//4,ĞŞ¸ÄÁËEN_USART1_RXµÄÊ¹ÄÜ·½Ê½
//V1.5ĞŞ¸ÄËµÃ÷
//1,Ôö¼ÓÁË¶ÔUCOSIIµÄÖ§³Ö
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êı,¶ø²»ĞèÒªÑ¡Ôñuse MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//±ê×¼¿âĞèÒªµÄÖ§³Öº¯Êı                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//ÖØ¶¨Òåfputcº¯Êı 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


#if EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
u8 USART_RX_BUF[USART_REC_LEN];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
u8 USART3_RX_BUF[USART3_REC_LEN]; 


//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓĞĞ§×Ö½ÚÊıÄ¿
u16 USART_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	  
u16 USART_RX_STA_UART3=0; 

u8 aRxBuffer_UART3[RXBUFFERSIZE_UART3];//
u8 aRxBuffer[RXBUFFERSIZE];//HAL¿âÊ¹ÓÃµÄ´®¿Ú½ÓÊÕ»º³å

UART_HandleTypeDef UART1_Handler; //UART¾ä±ú
  
//³õÊ¼»¯IO ´®¿Ú1 
//bound:²¨ÌØÂÊ
void uart_init(u32 bound)
{	
	//UART ³õÊ¼»¯ÉèÖÃ
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //²¨ÌØÂÊ
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //Ò»¸öÍ£Ö¹Î»
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //ÎŞÆæÅ¼Ğ£ÑéÎ»
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //ÎŞÓ²¼şÁ÷¿Ø
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //ÊÕ·¢Ä£Ê½
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()»áÊ¹ÄÜUART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//¸Ãº¯Êı»á¿ªÆô½ÓÊÕÖĞ¶Ï£º±êÖ¾Î»UART_IT_RXNE£¬²¢ÇÒÉèÖÃ½ÓÊÕ»º³åÒÔ¼°½ÓÊÕ»º³å½ÓÊÕ×î´óÊı¾İÁ¿
  
}

//UARTµ×²ã³õÊ¼»¯£¬Ê±ÖÓÊ¹ÄÜ£¬Òı½ÅÅäÖÃ£¬ÖĞ¶ÏÅäÖÃ
//´Ëº¯Êı»á±»HAL_UART_Init()µ÷ÓÃ
//huart:´®¿Ú¾ä±ú

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO¶Ë¿ÚÉèÖÃ
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//Èç¹ûÊÇ´®¿Ú1£¬½øĞĞ´®¿Ú1 MSP³õÊ¼»¯
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//Ê¹ÄÜGPIOAÊ±ÖÓ
		__HAL_RCC_USART1_CLK_ENABLE();			//Ê¹ÄÜUSART1Ê±ÖÓ
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//¸´ÓÃÍÆÍìÊä³ö
		GPIO_Initure.Pull=GPIO_PULLUP;			//ÉÏÀ­
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//¸ßËÙ
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//³õÊ¼»¯PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//Ä£Ê½ÒªÉèÖÃÎª¸´ÓÃÊäÈëÄ£Ê½£¡	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//³õÊ¼»¯PA10
		
#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//Ê¹ÄÜUSART1ÖĞ¶ÏÍ¨µÀ
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//ÇÀÕ¼ÓÅÏÈ¼¶3£¬×ÓÓÅÏÈ¼¶3
#endif	
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	#if 0
	printf("fuck 33\r\n");
	if(huart->Instance==USART3){
		printf("ddsds %d \r\n",aRxBuffer_UART3[0]);
		#if 1
		printf("fuck 44\r\n");
		if((USART_RX_STA_UART3&0x8000)==0)//½ÓÊÕÎ´Íê³É
		{
			printf("fuck 55\r\n");
			if(USART_RX_STA_UART3&0x4000)//½ÓÊÕµ½ÁË0x0d
			{
				printf("fuck 66\r\n");
				if(aRxBuffer_UART3[0]!=0x0a){
						USART_RX_STA_UART3=0;//½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
						printf("fuck 77\r\n");
				}
				else {USART_RX_STA_UART3|=0x8000;	//½ÓÊÕÍê³ÉÁ
						printf("fuck 88\r\n");
					
				}	
			HAL_UART_Receive_IT(&USART3_Handler, (u8 *)aRxBuffer_UART3, RXBUFFERSIZE_UART3);				
			}
			else //»¹Ã»ÊÕµ½0X0D
			{	
				if(aRxBuffer_UART3[0]==0x0d){USART_RX_STA_UART3|=0x4000;printf("fuck 99\r\n");}
				else
				{
					printf("fuck aa\r\n");
					USART3_RX_BUF[USART_RX_STA_UART3&0X3FFF]=aRxBuffer_UART3[0] ;
					
					USART_RX_STA_UART3++;
					HAL_UART_Receive_IT(&USART3_Handler, (u8 *)aRxBuffer_UART3, RXBUFFERSIZE_UART3);
					if(USART_RX_STA_UART3>(USART3_REC_LEN-1))USART_RX_STA_UART3=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
				}		 
			}
		}
		#endif
	}
#endif	
	/**************************************************************************/
	if(huart->Instance==USART1)//Èç¹ûÊÇ´®¿Ú1
	{
		if((USART_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
		{
			if(USART_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
				else USART_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
			}
			else //»¹Ã»ÊÕµ½0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
				}		 
			}
		}

	}
}
 
//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
#if SYSTEM_SUPPORT_OS	 	//Ê¹ÓÃOS
	OSIntEnter();    
#endif
	HAL_UART_IRQHandler(&UART1_Handler);	//µ÷ÓÃHAL¿âÖĞ¶Ï´¦Àí¹«ÓÃº¯Êı
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//µÈ´ı¾ÍĞ÷
	{
	 timeout++;////³¬Ê±´¦Àí
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//Ò»´Î´¦ÀíÍê³ÉÖ®ºó£¬ÖØĞÂ¿ªÆôÖĞ¶Ï²¢ÉèÖÃRxXferCountÎª1
	{
	 timeout++; //³¬Ê±´¦Àí
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//Ê¹ÓÃOS
	OSIntExit();  											 
#endif
} 
#endif	

/*ÏÂÃæ´úÂëÎÒÃÇÖ±½Ó°ÑÖĞ¶Ï¿ØÖÆÂß¼­Ğ´ÔÚÖĞ¶Ï·şÎñº¯ÊıÄÚ²¿¡£*/

//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
//void USART1_IRQHandler(void)                	
//{ 
//	u8 Res;
//	HAL_StatusTypeDef err;
//#if SYSTEM_SUPPORT_OS	 	//Ê¹ÓÃOS
//	OSIntEnter();    
//#endif
//	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //½ÓÊÕÖĞ¶Ï(½ÓÊÕµ½µÄÊı¾İ±ØĞëÊÇ0x0d 0x0a½áÎ²)
//	{
//		Res=USART1->DR; 
//		if((USART_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
//		{
//			if(USART_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
//				else USART_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
//			}
//			else //»¹Ã»ÊÕµ½0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
//				}		 
//			}
//		}   		 
//	}
//	HAL_UART_IRQHandler(&UART1_Handler);	
//#if SYSTEM_SUPPORT_OS	 	//Ê¹ÓÃOS
//	OSIntExit();  											 
//#endif
//} 
//#endif	


