/** 
  ******************************************************************************
  * @file    x_nucleo_ihm03a1_stm32f4xx.h
  * @author  IPC Rennes
  * @version V1.6.0
  * @date    January 25th, 2016
  * @brief   Header for BSP driver for x-nucleo-ihm03a1 Nucleo extension board 
  *  (based on powerSTEP01)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef X_NUCLEO_IHM03A1_STM32F1XX_H
#define X_NUCLEO_IHM03A1_STM32F1XX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_nucleo.h"
#include "x_nucleo_ihm03a1_stm32f1xx.h" 
//#define BSP_MOTOR_CONTROL_BOARD_USE_SPI2	 

	 
	 /** @addtogroup BSP
  * @{
  */   
   
/** @addtogroup X_NUCLEO_IHM03A1_STM32F4XX
  * @{   
  */   
   
/* Exported Constants --------------------------------------------------------*/
   
/** @defgroup IHM03A1_Exported_Constants Exported Constants
  * @{
  */   
   
/******************************************************************************/
/* USE_STM32F4XX_NUCLEO                                                       */
/******************************************************************************/

 /** @defgroup Constants_For_STM32F4XX_NUCLEO Constants For STM32F4XX_NUCLEO
* @{
*/ 
void Powerstep01_Board_Reset_All(void);
void Powerstep01_Board_ReleaseReset_All(void);

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}GPIO_VALUE_type;

typedef struct {
		GPIO_VALUE_type rst;
		GPIO_VALUE_type cs;
}powerStep01_select_type_t;

void PowerStep_Select_Motor_Baby(int chip);

extern GPIO_TypeDef* BSP_MOTOR_CONTROL_BOARD_CS_PORT;
extern uint16_t BSP_MOTOR_CONTROL_BOARD_CS_PIN;

extern GPIO_TypeDef* BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT;
extern uint16_t BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN;


/// Interrupt line used for Powerstep01 Busy
//#define BUSY_EXTI_LINE_IRQn           (EXTI3_IRQn)

/// Interrupt line used for Powerstep01 Flag
//#define FLAG_EXTI_LINE_IRQn           (EXTI2_IRQn)//�˴���غ�GPIOһ�£������¿���
/// Interrupt line used for Powerstep01 Busy

#define BUSY_EXTI_LINE_IRQn1           (EXTI3_IRQn)

/// Interrupt line used for Powerstep01 Flag
#define FLAG_EXTI_LINE_IRQn1           (EXTI2_IRQn)//�˴���غ�GPIOһ�£������¿���
/// Interrupt line used for Powerstep01 Busy
#define BUSY_EXTI_LINE_IRQn2           (EXTI9_5_IRQn)

/// Interrupt line used for Powerstep01 Flag
#define FLAG_EXTI_LINE_IRQn2           (EXTI9_5_IRQn)//�˴���غ�GPIOһ�£������¿���






//**************************************no-useful start**********************************/

/// Timer used for the step clock
#define BSP_MOTOR_CONTROL_BOARD_TIMER_STEP_CLOCK      (TIM3)

/// Channel Timer used for the step clock
#define BSP_MOTOR_CONTROL_BOARD_CHAN_TIMER_STEP_CLOCK      (TIM_CHANNEL_2)

/// HAL Active Channel Timer used for the step clock
#define BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_STEP_CLOCK      (HAL_TIM_ACTIVE_CHANNEL_2)

/// Timer Clock Enable for the step clock
#define __BSP_MOTOR_CONTROL_BOARD_STEP_CLOCK_CLCK_ENABLE()  __TIM3_CLK_ENABLE()

/// Timer Clock Disable for the step clock
#define __BSP_MOTOR_CONTROL_BOARD_STEP_CLOCK_CLCK_DISABLE()  __TIM3_CLK_DISABLE()

/// Step Clock global interrupt
#define BSP_MOTOR_CONTROL_BOARD_STEP_CLOCK_IRQn   (TIM3_IRQn)

/// Step Clock GPIO alternate function 
#define BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_STEP_CLOCK  (GPIO_AF2_TIM3)
//**************************************no-useful end**********************************/





