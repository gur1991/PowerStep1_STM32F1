#include "time_interrupt.h"
#include "temperature.h"
#include "keep_temperature.h"

void keep_thermometer_degree(void)
{
	ThermometerHandle->keep_degree();
}


