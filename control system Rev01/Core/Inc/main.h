/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_iwdg.h"
#include "stm32l0xx_ll_crs.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "config.h"
#include "periphery.h"
#include "bdc_motor.h"
#include "rs485.h"
#include "step_motor.h"
#include "power.h"
#include "gpio.h"
#include "CurrentBDC.h"
#include "ina239_power.h"
#include "spi.h"
#include "flash_work.h"
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
#define FREQUENCY_STEP 31
#define FLASH_CS_Pin LL_GPIO_PIN_13
#define FLASH_CS_GPIO_Port GPIOC
#define Enable_DC_DC_Pin LL_GPIO_PIN_14
#define Enable_DC_DC_GPIO_Port GPIOC
#define DC_DC_SW_Pin LL_GPIO_PIN_15
#define DC_DC_SW_GPIO_Port GPIOC
#define AIN_CRNT1_Pin LL_GPIO_PIN_0
#define AIN_CRNT1_GPIO_Port GPIOA
#define DRV_DIRECTION_Pin LL_GPIO_PIN_1
#define DRV_DIRECTION_GPIO_Port GPIOA
#define DRV_ENABLE_Pin LL_GPIO_PIN_2
#define DRV_ENABLE_GPIO_Port GPIOA
#define DRV_STEP_Pin LL_GPIO_PIN_3
#define DRV_STEP_GPIO_Port GPIOA
#define PP_RIGHT_Pin LL_GPIO_PIN_4
#define PP_RIGHT_GPIO_Port GPIOA
#define Encoder_Pin LL_GPIO_PIN_5
#define Encoder_GPIO_Port GPIOA
#define PP_DOWN_Pin LL_GPIO_PIN_6
#define PP_DOWN_GPIO_Port GPIOA
#define PP_UP_Pin LL_GPIO_PIN_7
#define PP_UP_GPIO_Port GPIOA
#define PWM_DC_Motor_Pin LL_GPIO_PIN_0
#define PWM_DC_Motor_GPIO_Port GPIOB
#define Relay_1_Pin LL_GPIO_PIN_1
#define Relay_1_GPIO_Port GPIOB
#define Relay_2_Pin LL_GPIO_PIN_2
#define Relay_2_GPIO_Port GPIOB
#define LIMIT_STP_MAX_Pin LL_GPIO_PIN_12
#define LIMIT_STP_MAX_GPIO_Port GPIOB
#define LIMIT_STP_MIN_Pin LL_GPIO_PIN_13
#define LIMIT_STP_MIN_GPIO_Port GPIOB
#define LIMIT_BDC_MAX_Pin LL_GPIO_PIN_14
#define LIMIT_BDC_MAX_GPIO_Port GPIOB
#define LIMIT_BDC_MIN_Pin LL_GPIO_PIN_15
#define LIMIT_BDC_MIN_GPIO_Port GPIOB
#define Panic_Pin LL_GPIO_PIN_8
#define Panic_GPIO_Port GPIOA
#define XS11_Pin LL_GPIO_PIN_11
#define XS11_GPIO_Port GPIOA
#define XS12_Pin LL_GPIO_PIN_12
#define XS12_GPIO_Port GPIOA
#define FLASH_RESET_Pin LL_GPIO_PIN_15
#define FLASH_RESET_GPIO_Port GPIOA
#define INA_CS_Pin LL_GPIO_PIN_6
#define INA_CS_GPIO_Port GPIOB
#define PP_RL_Pin LL_GPIO_PIN_7
#define PP_RL_GPIO_Port GPIOB
#define PP_YL_Pin LL_GPIO_PIN_8
#define PP_YL_GPIO_Port GPIOB
#define PP_GL_Pin LL_GPIO_PIN_9
#define PP_GL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
//#define USART_RECIEVE USART_RX()
//#define USART_TRANSMIT USART_TX()
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
