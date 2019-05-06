#include "light.h"
#include "delay.h"

//������ʼ������
void Light_Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOEʱ��

    GPIO_Initure.Pin=GPIO_PIN_5;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
       
}

//��д�����
//�����ź�ֵ
//����ʹ�ú�ֵ
u8 Light_Sensor_Get(u8 number)
{	
	u8 status=2;
	switch(number)
	{
		case 0:
				status=LIGHT_VALUE0;
				break;
		default:
				break;
	}	
    return status; 
}