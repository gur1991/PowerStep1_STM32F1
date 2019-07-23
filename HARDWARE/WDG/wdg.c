#include "wdg.h"
//#include "stm32f1xx_hal_iwdg.h"
IWDG_HandleTypeDef hiwdg;
//���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	    Tout=64�625/40=1000ms
void IWDG_Init(u8 prer,u16 rlr) 
{	
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = prer;
  hiwdg.Init.Reload = rlr;
  HAL_IWDG_Init(&hiwdg);
	
}
void IWDG_Start(void)
{
	HAL_IWDG_Start(&hiwdg);
}

//ι�������Ź�
void IWDG_Feed(void)
{   
 	HAL_IWDG_Refresh(&hiwdg);								   
}


