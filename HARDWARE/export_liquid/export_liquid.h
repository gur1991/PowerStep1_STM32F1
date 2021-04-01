#ifndef __EXPORT_LIQUID_H
#define __EXPORT_LIQUID_H
#include <stdio.h>
#include "sys.h"
#include "config.h"
#define	EXPORT_LIQUID 		PEout(5)

void export_liquid_control(uint8_t status);
void Export_Liquid_Control_Init(void);
#endif
