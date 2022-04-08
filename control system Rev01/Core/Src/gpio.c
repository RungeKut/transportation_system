/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOH);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(FLASH_CS_GPIO_Port, FLASH_CS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(Enable_DC_DC_GPIO_Port, Enable_DC_DC_Pin);

  /**/
  LL_GPIO_ResetOutputPin(DC_DC_SW_GPIO_Port, DC_DC_SW_Pin);

  /**/
  LL_GPIO_ResetOutputPin(DRV_DIRECTION_GPIO_Port, DRV_DIRECTION_Pin);

  /**/
  LL_GPIO_ResetOutputPin(DRV_ENABLE_GPIO_Port, DRV_ENABLE_Pin);

  /**/
  LL_GPIO_ResetOutputPin(DRV_STEP_GPIO_Port, DRV_STEP_Pin);

  /**/
  LL_GPIO_ResetOutputPin(PP_RIGHT_GPIO_Port, PP_RIGHT_Pin);

  /**/
  LL_GPIO_ResetOutputPin(PP_DOWN_GPIO_Port, PP_DOWN_Pin);

  /**/
  LL_GPIO_ResetOutputPin(PP_UP_GPIO_Port, PP_UP_Pin);

  /**/
  LL_GPIO_ResetOutputPin(Relay_1_GPIO_Port, Relay_1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(Relay_2_GPIO_Port, Relay_2_Pin);

  /**/
  LL_GPIO_ResetOutputPin(XS11_GPIO_Port, XS11_Pin);

  /**/
  LL_GPIO_ResetOutputPin(XS12_GPIO_Port, XS12_Pin);

  /**/
  LL_GPIO_ResetOutputPin(FLASH_RESET_GPIO_Port, FLASH_RESET_Pin);

  /**/
  LL_GPIO_ResetOutputPin(INA_CS_GPIO_Port, INA_CS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(PP_RL_GPIO_Port, PP_RL_Pin);

  /**/
  LL_GPIO_ResetOutputPin(PP_YL_GPIO_Port, PP_YL_Pin);

  /**/
  LL_GPIO_ResetOutputPin(PP_GL_GPIO_Port, PP_GL_Pin);

  /**/
  GPIO_InitStruct.Pin = FLASH_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Enable_DC_DC_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(Enable_DC_DC_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DC_DC_SW_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DC_DC_SW_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DRV_DIRECTION_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DRV_DIRECTION_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DRV_ENABLE_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DRV_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DRV_STEP_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DRV_STEP_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = PP_RIGHT_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(PP_RIGHT_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = PP_DOWN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(PP_DOWN_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = PP_UP_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(PP_UP_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Relay_1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(Relay_1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Relay_2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(Relay_2_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LIMIT_STP_MAX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(LIMIT_STP_MAX_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LIMIT_STP_MIN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(LIMIT_STP_MIN_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LIMIT_BDC_MAX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(LIMIT_BDC_MAX_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LIMIT_BDC_MIN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(LIMIT_BDC_MIN_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Panic_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(Panic_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = XS11_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(XS11_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = XS12_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(XS12_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = FLASH_RESET_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(FLASH_RESET_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = INA_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(INA_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = PP_RL_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(PP_RL_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = PP_YL_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(PP_YL_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = PP_GL_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(PP_GL_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