#ifndef BSP_MOTOR_CONTROL_BOARD_USE_SPI2
/// SPI SCK AF
#define BSP_MOTOR_CONTROL_BOARD_SPIx_SCK_AF    (GPIO_AF5_SPI1)
#else /* #ifndef BSP_MOTOR_CONTROL_BOARD_USE_SPI2 */
/// SPI SCK AF
#define BSP_MOTOR_CONTROL_BOARD_SPIx_SCK_AF    (GPIO_AF5_SPI2)
#endif /* #ifndef BSP_MOTOR_CONTROL_BOARD_USE_SPI2 */  
   
 /**
* @}
*/

/******************************************************************************/
/* Independent plateform definitions                                          */
/******************************************************************************/

   /** @defgroup Constants_For_All_Nucleo_Platforms Constants For All Nucleo Platforms
* @{
*/   

/// GPIO Pin used for the Powerstep01 busy pin
#define BSP_MOTOR_CONTROL_BOARD_BUSY_PIN   (GPIO_PIN_3)
/// GPIO port used for the Powerstep01 busy pin
#define BSP_MOTOR_CONTROL_BOARD_BUSY_PORT   (GPIOD)
/// GPIO Pin used for the Powerstep01 busy pin
#define BSP_MOTOR_CONTROL_BOARD_BUSY_PIN2   (GPIO_PIN_7)
/// GPIO port used for the Powerstep01 busy pin
#define BSP_MOTOR_CONTROL_BOARD_BUSY_PORT2   (GPIOD)














/// GPIO Pin used for the Powerstep01 flag pin
#define BSP_MOTOR_CONTROL_BOARD_FLAG_PIN   (GPIO_PIN_2)
/// GPIO port used for the Powerstep01 flag pin
#define BSP_MOTOR_CONTROL_BOARD_FLAG_PORT   (GPIOD)
/// GPIO Pin used for the Powerstep01 flag pin
#define BSP_MOTOR_CONTROL_BOARD_FLAG_PIN2   (GPIO_PIN_6)
/// GPIO port used for the Powerstep01 flag pin
#define BSP_MOTOR_CONTROL_BOARD_FLAG_PORT2   (GPIOD)














/*
*discard for step clk
*/
/// GPIO Pin used for the Powerstep01 step clock pin
#define BSP_MOTOR_CONTROL_BOARD_STEP_CLOCK_PIN  (GPIO_PIN_5)
/// GPIO Port used for the Powerstep01 step clock
#define BSP_MOTOR_CONTROL_BOARD_STEP_CLOCK_PORT  (GPIOB)




/// GPIO Pin used for the Powerstep01 reset pin
//#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN  (GPIO_PIN_1)
/// GPIO port used for the Powerstep01 reset pin
//#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT (GPIOD)

// GPIO Pin used for the Powerstep01 reset pin1
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN1  (GPIO_PIN_1)
/// GPIO port used for the Powerstep01 reset pin1
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT1 (GPIOD)
/// GPIO Pin used for the Powerstep01 reset pin2
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN2  (GPIO_PIN_5)
/// GPIO port used for the Powerstep01 reset pin2
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT2 (GPIOD)

/// GPIO Pin used for the Powerstep01 reset pin3
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN3  (GPIO_PIN_10)
/// GPIO port used for the Powerstep01 reset pin3
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT3 (GPIOG)
/// GPIO Pin used for the Powerstep01 reset pin4
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN4  (GPIO_PIN_14)
/// GPIO port used for the Powerstep01 reset pin4
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT4 (GPIOG)


/// GPIO Pin used for the Powerstep01 reset pin5
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN5  (GPIO_PIN_2)
/// GPIO port used for the Powerstep01 reset pin5
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT5 (GPIOE)
/// GPIO Pin used for the Powerstep01 reset pin6
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN6  (GPIO_PIN_6)
/// GPIO port used for the Powerstep01 reset pin6
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT6 (GPIOE)

/// GPIO Pin used for the Powerstep01 reset pin7
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN7  (GPIO_PIN_15)
/// GPIO port used for the Powerstep01 reset pin7
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT7 (GPIOC)
/// GPIO Pin used for the Powerstep01 reset pin8
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN8  (GPIO_PIN_6)
/// GPIO port used for the Powerstep01 reset pin8
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT8 (GPIOC)

