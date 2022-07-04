/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "crc.h"
#include "dma.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#pragma import(__use_no_heap_region) //Запрет на использование кучи
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//volatile extern uint8_t timerOff;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  /* BusFault_IRQn interrupt configuration */
  NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  /* UsageFault_IRQn interrupt configuration */
  NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  /* SVCall_IRQn interrupt configuration */
  NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  /* DebugMonitor_IRQn interrupt configuration */
  NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  /* PendSV_IRQn interrupt configuration */
  NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));

  /* Peripheral interrupt init*/
  /* PVD_IRQn interrupt configuration */
  NVIC_SetPriority(PVD_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  NVIC_EnableIRQ(PVD_IRQn);
  /* RCC_IRQn interrupt configuration */
  NVIC_SetPriority(RCC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  NVIC_EnableIRQ(RCC_IRQn);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  LL_GPIO_ResetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
  LL_GPIO_SetOutputPin(Led1_GPIO_Port, Led1_Pin);
	LL_GPIO_SetOutputPin(Led2_GPIO_Port, Led2_Pin);
	LL_GPIO_SetOutputPin(Led3_GPIO_Port, Led3_Pin);
  LL_mDelay(100);
  //GLOBAL_FLAG_TX |= TEST_FLAG;
  if	((LL_GPIO_IsInputPinSet(SA6_GPIO_Port, SA6_Pin) == 1) &&
       (LL_GPIO_IsInputPinSet(SA7_GPIO_Port, SA7_Pin) == 1))
  {
    if (LL_GPIO_IsInputPinSet(SA1_GPIO_Port, SA1_Pin) == 1)
    { //Тестирование всех моторов по очереди
      LL_IWDG_ReloadCounter(IWDG);
      GLOBAL_TEST_FLAG |= TEST_ALLMOTORS_FLAG;
      GLOBAL_FLAG_TX |= TEST_FLAG;
    }
    else if (LL_GPIO_IsInputPinSet(SA2_GPIO_Port, SA2_Pin) == 1)
    { //Тестирование ДПТ вперед-назад
      LL_IWDG_ReloadCounter(IWDG);
      GLOBAL_TEST_FLAG |= TEST_BDC_FLAG;
      GLOBAL_FLAG_TX |= TEST_FLAG;
    }
    else if (LL_GPIO_IsInputPinSet(SA3_GPIO_Port, SA3_Pin) == 1)
    { //Тестирование BLDC вверх-вниз
      LL_IWDG_ReloadCounter(IWDG);
      GLOBAL_TEST_FLAG |= TEST_BLDC_FLAG;
      GLOBAL_FLAG_TX |= TEST_FLAG;
    }
    else if (LL_GPIO_IsInputPinSet(SA4_GPIO_Port, SA4_Pin) == 1)
    { //Тестирование ШД сильнее-слабее
      LL_IWDG_ReloadCounter(IWDG);
      GLOBAL_TEST_FLAG |= TEST_STEP_FLAG;
      GLOBAL_FLAG_TX |= TEST_FLAG;
    }
  }
  MX_SPI1_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  
  MX_CRC_Init();
  if (!(GLOBAL_FLAG_TX & TEST_FLAG) )
  {
    MX_IWDG_Init();
    MX_TIM15_Init();
    SleepAfterWatchDog();
  }
  else
  {
//    speed = 0;
    Sound_PWM(4000, 30);
//    LL_TIM_SetPrescaler(TIM_Sound, 2000);
//    Sound_Play(track_sw);
//    LL_TIM_SetPrescaler(TIM_Sound, 359);
  }
  
  /* USER CODE BEGIN 2 */

  
	StartUpConfig();
 // Sound_Play(track_sw);
 // LL_mDelay(500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (!(GLOBAL_FLAG_TX & TEST_FLAG) )
    {
      ScanButton();
      ResetTIM_Sleep();
      LL_IWDG_ReloadCounter(IWDG);
    }
    else
    {
      TestButtonPress();
    }
    DisplayOut(battery, speed, weight);
    Sound_Player((uint8_t *)Sound_track);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(72000000);
  LL_SetSystemCoreClock(72000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

