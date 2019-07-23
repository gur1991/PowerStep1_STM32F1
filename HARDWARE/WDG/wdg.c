#include "wdg.h"
//#include "stm32f1xx_hal_iwdg.h"
IWDG_HandleTypeDef hiwdg;
//Óë·ÖÆµÊýÎª64,ÖØÔØÖµÎª625,Òç³öÊ±¼äÎª1s	    Tout=64×625/40=1000ms
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

//Î¹¶ÀÁ¢¿´ÃÅ¹·
void IWDG_Feed(void)
{   
 	HAL_IWDG_Refresh(&hiwdg);								   
}


