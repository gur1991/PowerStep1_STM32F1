#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

//��ʼ��pwm
void TIM3_PWM_Init(u16 arr,u16 psc);

//����ռ�ձȣ��¶ȿ��ƻ���ô˽ӿ�
void TIM_SetTIM3Compare4(u32 compare);

#endif