/// GPIO Pin used for the Powerstep01 reset pin9
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN9  (GPIO_PIN_5)
/// GPIO port used for the Powerstep01 reset pin9
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT9 (GPIOG)
/// GPIO Pin used for the Powerstep01 reset pin10
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN10  (GPIO_PIN_15)
/// GPIO port used for the Powerstep01 reset pin10
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT10 (GPIOD)

/// GPIO Pin used for the Powerstep01 reset pin11
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN11  (GPIO_PIN_11)
/// GPIO port used for the Powerstep01 reset pin11
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT11 (GPIOD)
/// GPIO Pin used for the Powerstep01 reset pin12
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN12  (GPIO_PIN_15)
/// GPIO port used for the Powerstep01 reset pin12
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT12 (GPIOB)

/// GPIO Pin used for the Powerstep01 reset pin13
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN13  (GPIO_PIN_11)
/// GPIO port used for the Powerstep01 reset pin13
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT13 (GPIOB)
/// GPIO Pin used for the Powerstep01 reset pin14
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PIN14  (GPIO_PIN_13)
/// GPIO port used for the Powerstep01 reset pin14
#define BSP_MOTOR_CONTROL_BOARD_STBY_RESET_PORT14 (GPIOE)













/// GPIO Pin used for the Powerstep01 SPI chip select pin
//#define BSP_MOTOR_CONTROL_BOARD_CS_PIN  (GPIO_PIN_4)
/// GPIO port used for the Powerstep01 SPI chip select  pin
//#define BSP_MOTOR_CONTROL_BOARD_CS_PORT (GPIOD)

/// GPIO Pin used for the Powerstep01 SPI chip select pin1
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN1  (GPIO_PIN_4)
/// GPIO port used for the Powerstep01 SPI chip select  pin1
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT1 (GPIOD)
/// GPIO Pin used for the Powerstep01 SPI chip select pin2
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN2  (GPIO_PIN_9)
/// GPIO port used for the Powerstep01 SPI chip select  pin2
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT2 (GPIOG)

/// GPIO Pin used for the Powerstep01 SPI chip select pin3
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN3  (GPIO_PIN_13)
/// GPIO port used for the Powerstep01 SPI chip select  pin3
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT3 (GPIOG)
/// GPIO Pin used for the Powerstep01 SPI chip select pin4
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN4  (GPIO_PIN_4)
/// GPIO port used for the Powerstep01 SPI chip select  pin4
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT4 (GPIOB)


/// GPIO Pin used for the Powerstep01 SPI chip select pin5
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN5  (GPIO_PIN_5)
/// GPIO port used for the Powerstep01 SPI chip select  pin5
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT5 (GPIOE)
/// GPIO Pin used for the Powerstep01 SPI chip select pin6
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN6  (GPIO_PIN_14)
/// GPIO port used for the Powerstep01 SPI chip select  pin6
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT6 (GPIOC)


/// GPIO Pin used for the Powerstep01 SPI chip select pin7
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN7  (GPIO_PIN_2)
/// GPIO port used for the Powerstep01 SPI chip select  pin7
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT7 (GPIOF)
/// GPIO Pin used for the Powerstep01 SPI chip select pin8
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN8  (GPIO_PIN_9)
/// GPIO port used for the Powerstep01 SPI chip select  pin8
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT8 (GPIOC)

/// GPIO Pin used for the Powerstep01 SPI chip select pin9
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN9  (GPIO_PIN_8)
/// GPIO port used for the Powerstep01 SPI chip select  pin9
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT9 (GPIOG)
/// GPIO Pin used for the Powerstep01 SPI chip select pin10
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN10  (GPIO_PIN_4)
/// GPIO port used for the Powerstep01 SPI chip select  pin10
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT10 (GPIOG)

