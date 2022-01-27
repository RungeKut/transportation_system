/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/*----------------------------------------------------------------*/
/*Развертка индикатора																						*/
/*----------------------------------------------------------------*/
volatile uint8_t digit = 1;
volatile uint8_t whitingCounter = 1;
volatile uint8_t digits[3] = {0,0,0};
//uint8_t display_segment = 0;
//uint8_t divider = 0;
//volatile uint8_t timerOff = 0;
volatile extern uint16_t weight;
volatile uint8_t TIM3_FLAG = 0;
const uint8_t whiting[3][10] = {
{0xF8, 0xF0, 0xF0, 0xF1, 0xF3, 0xF7, 0xFF, 0xFF, 0xFE, 0xFC},
{0xFE, 0xFE, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xFE, 0xFE, 0xFE},
{0xFE, 0xFF, 0xFF, 0xF7, 0xE7, 0xC7, 0xC6, 0xC6, 0xCE, 0xDE}
};
const uint8_t number[] =
{
  0x3f, //0
  0x06, //1
  0x5b, //2
  0x4f, //3
  0x66, //4
  0x6d, //5
  0x7d, //6
  0x07, //7
  0x7f, //8
  0x6f, //9
	0xff  //погасить разряд
};
/*----------------------------------------------------------------*/
volatile extern uint32_t GLOBAL_FLAG_RX;
volatile extern uint32_t GLOBAL_FLAG_TX;

extern void GoToStandbyMode(void);

