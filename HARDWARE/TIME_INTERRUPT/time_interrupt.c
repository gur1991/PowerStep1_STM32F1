#include "time_interrupt.h"
#include "temperature.h"
#include "keep_temperature.h"
#include "config.h"
void keep_thermometer_degree(void)
{
#if USE_KEEP_TEMPERATURE_BOARD		
	ThermometerHandle->keep_degree();
#endif	
}


