/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hall.h"
#include "6Step_Lib_Hall.h"
#include "button.h"
#include "atomic.h"
#include "bitbanding.h"
#include "median.h"

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
static float U = 0;
float I_level_chg = 0;
static bool chargerFlag = false;
static bool chargerON = false;
static float delta = 0;
static bool isChargeBeep = false;
extern ButtonState_TypeDef ButtonState;
extern ButtonID_TypeDef ButtonID;
extern uint32_t error;
extern SIXSTEP_Base_InitTypeDef SIXSTEP_parameters;
extern volatile bool rtc_flag;
extern float I_bdc, I_bldc;
extern __IO uint32_t adc_ready_flag;

extern median_context_t median_filter_ctx [ADC_REG_CHANNEL_NUM];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//static void LEDCheck_OnStart (void);
static void LED_OFF_all (void);
static void sound (uint8_t count , uint32_t duration);
static void U_enable_all ();
static void U_disable_all ();
static void U_enable_12V ();
static void U_disable_12V ();
static void U_enable_5V ();
static void U_disable_5V ();
static void U_enable_3V ();
static void U_disable_3V ();
static void ADC_start ();
static void ADC_stop ();
static void blink ();
static void charger ();

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_RTC_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

			//	InitHall( );
			MC_SixStep_INIT( );
			__POWER_LED_ON();
		  	HAL_Delay(300);
			__POWER_LED_OFF();

			// HAL_DBGMCU_EnableDBGStopMode();
			HAL_PWR_DisableSleepOnExit( );
//----------------------------------------------------
			U_enable_all( );
