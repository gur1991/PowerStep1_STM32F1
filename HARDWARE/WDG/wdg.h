#ifndef __WDG_H
#define __WDG_H
#include "sys.h"
void IWDG_Init(uint32_t prer,uint32_t rlr);
void IWDG_Feed(void);
void IWDG_Start(void);

#endif
