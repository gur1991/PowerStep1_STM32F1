#ifndef __BABY18_DAD400_H
#define __BABY18_DAD400_H
#include "sys.h"
#include "stdio.h"
#include "uart2.h"
#include "delay.h"
#include "string.h"
typedef struct{
		u8 Baby18_STX;
		u8 Baby18_KEY[3];
		u8 Baby18_ETX;
}BABY18DAD_type_t;

//typedef int baby18_wavelength_on_channels_write_protocol(uint8_t*data,int length);
typedef int(*BabyCall)(uint8_t*data,int length,int timeout);

typedef enum Baby18Call{
		DETECTOR_NAME_READ=66,
		HARDWARE_VERSION_READ, 
		MODEL_READ,
		FIRMWARE_VERSION_READ,
		SERIAL_NUMBER_READ,
	
	
		STATUS_OF_DETECTOR_READ,
		STATUS_OF_DETECTOR_READ_WITH_CHECKSUM,
		BREAK_ERROR_WRITE,
	
		WAVELENGTH_RANGE_READ,
		WAVELENGTH_ON_CHANNELS_READ,
		WAVELENGTH_ON_CHANNELS_WRITE,
		WAVELENGTH_SCAN_RANGE_READ,
		WAVELENGTH_SCAN_RANGE_WRITE,
	
	  ELECTRONIC_BAND_WIDTH_READ,
		ELECTRONIC_BAND_WIDTH_WRITE,
		ELECTRONIC_HALF_WIDTH_READ,
		ELECTRONIC_HALF_WIDTH_WRITE,
	
		SUBSCRIPTION_FREQUENCY_READ,
		SUBSCRIPTION_FREQUENCY_WRITE,

		TIME_CONSTANT_READ,
		TIME_CONSTANT_WRITE,
		
		LAMP_USAGE_PARAMETERS_READ,
		
		BOARD_DIP_SWITCHES_STATE_READ,
	
		AUTOMATIC_IGNITION_STATE_READ,
		AUTOMATIC_IGNITION_STATE_WRITE,
		
		LAMP_STATE_READ,
		LAMP_STATE_WRITE,
		
		GENERATE_SOUND_WRITE,
		
		AUTOZERO_READ,
		AUTOZERO_WRITE,
		
		ABSORBANCE_READ,
		ABSORBANCE_READ_WITH_CHECKSUM,

		INTENSITY_READ,
		INTENSITY_READ_WITH_CHECKSUM,
		
		ABSORBRANCE_SUBSCRIBE_START,
		ABSORBRANCE_SUBSCRIBE_START_WITH_CHECKSUM,
		
		INTENSITY_SUBCRIBE_START,
		INTENSITY_SUBCRIBE_START_WITH_CHECKSUM,
		
		SCAN_ABSORBANCE_READ,
		SCAN_INTENSITY_READ,
		
		ABSORBRANCE_SUBSCRIBE_STOP,
		ABSORBRANCE_SUBSCRIBE_STOP_C,
		
		INTENSITY_SUBCRIBE_STOP,
		INTENSITY_SUBCRIBE_STOP_C,
		
		USER_CALIB_MODE_READ,
		USER_CALIB_MODE_WRITE,
		
		USER_CALIB_PARAMETERS_READ,	
		USER_CALIB_PARAMETERS_WRITE,	
		 
		USER_CALIB_AUTOZERO_READ,
		USER_CALIB_AUTOZERO_WRITE,
		
		CALIB_100_LIGHT_INTENSITY_RUN,
		CALIB_PARAMETRIC_ABSORBANCE_RUN,
		CALIB_STANDARD_ABSORBANCE_WRITE,

		STORE_INTENS_CALIB_VALUE_TO_MEMORY,
		STORE_ABS_CALIB_VALUE_TO_MEMORY,
		
		NEW_LAMP_WRITE,
		UNIT_REPORT_READ,//need long time and log buf
		
}Baby18Call_type_t;

#endif