//----------------------------------------------------

			InitHall( );
			ADC_start( );

			U = Ubat_Measure( ); // Get Vbat value;
			if (__IS_EM_BUTTON_EN)
			{
						sound(1, 300);
			}
			else if (U <= BAT_LEVEL_WARN)
			{
						sound(2, 500);
			}

			ADC_stop( );
			if (U >= BAT_LEVEL_WARN && !__IS_EM_BUTTON_EN)
			{
					//	LEDCheck_OnStart( );
			}

			U_disable_all( );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
			while (1)
			{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

						HAL_SuspendTick( );
						HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
						HAL_ResumeTick( );
						SystemClock_Config( );

						if (ButtonID == NONE_BUTTON)
						{
									charger( );

									if (I_level_chg >= 0.1 && !isChargeBeep)
									{
												HAL_NVIC_DisableIRQ(EXTI0_IRQn);
												HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
												HAL_NVIC_DisableIRQ(EXTI4_IRQn);
												__POWER_LED_OFF();
												isChargeBeep = true;
												sound(2, 1000);
									}

									else if (I_level_chg < 0.05)
									{

												isChargeBeep = false;
												chargerON = false;
												delta = 0;
												blink( );
												if (rtc_flag)
												{
															rtc_flag = false;

															__POWER_LED_ON();
															HAL_Delay(300);
															__POWER_LED_OFF();
												}

									}

						}

						switch (ButtonID)
									{

									case UP_BUTTON:

												if (chargerFlag)
												{
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;
															break;
												}
												MC_ResetError( );
												if ( !__IS_EM_BUTTON_EN)
												{
															LED_OFF_all( );
												}
												U_enable_all( );

												InitHall( );

												if (__IS_EM_BUTTON_EN)
												{
															__RED_LED_ON();
															sound(1, 300);
															U_disable_all( );
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;

															break;
												}

												ADC_start( );

												U = Ubat_Measure( ); // Get Vbat value;

												if (U <= BAT_LEVEL_WARN)
												{
															sound(2, 500);
															U_disable_all( );
															ADC_stop( );
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;
															break;
												}
												HAL_TIM_Base_Start_IT( &LF_TIMx); //Start TIM4 then TIM1 will start counting after TIM4 has overloaded;
												HAL_TIM_Base_Start_IT( &SW_TIMx);
												MC_ChangeDirection_reverse( );
												MC_StartMotor_BLDC( );

#if	(TEST > 0)
			do
			{
				MC_ADC_Task_BLDC();

				if(error != 0)
				{
					MC_ResetError();
					MC_StopMotor_BLDC();
					MC_Set_Speed(0);
					MC_Task_Speed();

					for (int i = 0; i < 6000; i++)
					{
						MC_ADC_Task_BLDC();
					}
					InitHall();
					HAL_Delay(60000);
					MC_StartMotor_BLDC();

				}

			} while(1);

      #else
												do
												{

															MC_ADC_Task_BLDC( );
												}while (ButtonID == UP_BUTTON && !error);

#endif
												HAL_TIM_Base_Stop_IT( &SW_TIMx);
												__HAL_TIM_SetCounter(&SW_TIMx,0);
												InitHall( );
												U_disable_all( );

												MC_Set_Speed(0);
												MC_Task_Speed( );
												MC_StopMotor_BLDC( );

												for (int i = 0; i < 10000; i++)
												{
															MC_ADC_Task_BLDC( );
												}

												ADC_stop( );

												HAL_TIM_Base_Stop_IT( &LF_TIMx); //Stop TIM4;
												HAL_TIM_Base_Stop_IT( &HF_TIMx); //Stop TIM1;

												if (BIT_BAND_SRAM(error, MC_OVERCURRENT_Pos))
												{
															sound(1, 1000);
												}

												if (BIT_BAND_SRAM(error, MC_SWITCH_Pos))
												{
															sound(3, 300);
												}

												I_bldc = 0;

												ButtonID = NONE_BUTTON;
												ButtonState = BUTTON_RELEASE;

									break;

									case DOWN_BUTTON:
												if (chargerFlag)
												{
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;
															break;
												}

												MC_ResetError( );
												if ( !__IS_EM_BUTTON_EN)
												{
															LED_OFF_all( );
												}

												U_enable_all( );
												InitHall( );

												if (__IS_EM_BUTTON_EN)
												{
															__RED_LED_ON();
															sound(1, 300);
															U_disable_all( );

															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;

															break;
												}

												ADC_start( );
												U = Ubat_Measure( ); // Get Vbat value;

												if (U <= BAT_LEVEL_WARN)
												{
															sound(2, 500);
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;
															ADC_stop( );
															U_disable_all( );
															break;
												}

												__RELAY_ON();
												HAL_Delay(10);

												HAL_TIM_Base_Start_IT( &LF_TIMx); //Start TIM4 then TIM1 will start counting after TIM4 has overloaded;
												MC_ChangeDirection( );
												MC_StartMotor_BLDC( );
												HAL_TIM_Base_Start_IT( &SW_TIMx);

												do
												{
															MC_ADC_Task_BLDC( );
												}while (ButtonID == DOWN_BUTTON && !error);

												HAL_TIM_Base_Stop_IT( &SW_TIMx);
												__HAL_TIM_SetCounter(&SW_TIMx,0);
												InitHall( );
												U_disable_all( );

												MC_Set_Speed(0);
												MC_Task_Speed( );
												MC_StopMotor_BLDC( );

												for (int i = 0; i < 10000; i++)
												{
															MC_ADC_Task_BLDC( );
												}

												ADC_stop( );

												HAL_TIM_Base_Stop_IT( &LF_TIMx); //Stop TIM4;
												HAL_TIM_Base_Stop_IT( &HF_TIMx); //Stop TIM1;

												__RELAY_OFF();

												if (BIT_BAND_SRAM(error, MC_OVERCURRENT_Pos))
												{
															sound(1, 1000);
												}

												if (BIT_BAND_SRAM(error, MC_SWITCH_Pos))
												{
															sound(3, 300);
												}

												I_bldc = 0;
												ButtonID = NONE_BUTTON;
												ButtonState = BUTTON_RELEASE;

									break;

									case RIGHT_BUTTON:

												if ( !__IS_EM_BUTTON_EN)
												{
															LED_OFF_all( );
												}
												MC_ResetError( );
												U_enable_all( );

												if (__IS_EM_BUTTON_EN)
												{
															sound(1, 300);
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;
															U_disable_all( );
															break;
												}

												ADC_start( );

												U = Ubat_Measure( ); // Get Vbat value;

												if (U <= BAT_LEVEL_WARN)
												{
															sound(2, 500);
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;
															ADC_stop( );
															U_disable_all( );
															break;
												}
												MC_StartMotor_BDC(false);
												HAL_TIM_Base_Start_IT( &SW_TIMx);

												do
												{
															MC_ADC_Task_BDC( );
												}while (ButtonID == RIGHT_BUTTON && !__IS_EM_BUTTON_EN && !__FF2_PIN && !error);

												HAL_TIM_Base_Stop_IT( &SW_TIMx);
												__HAL_TIM_SetCounter(&SW_TIMx,0);
												I_bdc = 0;
												MC_StopMotor_BDC( ); //Switch off PWM outputs for BDC motor;

												U_disable_all( );
												ADC_stop( );

												if (BIT_BAND_SRAM(error, MC_OVERCURRENT_Pos_bdc))
												{
															sound(1, 1000);
												}
												if ( __FF2_PIN || BIT_BAND_SRAM(error, MC_SWITCH_Pos))
												{
															if ( !BIT_BAND_SRAM(error, MC_SWITCH_Pos))
															{
																		MC_SetError(MC_SWITCH_Pos);
															}
															sound(3, 300);
												}

												ButtonID = NONE_BUTTON;
												ButtonState = BUTTON_RELEASE;

									break;

									case LEFT_BUTTON:

												if ( !__IS_EM_BUTTON_EN)
												{
															LED_OFF_all( );
												}
												MC_ResetError( );
												U_enable_all( );

												if (__IS_EM_BUTTON_EN)
												{
															sound(1, 300);
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;
															U_disable_all( );
															break;
												}

												ADC_start( );

												U = Ubat_Measure( ); // Get Vbat value;

												if (U <= BAT_LEVEL_WARN)
												{
															sound(2, 500);
															ButtonID = NONE_BUTTON;
															ButtonState = BUTTON_RELEASE;
															ADC_stop( );
															U_disable_all( );
															break;
												}
												__RELAY_ON();
												HAL_Delay(10);
												MC_StartMotor_BDC(true);
												HAL_TIM_Base_Start_IT( &SW_TIMx);

												do
												{
															MC_ADC_Task_BDC( );
												}while (ButtonID == LEFT_BUTTON && !__IS_EM_BUTTON_EN && !__FF2_PIN && !error);

												HAL_TIM_Base_Stop_IT( &SW_TIMx);
												__HAL_TIM_SetCounter(&SW_TIMx,0);

												I_bdc = 0;

												MC_StopMotor_BDC( ); //Switch off PWM outputs for BLDC motor;
												ADC_stop( );
												U_disable_all( );

												__RELAY_OFF();

												if (BIT_BAND_SRAM(error, MC_OVERCURRENT_Pos_bdc))
												{
															sound(1, 1000);
												}
												if (__FF2_PIN || BIT_BAND_SRAM(error, MC_SWITCH_Pos))
												{
															if ( !BIT_BAND_SRAM(error, MC_SWITCH_Pos))
															{
																		MC_SetError(MC_SWITCH_Pos);
															}
															sound(3, 300);
												}
												ButtonID = NONE_BUTTON;
												ButtonState = BUTTON_RELEASE;

									break;

									default:
									break;

									}

			}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*static void LEDCheck_OnStart (void)
{

			__RED_LED_ON();
			HAL_Delay(300);
			__RED_LED_OFF();

			__BLUE_LED_ON();
			HAL_Delay(300);
			__BLUE_LED_OFF();

			__GREEN_LED_ON();
			HAL_Delay(300);
			__GREEN_LED_OFF();

}*/
static void sound (uint8_t count , uint32_t duration)
{

			for (int i = 1; i <= count; i++)
			{
						__BEEP_ON();
						HAL_Delay(duration);
						__BEEP_OFF();
						if (count > 1)
						{
							HAL_Delay(duration);
						}
			}

}

