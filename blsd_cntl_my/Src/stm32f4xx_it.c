/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* USER CODE BEGIN 0 */
#include "adctenso.h"
#include "6Step_Lib_Hall.h"
#include "bitbanding.h"

void SwTimer_IntHandler(void);
void SwTimer_Update(void);
void UART_IRQHandler(UART_HandleTypeDef *huart);
void HallSensorCB(uint16_t GPIO_Pin);
void PressButtonCB(uint16_t GPIO_Pin);
void MBPortTimerExpiredCB(void);
//void MC_IncCommCount(void);
void MC_Commutate(void);
void MC_CommutationCallBack(void);
//void MC_SetMechSpeedUpdate(void);
void MC_ADCx_SixStep_Hall();

extern UART_HandleTypeDef huart2;
extern SIXSTEP_Base_InitTypeDef SIXSTEP_parameters;

extern bool com_flag;


/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim11;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */
	SwTimer_Update();
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
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line2 interrupt.
*/
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
* @brief This function handles ADC1 global interrupt.
*/
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
* @brief This function handles TIM1 break interrupt and TIM9 global interrupt.
*/
void TIM1_BRK_TIM9_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 0 */


  /* USER CODE END TIM1_BRK_TIM9_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  HAL_TIM_IRQHandler(&htim9);
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 1 */

  /* USER CODE END TIM1_BRK_TIM9_IRQn 1 */
}

/**
* @brief This function handles TIM1 trigger and commutation interrupts and TIM11 global interrupt.
*/
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 0 */

  /* USER CODE END TIM1_TRG_COM_TIM11_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  HAL_TIM_IRQHandler(&htim11);
  /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 1 */

  /* USER CODE END TIM1_TRG_COM_TIM11_IRQn 1 */
}

/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  //HAL_GPIO_TogglePin(GPIO_BEMF_GPIO_Port, GPIO_BEMF_Pin);
  /* USER CODE END TIM4_IRQn 1 */
}

/**
* @brief This function handles I2C1 event interrupt.
*/
void I2C1_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_EV_IRQn 0 */

  /* USER CODE END I2C1_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_EV_IRQn 1 */

  /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
* @brief This function handles I2C1 error interrupt.
*/
void I2C1_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_ER_IRQn 0 */

  /* USER CODE END I2C1_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_ER_IRQn 1 */

  /* USER CODE END I2C1_ER_IRQn 1 */
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void USART2_IRQHandler(void)
{
	UART_IRQHandler(&huart2);
}


/*"Быстрая" половина обработчика прерываний аппаратного таймера.
  Эта функция должна быть оформлена как прерывание от задействованного аппаратного таймера.
  Как правило это прерывание переполнения(обновления) или компаратора.
  Больше никаких изменений не требуется, разве что организовать сброс флагов прерывания(если требуется).
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM11)
  {
    SwTimer_IntHandler();
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Set PendSV pending for slow half handler.
  }
  else if (htim->Instance == TIM9)
  {
	  MBPortTimerExpiredCB();
  }
  else if(htim->Instance == TIM4)
  {
	  /*
	  MC_SetMechSpeedUpdate();
	  if (SIXSTEP_parameters.hall_ok == 0)
	  {
		  MC_Commutate();
	  }
	  */
	  MC_TIMx_SixStep_timebase();
  }

//  if (htim->Instance == TIM1)
//  {
////		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
//		if (com_flag) {
//			com_flag = false;
//			HF_TIMx.Instance->EGR = TIM_EVENTSOURCE_COM;
//		}
//  }
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM4)
	{
		if (BIT_BAND_SRAM(SIXSTEP_parameters.hall_ok, 0) == 0)
		  {
			/*
		    if (SIXSTEP_parameters.start_cnt != 0)
		    {
		      SIXSTEP_parameters.hall_ko_successive++;
		      if (SIXSTEP_parameters.hall_ko_successive > HALL_KO_SUCCESSIVE_MAX)
		      {
		        MC_SixStep_Hall_Startup_Failure_Handler();
		        return;
		      }
		      SIXSTEP_parameters.STATUS = STARTUP;
		      if (SIXSTEP_parameters.speed_target > 0)
		      {
		        if(SIXSTEP_parameters.next_step_pos >= 6)
		        {
		          SIXSTEP_parameters.next_step_pos = 1;
		        }
		        else
		        {
		          SIXSTEP_parameters.next_step_pos++;
		        }
		      }
		      else
		      {
		        if(SIXSTEP_parameters.next_step_pos <= 1)
		        {
		          SIXSTEP_parameters.next_step_pos = 6;
		        }
		        else
		        {
		          SIXSTEP_parameters.next_step_pos--;
		        }
		      }
		      MC_SixStep_TABLE(SIXSTEP_parameters.next_step_pos);
		    }
		    else
		    {
		      MC_SixStep_Hall_Run_Failure_Handler();
		    }*/
//		    MC_Commutate();
		  }
//		HF_TIMx.Instance->EGR = TIM_EVENTSOURCE_COM;
	}
}

void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		MC_CommutationCallBack();
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == Hall_1_Pin || GPIO_Pin == Hall_2_Pin || GPIO_Pin == Hall_3_Pin)
	{
		HallSensorCB(GPIO_Pin);
	}
//	else if (GPIO_Pin == B1_Pin)
//	{
//		PressButtonCB(GPIO_Pin);
//	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	/*
	if ((hadc->Instance->SQR3 & ADC_SQR3_SQ1) == ADC_SQR3_RK(TENSO_CH,1))
	{
		ADCTenso_ConvCpltCB(hadc);
	}
	*/
	MC_ADCx_SixStep_Hall();
}


void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3)
	{
		//__HAL_TIM_ENABLE(htim);
		HAL_GPIO_TogglePin(GPIO_BEMF_GPIO_Port, GPIO_BEMF_Pin);
//		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	}
}


void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim)
{
//	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	if (++hard_overcurrent_cnt > MC_HW_OVERCURRENT_MAX_CNT) {
		MC_StopMotor();
		MC_SetError(MC_HARDWARE_OVERCURRENT_Pos);
	}
}



/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
