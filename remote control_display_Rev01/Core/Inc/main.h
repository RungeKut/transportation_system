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

#include "stm32f3xx_ll_crc.h"
#include "stm32f3xx_ll_dma.h"
#include "stm32f3xx_ll_iwdg.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_cortex.h"
#include "stm32f3xx_ll_utils.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_spi.h"
#include "stm32f3xx_ll_tim.h"
#include "stm32f3xx_ll_usart.h"
#include "stm32f3xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "periphery.h"
#include "usart.h"
#include "spi.h"
#include "gpio.h"
#include "digits.h"
//#include "font7x15.h"
#include "st7735.h"
//#include "seg_display.h"
#include "power.h"
#include "rs485.h"
#include "config.h"
#include "dma.h"
#include "crc_work.h"
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
void Convert2Array(uint8_t num);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FREQUENCY_STEP 71
#define Seg_d_Pin LL_GPIO_PIN_15
#define Seg_d_GPIO_Port GPIOC
#define SA2_Pin LL_GPIO_PIN_0
#define SA2_GPIO_Port GPIOA
#define Seg_dp_Pin LL_GPIO_PIN_1
#define Seg_dp_GPIO_Port GPIOA
#define Seg_g_Pin LL_GPIO_PIN_2
#define Seg_g_GPIO_Port GPIOA
#define SA4_Pin LL_GPIO_PIN_3
#define SA4_GPIO_Port GPIOA
#define SA6_Pin LL_GPIO_PIN_4
#define SA6_GPIO_Port GPIOA
#define PIEZO_Pin LL_GPIO_PIN_6
#define PIEZO_GPIO_Port GPIOA
#define VIBRO_Pin LL_GPIO_PIN_0
#define VIBRO_GPIO_Port GPIOB
#define Seg_a_Disp_DC_Pin LL_GPIO_PIN_1
#define Seg_a_Disp_DC_GPIO_Port GPIOB
#define Seg_e_Pin LL_GPIO_PIN_2
#define Seg_e_GPIO_Port GPIOB
#define Seg_f_Pin LL_GPIO_PIN_10
#define Seg_f_GPIO_Port GPIOB
#define SA8_Pin LL_GPIO_PIN_11
#define SA8_GPIO_Port GPIOB
#define SA5_Pin LL_GPIO_PIN_12
#define SA5_GPIO_Port GPIOB
#define SA3_Pin LL_GPIO_PIN_13
#define SA3_GPIO_Port GPIOB
#define SA10_Pin LL_GPIO_PIN_14
#define SA10_GPIO_Port GPIOB
#define SA7_Pin LL_GPIO_PIN_15
#define SA7_GPIO_Port GPIOB
#define SA9_Pin LL_GPIO_PIN_8
#define SA9_GPIO_Port GPIOA
#define Led1_Pin LL_GPIO_PIN_11
#define Led1_GPIO_Port GPIOA
#define SA1_Pin LL_GPIO_PIN_12
#define SA1_GPIO_Port GPIOA
#define Seg_1_Disp_reset_Pin LL_GPIO_PIN_15
#define Seg_1_Disp_reset_GPIO_Port GPIOA
#define Led2_Pin LL_GPIO_PIN_4
#define Led2_GPIO_Port GPIOB
#define Seg_c_Disp_BackLight_Pin LL_GPIO_PIN_5
#define Seg_c_Disp_BackLight_GPIO_Port GPIOB
#define Seg_b_Pin LL_GPIO_PIN_6
#define Seg_b_GPIO_Port GPIOB
#define Seg_2_Disp_CS_Pin LL_GPIO_PIN_7
#define Seg_2_Disp_CS_GPIO_Port GPIOB
#define Seg_3_Disp_IMO_Pin LL_GPIO_PIN_8
#define Seg_3_Disp_IMO_GPIO_Port GPIOB
#define Led3_Pin LL_GPIO_PIN_9
#define Led3_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