static void U_enable_all ()
{

			__PWR_12V_ON(); //12V On;
			__PWR_5V_ON(); //5V On;
			__PWR_3U_ON(); //3V_On;

			HAL_Delay(60);

}
static void U_disable_all ()
{
			__PWR_12V_OFF();
			__PWR_5V_OFF();
			__PWR_3U_OFF();
}
static void U_enable_12V ()
{

			__PWR_12V_ON();
			HAL_Delay(60);

}
static void U_disable_12V ()
{

			__PWR_12V_OFF();

}

static void U_enable_5V ()
{

			__PWR_5V_ON();
			HAL_Delay(60);

}

static void U_disable_5V ()
{

			__PWR_5V_OFF();

}

static void U_enable_3V ()
{

			__PWR_3U_ON();
			HAL_Delay(60);

}
static void U_disable_3V ()
{

			__PWR_3U_OFF();

}

static void ADC_start ()
{

			HAL_TIM_Base_Start( &ADC_TIMx); // Start ADC Sample Timer;
			HAL_ADC_Start_DMA( &ADCx, (uint32_t*) SIXSTEP_parameters.ADC_SEQ_Buffer, ADC_REG_CHANNEL_NUM); // Start ADC+DMA;
			HAL_Delay(10);

}
static void ADC_stop ()
{
			HAL_TIM_Base_Stop( &ADC_TIMx); // Stop ADC Sample Timer TIM3;
			HAL_ADC_Stop_DMA( &ADCx); // Stop ADC+DMA;
			__HAL_TIM_SetCounter(&ADC_TIMx,0);
			BIT_BAND_SRAM(adc_ready_flag, 1) = 0;

}

