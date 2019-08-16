#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "config.h"
#include "time_interrupt.h"


//初始化pwm
void TIM3_PWM_Init(u16 arr,u16 psc);

//设置占空比，温度控制会调用此接口
void TIM_SetTIM3Compare4(u32 compare);


void TIM5_Init(u16 arr,u16 psc);

#endif

