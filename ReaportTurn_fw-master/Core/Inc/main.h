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
#include "stm32l0xx_hal.h"

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
#define A_30n_Pin GPIO_PIN_3
#define A_30n_GPIO_Port GPIOA
#define A_60n_Pin GPIO_PIN_4
#define A_60n_GPIO_Port GPIOA
#define A_90n_Pin GPIO_PIN_5
#define A_90n_GPIO_Port GPIOA
#define A_120n_Pin GPIO_PIN_6
#define A_120n_GPIO_Port GPIOA
#define A_150n_Pin GPIO_PIN_7
#define A_150n_GPIO_Port GPIOA
#define A_30p_Pin GPIO_PIN_0
#define A_30p_GPIO_Port GPIOB
#define A_60p_Pin GPIO_PIN_1
#define A_60p_GPIO_Port GPIOB
#define A_90p_Pin GPIO_PIN_2
#define A_90p_GPIO_Port GPIOB
#define A_120p_Pin GPIO_PIN_10
#define A_120p_GPIO_Port GPIOB
#define A_150p_Pin GPIO_PIN_11
#define A_150p_GPIO_Port GPIOB
#define PWM_L_Pin GPIO_PIN_12
#define PWM_L_GPIO_Port GPIOB
#define SR_Pin GPIO_PIN_13
#define SR_GPIO_Port GPIOB
#define PWM_H_Pin GPIO_PIN_14
#define PWM_H_GPIO_Port GPIOB
#define PHASE_Pin GPIO_PIN_15
#define PHASE_GPIO_Port GPIOB
#define OPTO_IN_Pin GPIO_PIN_8
#define OPTO_IN_GPIO_Port GPIOA
#define OPTO_IN_EXTI_IRQn EXTI4_15_IRQn
#define RESET_Pin GPIO_PIN_9
#define RESET_GPIO_Port GPIOA
#define RC_R_Pin GPIO_PIN_10
#define RC_R_GPIO_Port GPIOA
#define RC_R_EXTI_IRQn EXTI4_15_IRQn
#define RC_L_Pin GPIO_PIN_11
#define RC_L_GPIO_Port GPIOA
#define RC_L_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
