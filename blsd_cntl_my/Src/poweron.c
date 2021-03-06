//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 25.11.2017
//Изменен:                25.11.2017
//**************************************************************/

/**\author Новиков В. А.*/
/**\file proto.h*/
/**\brief Подача питания на выходные транзисторы.*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "poweron.h"
#include "swtimer.h"
#include "spimbaddr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define PWR_TIM_HNDL	(&htim10)
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim10;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


void InitPower(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	uint8_t ModBusAddr;
	uint8_t PwrTimer;

//	if (GetMBAddress(&ModBusAddr) && ModBusAddr > 1)
//	{
//		PwrTimer = SwTimer_Add(SWTIMER_MODE_SINGLE, PWR_TIMER_TIMEOUT * ModBusAddr, 0);
//		while (!SwTimer_GetStatus(PwrTimer))
//		{
//			;
//		}
//		SwTimer_SetCompare(PwrTimer,PWR_TIMER_TIMEOUT);
//		SwTimer_SetCounter(PwrTimer, 0);
//		SwTimer_SetMode(PwrTimer,SWTIMER_MODE_SINGLE);
//	}
//	else
//	{
//		PwrTimer = SwTimer_Add(SWTIMER_MODE_SINGLE, PWR_TIMER_TIMEOUT, 0);
//	}
//	HAL_TIM_PWM_Start(PWR_TIM_HNDL, TIM_CHANNEL_1);
//	while (!SwTimer_GetStatus(PwrTimer))
//	{
//		;
//	}
//	HAL_TIM_PWM_Stop(PWR_TIM_HNDL, TIM_CHANNEL_1);
	GPIO_InitStruct.Pin = PWR_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(PWR_EN_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(PWR_EN_GPIO_Port, PWR_EN_Pin, GPIO_PIN_SET);
	SwTimer_Delete(PwrTimer);
}
