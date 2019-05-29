#include "baby18dad400.h"

static uint8_t baby18_request[24];
static uint8_t baby18_reponse[128];

static uint8_t KEY_BABY18[3];
static uint8_t DATA_BABY18[16];


/*
*return -1 fail   0 ok
*key[3]
*data[data_length]
*rx_len all response 
*rx_data--->baby18_reponse[3]....baby18_reponse[rx_data-1] 
*TIMEOUT_MS  ms
*/

static int baby18_transfer(uint8_t *key,uint8_t *data,int data_length,int*rx_len,int TIMEOUT_MS){
		int tlen=0,rlen=0;
	
		memset(baby18_request,0,sizeof(baby18_request));
		memset(baby18_reponse,0,sizeof(baby18_reponse));
		baby18_request[0]='#';
		memcpy(baby18_request+1,key,3);//len 3
		if(data_length>0){
				memcpy(baby18_request+4,data,data_length);
		}
		baby18_request[4+data_length]=0x0d;
		tlen=5+data_length;
		RS485_Send_Data(baby18_request,tlen);
		delay_ms(TIMEOUT_MS);
		RS485_Receive_Data(baby18_reponse,&rlen);
		if(rlen>=5&&(baby18_reponse[rlen-1]==0x0d||baby18_reponse[rlen-1]==0x0a)){
				if(strncmp(baby18_reponse,baby18_request+1,3)){
						return -1;
				}else{
						*rx_len=rlen;
						return 0;
				}
		}else{
						return -1;
		}			
}
/******************************** no use this API***********************/
//DTr
int baby18_detector_name_read_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='D';
		KEY_BABY18[1]='T';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}
//HWr 
int baby18_hardware_version_read_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='H';
		KEY_BABY18[1]='W';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}

//MDr 
int baby18_model_read_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='M';
		KEY_BABY18[1]='D';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}

//SWr 
int baby18_firmware_version_read_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='S';
		KEY_BABY18[1]='W';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}
//SNr 
int baby18_serial_number_read_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='S';
		KEY_BABY18[1]='N';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}
//STr 
int baby18_status_of_detector_read_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='S';
		KEY_BABY18[1]='T';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}


//SXr
int baby18_status_of_detector_read_with_checksum_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='S';
		KEY_BABY18[1]='X';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}

//BEw
int baby18_break_errors_write_with_checksum_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='B';
		KEY_BABY18[1]='E';
		KEY_BABY18[2]='w';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}

//WRr
int baby18_wavelength_range_read_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='W';
		KEY_BABY18[1]='R';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}


//WLr
int baby18_wavelength_on_channels_read_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='W';
		KEY_BABY18[1]='L';
		KEY_BABY18[2]='r';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}



//WLwAnnnBnnn
int baby18_wavelength_on_channels_write_protocol(uint8_t*data,int length,int timeout){	
		int ret=0;
		int len,i;
		KEY_BABY18[0]='W';
		KEY_BABY18[1]='L';
		KEY_BABY18[2]='w';
		memcpy(DATA_BABY18,data,length);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,length,&len,timeout);
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}


/******************************this no use******************************************/
static BabyCall GetOperationFunction(Baby18Call_type_t TYPE){
		
		switch(TYPE){
			case DETECTOR_NAME_READ:
					KEY_BABY18[0]='D';
					KEY_BABY18[1]='T';
					KEY_BABY18[2]='r';
					return baby18_detector_name_read_protocol;
			
			case HARDWARE_VERSION_READ:return baby18_hardware_version_read_protocol;
			case MODEL_READ:return baby18_model_read_protocol;
		}
}
/***********************new world*******************************/
					//KEY_BABY18[0]='D';
					//KEY_BABY18[1]='T';
					//KEY_BABY18[2]='r';
