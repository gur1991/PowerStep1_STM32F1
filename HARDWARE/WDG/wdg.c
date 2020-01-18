#include "wdg.h"
#include "config.h"
//#include "stm32f1xx_hal_iwdg.h"
IWDG_HandleTypeDef hiwdg;
//与分频数为64,重载值为625,溢出时间为1s	    Tout=64�625/40=1000ms
void IWDG_Init(uint32_t prer,uint32_t rlr) 
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

//喂独立看门狗
void IWDG_Feed(void)
{   
 	HAL_IWDG_Refresh(&hiwdg);								   
}


