/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BEEP_Pin GPIO_PIN_13
#define BEEP_GPIO_Port GPIOC
#define SR_MCU_Pin GPIO_PIN_14
#define SR_MCU_GPIO_Port GPIOC
#define RST_MCU_Pin GPIO_PIN_15
#define RST_MCU_GPIO_Port GPIOC
#define LB_MCU_Pin GPIO_PIN_0
#define LB_MCU_GPIO_Port GPIOC
#define ADC_I_CHG_Pin GPIO_PIN_1
#define ADC_I_CHG_GPIO_Port GPIOC
#define CONTROL__12V_Pin GPIO_PIN_2
#define CONTROL__12V_GPIO_Port GPIOC
#define SPEED_Pin GPIO_PIN_3
#define SPEED_GPIO_Port GPIOC
#define RELAY_ON_Pin GPIO_PIN_1
#define RELAY_ON_GPIO_Port GPIOA
#define PSE_MCU_Pin GPIO_PIN_2
#define PSE_MCU_GPIO_Port GPIOA
#define SENSOR_V_Pin GPIO_PIN_3
#define SENSOR_V_GPIO_Port GPIOA
#define ADC_I_BLDC_Pin GPIO_PIN_4
#define ADC_I_BLDC_GPIO_Port GPIOA
#define I_ADC_BDC_Pin GPIO_PIN_5
#define I_ADC_BDC_GPIO_Port GPIOA
#define FF2_MCU_Pin GPIO_PIN_7
#define FF2_MCU_GPIO_Port GPIOA
#define HALL_SENSOR_3_Pin GPIO_PIN_4
#define HALL_SENSOR_3_GPIO_Port GPIOC
#define HALL_SENSOR_3_EXTI_IRQn EXTI4_IRQn
#define HALL_SENSOR_2_Pin GPIO_PIN_5
#define HALL_SENSOR_2_GPIO_Port GPIOC
#define HALL_SENSOR_2_EXTI_IRQn EXTI9_5_IRQn
#define HALL_SENSOR_1_Pin GPIO_PIN_0
#define HALL_SENSOR_1_GPIO_Port GPIOB
#define HALL_SENSOR_1_EXTI_IRQn EXTI0_IRQn
#define CONTROL__5V_Pin GPIO_PIN_1
#define CONTROL__5V_GPIO_Port GPIOB
#define CONTROL_3V_Pin GPIO_PIN_2
#define CONTROL_3V_GPIO_Port GPIOB
#define EB_BUTTON_Pin GPIO_PIN_12
#define EB_BUTTON_GPIO_Port GPIOB
#define RIGHT_BUTTON_Pin GPIO_PIN_15
#define RIGHT_BUTTON_GPIO_Port GPIOA
#define RIGHT_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define LEFT_BUTTON_Pin GPIO_PIN_10
#define LEFT_BUTTON_GPIO_Port GPIOC
#define LEFT_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define GREEN_LED_Pin GPIO_PIN_11
#define GREEN_LED_GPIO_Port GPIOC
#define BLUE_LED_Pin GPIO_PIN_12
#define BLUE_LED_GPIO_Port GPIOC
#define RED_LED_Pin GPIO_PIN_2
#define RED_LED_GPIO_Port GPIOD
#define DOWN_BUTTON_Pin GPIO_PIN_6
#define DOWN_BUTTON_GPIO_Port GPIOB
#define DOWN_BUTTON_EXTI_IRQn EXTI9_5_IRQn
#define UP_BUTTON_Pin GPIO_PIN_7
#define UP_BUTTON_GPIO_Port GPIOB
#define UP_BUTTON_EXTI_IRQn EXTI9_5_IRQn
#define POWER_LED_Pin GPIO_PIN_8
#define POWER_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