static int Baby_Choose_Key(Baby18Call_type_t TYPE){
		switch(TYPE){
			case DETECTOR_NAME_READ:
					memcpy(KEY_BABY18,"DTr",3);
					break;
			case HARDWARE_VERSION_READ:
					memcpy(KEY_BABY18,"HWr",3);
					break;
			case MODEL_READ:
					memcpy(KEY_BABY18,"MDr",3);
					break;
			case FIRMWARE_VERSION_READ:
					memcpy(KEY_BABY18,"SWr",3);
					break;
			case SERIAL_NUMBER_READ:
					memcpy(KEY_BABY18,"SNr",3);
					break;
			case STATUS_OF_DETECTOR_READ:
					memcpy(KEY_BABY18,"STr",3);
					break;
			case STATUS_OF_DETECTOR_READ_WITH_CHECKSUM:
					memcpy(KEY_BABY18,"SXr",3);
					break;
			case BREAK_ERROR_WRITE:
					memcpy(KEY_BABY18,"BEw",3);
					break;
			case WAVELENGTH_RANGE_READ:
					memcpy(KEY_BABY18,"WRr",3);
					break;
			case WAVELENGTH_ON_CHANNELS_READ:
					memcpy(KEY_BABY18,"WLr",3);
					break;
			case WAVELENGTH_ON_CHANNELS_WRITE:
					memcpy(KEY_BABY18,"WLw",3);
					break;
			case WAVELENGTH_SCAN_RANGE_READ:
					memcpy(KEY_BABY18,"WSr",3);
					break;
			case WAVELENGTH_SCAN_RANGE_WRITE:
					memcpy(KEY_BABY18,"WSw",3);
					break;
			case ELECTRONIC_BAND_WIDTH_READ:
					memcpy(KEY_BABY18,"BWr",3);
					break;
			case ELECTRONIC_BAND_WIDTH_WRITE:
					memcpy(KEY_BABY18,"BWw",3);
					break;
			case ELECTRONIC_HALF_WIDTH_READ:
					memcpy(KEY_BABY18,"HFr",3);
					break;
			case ELECTRONIC_HALF_WIDTH_WRITE:
					memcpy(KEY_BABY18,"HFw",3);
					break;
			case SUBSCRIPTION_FREQUENCY_READ:
					memcpy(KEY_BABY18,"SFr",3);
					break;
			case SUBSCRIPTION_FREQUENCY_WRITE:
					memcpy(KEY_BABY18,"SFw",3);
					break;
			case TIME_CONSTANT_READ:
					memcpy(KEY_BABY18,"TCr",3);
					break;
			case TIME_CONSTANT_WRITE:
					memcpy(KEY_BABY18,"TCw",3);
					break;
			case LAMP_USAGE_PARAMETERS_READ:
					memcpy(KEY_BABY18,"LLr",3);
					break;
			case BOARD_DIP_SWITCHES_STATE_READ:
					memcpy(KEY_BABY18,"BSr",3);
					break;
			case AUTOMATIC_IGNITION_STATE_READ:
					memcpy(KEY_BABY18,"ALr",3);
					break;
			case AUTOMATIC_IGNITION_STATE_WRITE:
					memcpy(KEY_BABY18,"ALw",3);
					break;
			case LAMP_STATE_READ:
					memcpy(KEY_BABY18,"LPr",3);
					break;
			case LAMP_STATE_WRITE:
					memcpy(KEY_BABY18,"LPw",3);
					break;
			case GENERATE_SOUND_WRITE:
					memcpy(KEY_BABY18,"SGw",3);
					break;
			case AUTOZERO_READ:
					memcpy(KEY_BABY18,"ZRr",3);
					break;
			case AUTOZERO_WRITE:
					memcpy(KEY_BABY18,"ZRw",3);
					break;	
			case ABSORBANCE_READ:
					memcpy(KEY_BABY18,"ABr",3);
					break;
			case ABSORBANCE_READ_WITH_CHECKSUM:
					memcpy(KEY_BABY18,"AXr",3);
					break;
			case INTENSITY_READ:
					memcpy(KEY_BABY18,"ITr",3);
					break;
			case INTENSITY_READ_WITH_CHECKSUM:
					memcpy(KEY_BABY18,"IXr",3);
					break;
			case ABSORBRANCE_SUBSCRIBE_START:
					memcpy(KEY_BABY18,"ABs",3);
					break;
			case ABSORBRANCE_SUBSCRIBE_START_WITH_CHECKSUM:
					memcpy(KEY_BABY18,"AXs",3);
					break;
			case INTENSITY_SUBCRIBE_START:
					memcpy(KEY_BABY18,"ITs",3);
					break;
			case INTENSITY_SUBCRIBE_START_WITH_CHECKSUM:
					memcpy(KEY_BABY18,"IXs",3);
					break;
			case SCAN_ABSORBANCE_READ:
					memcpy(KEY_BABY18,"SAr",3);
					break;
			case SCAN_INTENSITY_READ:
					memcpy(KEY_BABY18,"SIr",3);
					break;
			case ABSORBRANCE_SUBSCRIBE_STOP:
					memcpy(KEY_BABY18,"ABu",3);
					break;
			case ABSORBRANCE_SUBSCRIBE_STOP_C:
					memcpy(KEY_BABY18,"AXu",3);
					break;
			case INTENSITY_SUBCRIBE_STOP:
					memcpy(KEY_BABY18,"ITu",3);
					break;
			case INTENSITY_SUBCRIBE_STOP_C:
					memcpy(KEY_BABY18,"IXu",3);
					break;
			case USER_CALIB_MODE_READ:
					memcpy(KEY_BABY18,"MUr",3);
					break;
			case USER_CALIB_MODE_WRITE:
					memcpy(KEY_BABY18,"MUw",3);
					break;
			case USER_CALIB_PARAMETERS_READ:
					memcpy(KEY_BABY18,"CPr",3);
					break;	
			case USER_CALIB_PARAMETERS_WRITE:
					memcpy(KEY_BABY18,"CPw",3);
					break;
			case USER_CALIB_AUTOZERO_READ:
					memcpy(KEY_BABY18,"CZr",3);
					break;
			case USER_CALIB_AUTOZERO_WRITE:
					memcpy(KEY_BABY18,"CZw",3);
					break;
			case CALIB_100_LIGHT_INTENSITY_RUN:
					memcpy(KEY_BABY18,"CIw",3);
					break;
			case CALIB_PARAMETRIC_ABSORBANCE_RUN:
					memcpy(KEY_BABY18,"CAw",3);
					break;
			case CALIB_STANDARD_ABSORBANCE_WRITE:
					memcpy(KEY_BABY18,"CSw",3);
					break;
			case STORE_INTENS_CALIB_VALUE_TO_MEMORY:
					memcpy(KEY_BABY18,"RIw",3);
					break;
			case STORE_ABS_CALIB_VALUE_TO_MEMORY:
					memcpy(KEY_BABY18,"RAw",3);
					break;
			case NEW_LAMP_WRITE:
					memcpy(KEY_BABY18,"CHw",3);
					break;	
			case UNIT_REPORT_READ:
					memcpy(KEY_BABY18,"CRr",3);
					break;
			default:
					printf("no this command !\r\n");
					return -1;
		}
		return 0;
}

static int Baby_Run(uint8_t*tx_buf,int tx_len,int*rx_len,int timeout){
		int ret=0;
		int len,i;
		memcpy(DATA_BABY18,tx_buf,tx_len);
		ret=baby18_transfer(KEY_BABY18,DATA_BABY18,tx_len,&len,timeout);
		*rx_len=len;
		return ret;
}

/*
*this function is interface to need
*
*/
int Baby18dad_Execute_Interface(Baby18Call_type_t TYPE,uint8_t*tx_data,int tx_len,int *rx_len,int timeout){
		int ret,i;
		int len=0;
		//static uint8_t tmpbuf[128];
		//memcpy(tmpbuf,tx_data,tx_len);
	
		Baby_Choose_Key(TYPE);
		ret=Baby_Run(tx_data,tx_len,&len,timeout);
		*rx_len=len;
	
		if(ret){	
				printf("baby18_transfer fail ! \r\n");
		}else{
				for(i=0;i<len-3;i++){
						printf("%c",baby18_reponse[3+i]);
				}
				printf("\r\n");
		}
		return ret;
}



