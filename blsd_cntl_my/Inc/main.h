/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
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
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
//#define ADR1_Pin GPIO_PIN_10
//#define ADR1_GPIO_Port GPIOC

//#define ADR2_Pin GPIO_PIN_11
//#define ADR2_GPIO_Port GPIOC

//#define ADR3_Pin GPIO_PIN_12
//#define ADR3_GPIO_Port GPIOC

//#define ADR4_Pin GPIO_PIN_13
//#define ADR4_GPIO_Port GPIOC
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define ADR4_Pin GPIO_PIN_13
#define ADR4_GPIO_Port GPIOC
#define ANALOG_SEL_0_Pin GPIO_PIN_14
#define ANALOG_SEL_0_GPIO_Port GPIOC
#define ANALOG_SEL_1_Pin GPIO_PIN_15
#define ANALOG_SEL_1_GPIO_Port GPIOC
#define CURR_FB_2_Pin GPIO_PIN_1
#define CURR_FB_2_GPIO_Port GPIOC
#define TEMP_FB_Pin GPIO_PIN_2
#define TEMP_FB_GPIO_Port GPIOC
#define BEMF1_Pin GPIO_PIN_3
#define BEMF1_GPIO_Port GPIOC
#define VBUS_Pin GPIO_PIN_1
#define VBUS_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define AN_DEV_Pin GPIO_PIN_5
#define AN_DEV_GPIO_Port GPIOA
#define BEMF2_Pin GPIO_PIN_4
#define BEMF2_GPIO_Port GPIOC
#define BEMF3_Pin GPIO_PIN_5
#define BEMF3_GPIO_Port GPIOC
#define MBTimerDebug_Pin GPIO_PIN_1
#define MBTimerDebug_GPIO_Port GPIOB
#define RS_485_DIR_Pin GPIO_PIN_2
#define RS_485_DIR_GPIO_Port GPIOB
#define Hall_3_Pin GPIO_PIN_10
#define Hall_3_GPIO_Port GPIOB
#define Hall_3_EXTI_IRQn EXTI15_10_IRQn
#define GPIO_BEMF_Pin GPIO_PIN_9
#define GPIO_BEMF_GPIO_Port GPIOC
#define SOFT_START_PWM_Pin GPIO_PIN_11
#define SOFT_START_PWM_GPIO_Port GPIOA
#define CPOUT_Pin GPIO_PIN_12
#define CPOUT_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Hall_1_Pin GPIO_PIN_15
#define Hall_1_GPIO_Port GPIOA
#define Hall_1_EXTI_IRQn EXTI15_10_IRQn
#define ADR1_Pin GPIO_PIN_10
#define ADR1_GPIO_Port GPIOC
#define ADR2_Pin GPIO_PIN_11
#define ADR2_GPIO_Port GPIOC
#define ADR3_Pin GPIO_PIN_12
#define ADR3_GPIO_Port GPIOC
#define Hall_2_Pin GPIO_PIN_2
#define Hall_2_GPIO_Port GPIOD
#define Hall_2_EXTI_IRQn EXTI2_IRQn
#define CURR_REF_Pin GPIO_PIN_4
#define CURR_REF_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_5
#define LED_RED_GPIO_Port GPIOB
#define PWR_EN_Pin GPIO_PIN_8
#define PWR_EN_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_9
#define LED_GREEN_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