/// GPIO Pin used for the Powerstep01 SPI chip select pin11
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN11  (GPIO_PIN_14)
/// GPIO port used for the Powerstep01 SPI chip select  pin11
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT11 (GPIOD)
/// GPIO Pin used for the Powerstep01 SPI chip select pin12
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN12  (GPIO_PIN_10)
/// GPIO port used for the Powerstep01 SPI chip select  pin12
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT12 (GPIOD)

/// GPIO Pin used for the Powerstep01 SPI chip select pin13
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN13  (GPIO_PIN_14)
/// GPIO port used for the Powerstep01 SPI chip select  pin13
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT13 (GPIOB)
/// GPIO Pin used for the Powerstep01 SPI chip select pin14
#define BSP_MOTOR_CONTROL_BOARD_CS_PIN14  (GPIO_PIN_10)
/// GPIO port used for the Powerstep01 SPI chip select  pin14
#define BSP_MOTOR_CONTROL_BOARD_CS_PORT14 (GPIOB)








/* Definition for SPIx clock resources */
#ifndef BSP_MOTOR_CONTROL_BOARD_USE_SPI2
/* Default SPI is SPI1 */

/// Used SPI
#define SPIx                             (SPI1)

/// SPI clock enable
#define SPIx_CLK_ENABLE()                __SPI1_CLK_ENABLE()

/// SPI SCK enable
#define SPIx_SCK_GPIO_CLK_ENABLE()       __GPIOA_CLK_ENABLE()

/// SPI MISO enable
#define SPIx_MISO_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE() 

/// SPI MOSI enable
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE() 

/// SPI Force reset
#define SPIx_FORCE_RESET()               __SPI1_FORCE_RESET()

/// SPI Release reset
#define SPIx_RELEASE_RESET()             __SPI1_RELEASE_RESET()

/// SPI SCK pin
#define SPIx_SCK_PIN                     (GPIO_PIN_5)

/// SPI SCK port
#define SPIx_SCK_GPIO_PORT               (GPIOA)

/// SPI MISO pin 
#define SPIx_MISO_PIN                    (GPIO_PIN_6)

/// SPI MISO port
#define SPIx_MISO_GPIO_PORT              (GPIOA)

/// SPI MOSI pin
#define SPIx_MOSI_PIN                    (GPIO_PIN_7)

/// SPI MOSI port
#define SPIx_MOSI_GPIO_PORT              (GPIOA)

#else  /* USE SPI2 */

/// Used SPI
#define SPIx                             (SPI2)

/// SPI clock enable
#define SPIx_CLK_ENABLE()                __SPI2_CLK_ENABLE()

/// SPI SCK enable
#define SPIx_SCK_GPIO_CLK_ENABLE()       __GPIOB_CLK_ENABLE()

/// SPI MISO enable
#define SPIx_MISO_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE() 

/// SPI MOSI enable
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE() 

/// SPI Force reset
#define SPIx_FORCE_RESET()               __SPI2_FORCE_RESET()

/// SPI Release reset
#define SPIx_RELEASE_RESET()             __SPI2_RELEASE_RESET()

/// SPI SCK pin
#define SPIx_SCK_PIN                     (GPIO_PIN_13)

/// SPI SCK port
#define SPIx_SCK_GPIO_PORT               (GPIOB)

/// SPI MISO pin 
#define SPIx_MISO_PIN                    (GPIO_PIN_14)

/// SPI MISO port
#define SPIx_MISO_GPIO_PORT              (GPIOB)

/// SPI MOSI pin
#define SPIx_MOSI_PIN                    (GPIO_PIN_15)

/// SPI MOSI port
#define SPIx_MOSI_GPIO_PORT              (GPIOB)

#endif /* BSP_MOTOR_CONTROL_BOARD_USE_SPI2 */

/// SPI SCK AF
#define SPIx_SCK_AF                      (BSP_MOTOR_CONTROL_BOARD_SPIx_SCK_AF)

/// SPI MISO AF 
#define SPIx_MISO_AF                     (BSP_MOTOR_CONTROL_BOARD_SPIx_SCK_AF)

/// SPI MOSI AF
#define SPIx_MOSI_AF                     (BSP_MOTOR_CONTROL_BOARD_SPIx_SCK_AF)

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* X_NUCLEO_IHM03A1_STM32F4XX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
