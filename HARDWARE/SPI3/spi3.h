#ifndef __SPI_3_H
#define __SPI_3_H
#include "sys.h"
#include "config.h"


void SPI3_Init(void);
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI3_ReadWriteByte(u8 TxData);

#endif