void DMA1_TX_InitStart(void);
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void DMA1_RecieveComplete(void);
void DMA1_TransmitComplete(void);
void DMA1_Errors(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */
/*----------------------------------------------------------------*/
void Convert2Array(uint16_t num) //Конвертирует десятичное число в массив по разрядам Ex. 257 -> {2,5,7}
/*----------------------------------------------------------------*/
{
	uint8_t digits_count=0;	
	uint8_t limit=3;
	digits[0]=number[10];
	digits[1]=number[10];		
	digits[2]=number[10];
	if (num <= 0) { return; }
	while (num > 0 && limit) 
	{
		digits[3 - digits_count++] = num % 10;
    num /= 10;
    limit--;
  }
	return;
};
/*----------------------------------------------------------------*/
void SegmentON(uint8_t segment) { //Для 4х разрядного индикатора
	if (digit == 1) {
		LL_GPIO_ResetOutputPin(Seg_3_Disp_IMO_GPIO_Port, Seg_3_Disp_IMO_Pin);	//откл 4 разряд
		LL_GPIO_SetOutputPin(Seg_1_Disp_reset_GPIO_Port, Seg_1_Disp_reset_Pin);		//вкл 1 разряд
	} else if (digit == 2) {
		LL_GPIO_ResetOutputPin(Seg_1_Disp_reset_GPIO_Port, Seg_1_Disp_reset_Pin);	//откл 1 разряд
		LL_GPIO_SetOutputPin(Seg_2_Disp_CS_GPIO_Port, Seg_2_Disp_CS_Pin);		//вкл 2 разряд
	} else if (digit == 3) {
		LL_GPIO_ResetOutputPin(Seg_2_Disp_CS_GPIO_Port, Seg_2_Disp_CS_Pin);	//откл 2 разряд
		LL_GPIO_SetOutputPin(Seg_3_Disp_IMO_GPIO_Port, Seg_3_Disp_IMO_Pin);		//вкл 3 разряд
	}
	if (segment & 0x01) {
		LL_GPIO_ResetOutputPin(Seg_a_Disp_DC_GPIO_Port, Seg_a_Disp_DC_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_a_Disp_DC_GPIO_Port, Seg_a_Disp_DC_Pin);
	} if (segment & 0x02) {
		LL_GPIO_ResetOutputPin(Seg_b_GPIO_Port, Seg_b_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_b_GPIO_Port, Seg_b_Pin);
	} if (segment & 0x04) {
		LL_GPIO_ResetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
	} if (segment & 0x08) {
		LL_GPIO_ResetOutputPin(Seg_d_GPIO_Port, Seg_d_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_d_GPIO_Port, Seg_d_Pin);
	} if (segment & 0x10) {
		LL_GPIO_ResetOutputPin(Seg_e_GPIO_Port, Seg_e_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_e_GPIO_Port, Seg_e_Pin);
	} if (segment & 0x20) {
		LL_GPIO_ResetOutputPin(Seg_f_GPIO_Port, Seg_f_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_f_GPIO_Port, Seg_f_Pin);
	} if (segment & 0x40) {
		LL_GPIO_ResetOutputPin(Seg_g_GPIO_Port, Seg_g_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_g_GPIO_Port, Seg_g_Pin);
	} if (segment & 0x80) {
		LL_GPIO_ResetOutputPin(Seg_dp_GPIO_Port, Seg_dp_Pin);
	} else {
		LL_GPIO_SetOutputPin(Seg_dp_GPIO_Port, Seg_dp_Pin);
	}
	digit++;
	if (digit >= 4) {
		digit = 1;
//для проверки индикатора		
//	weight++;
//	if (weight > 999) { weight = 0; }
		
//		if ((timerOff > 0) || GLOBAL_FLAG_TX != 0) 
//		{
			DMA1_TX_InitStart();
//		}
	}
//	if (timerOff > 0)
//	{
//		timerOff--;
//	}
}
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable Interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles PVD interrupt through EXTI line 16.
  */
void PVD_IRQHandler(void)
{
  /* USER CODE BEGIN PVD_IRQn 0 */

  /* USER CODE END PVD_IRQn 0 */
  HAL_PWR_PVD_IRQHandler();
  /* USER CODE BEGIN PVD_IRQn 1 */

  /* USER CODE END PVD_IRQn 1 */
}

/**
  * @brief This function handles RCC and CRS global interrupt.
  */
void RCC_CRS_IRQHandler(void)
{
  /* USER CODE BEGIN RCC_CRS_IRQn 0 */

  /* USER CODE END RCC_CRS_IRQn 0 */
  /* USER CODE BEGIN RCC_CRS_IRQn 1 */

  /* USER CODE END RCC_CRS_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */
  if(LL_DMA_IsActiveFlag_TC2(DMA1))
  {
    DMA1_TransmitComplete();
  }
	else if(LL_DMA_IsActiveFlag_TC3(DMA1))
  {
    DMA1_RecieveComplete();
  }
  else if(LL_DMA_IsActiveFlag_TE2(DMA1) || LL_DMA_IsActiveFlag_TE3(DMA1))
  {
		DMA1_Errors();
  }
  /* USER CODE END DMA1_Channel2_3_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	if (LL_TIM_IsActiveFlag_UPDATE(TIM2))
	{
		LL_TIM_ClearFlag_UPDATE(TIM2);
		if (!(GLOBAL_FLAG_RX & INITIALIZATION_FLAG))
		{
			Convert2Array(weight);
			SegmentON(number[digits[digit]]);
		}
		else if (GLOBAL_FLAG_RX & INITIALIZATION_FLAG)
		{
			if (!(LL_TIM_IsEnabledCounter(TIM6)))
			{
				LL_TIM_EnableCounter(TIM6);
			}
			SegmentON(255 - (whiting[3 - digit][whitingCounter - 1])) ;
		}
		else
		{
			if (LL_TIM_IsEnabledCounter(TIM6))
			{
				LL_TIM_DisableCounter(TIM6);
			}
		}
	}
  /* USER CODE END TIM2_IRQn 0 */
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	if (LL_TIM_IsActiveFlag_UPDATE(TIM3))
	{
		LL_TIM_ClearFlag_UPDATE(TIM3);
		if (TIM3_FLAG == 0)
		{
			TIM3_FLAG = 1;
			return;
		}
		TIM3_FLAG = 0;
		/* отключаем источник 5V */
//		LL_GPIO_ResetOutputPin(EN_5V_GPIO_Port, EN_5V_Pin);
		LL_mDelay(10);
		GoToStandbyMode();
//		LL_TIM_SetCounter(TIM3,10);
	}
  /* USER CODE END TIM3_IRQn 0 */
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt and DAC1/DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
	if (LL_TIM_IsActiveFlag_UPDATE(TIM6))
	{
		LL_TIM_ClearFlag_UPDATE(TIM6);
		
		whitingCounter++;
		if (whitingCounter >= 11)
		{
			whitingCounter = 1;
		}
	}
  /* USER CODE END TIM6_DAC_IRQn 0 */

  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