static void blink ()
{
			U_enable_12V( );
			U_enable_5V( );

			if (chargerFlag)
			{
						LED_OFF_all();
						chargerFlag = false;
						HAL_NVIC_EnableIRQ(EXTI0_IRQn);
						HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
						HAL_NVIC_EnableIRQ(EXTI4_IRQn);
						U_enable_3V( );
						ADC_start( );
						U = Ubat_Measure( );
						ADC_stop( );
						U_disable_3V( );
						return;
			}

			if (BIT_BAND_SRAM(error, MC_SWITCH_Pos))
			{
						__RED_LED_ON();
						HAL_Delay(300);
						__RED_LED_OFF();
						HAL_Delay(300);
						__RED_LED_ON();
						HAL_Delay(300);
						__RED_LED_OFF();
						HAL_Delay(300);
						__RED_LED_ON();
						HAL_Delay(300);
						__RED_LED_OFF();

			}

			else if (BIT_BAND_SRAM(error, MC_OVERCURRENT_Pos) || BIT_BAND_SRAM(error, MC_OVERCURRENT_Pos_bdc))
			{
						__BLUE_LED_ON();
				  		HAL_Delay(300);
						__BLUE_LED_OFF();

			}
			else if (__IS_EM_BUTTON_EN)
			{
						__RED_LED_ON();
			}
			else
			{

						if ( !__IS_EM_BUTTON_EN) __RED_LED_OFF();

						Show_Battery_Level(U); // Show Vbat level corresponds to  Vbat value;

						U_disable_5V( );
						U_disable_12V( );
						U_disable_3V( );

			}

}

static void LED_OFF_all (void)
{
			__RED_LED_OFF();
			__BLUE_LED_OFF();
			__GREEN_LED_OFF();
			__POWER_LED_OFF();

}

static void charger ()
{

			static uint32_t sum = 0;
			static uint16_t average = 0;

			U_enable_3V( );
			U_enable_5V( );
			ADC_start( );

			adc_ch_t adc_ch = I_CHG;
			for (int n = 0; n < 64; n++)
			{

						sum += median_filter(SIXSTEP_parameters.ADC_SEQ_Buffer [adc_ch], &median_filter_ctx [3]);
						HAL_Delay(5);
			}

			average = sum / 64;

			if (delta > 0) I_level_chg = (((3.36 * average / 4096.0) / K) / RSENSE_CHG) - delta;
			else I_level_chg = (((3.36 * average / 4096.0) / K) / RSENSE_CHG);

			I_level_chg = floor(I_level_chg * 100) / 100;

			average = 0;
			sum = 0;
			if ( !chargerON)
			{
						chargerON = true;
						delta = I_level_chg - 2.8;
			}
			if (I_level_chg >= 2.8)
			{
						chargerFlag = true;
						__POWER_LED_OFF();
						__RED_LED_ON();
						__BLUE_LED_OFF();
						__GREEN_LED_OFF();

			}

			else if (I_level_chg < 2.8 && I_level_chg >= 1.6)
			{
						chargerFlag = true;
						__POWER_LED_OFF();
						__RED_LED_ON();
						__BLUE_LED_OFF();
						__GREEN_LED_OFF();
			}
			else if (I_level_chg < 1.6 && I_level_chg >= 0.3)
			{
						chargerFlag = true;
						__POWER_LED_OFF();
						__RED_LED_OFF();
						__BLUE_LED_ON();
						__GREEN_LED_OFF();

			}

			else if (I_level_chg > 0 && I_level_chg < 0.3)
			{
						chargerFlag = true;
						__POWER_LED_OFF();
						__RED_LED_OFF();
						__BLUE_LED_OFF();
						__GREEN_LED_ON();

			}

			//U_disable_5V( );
			U_disable_3V( );
			ADC_stop( );
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
