/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portevent.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/

/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"
#include "mb.h"
#include "mbport.h"
#include "swtimer.h"

void _Error_Handler(char * file, int line);
/* ----------------------- Defines ------------------------------------------*/
#define MB_TIM_DEBUG	(1)
#define MB_TIM_HNDL	(&htim9)
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim9;
/* ----------------------- Static variables ---------------------------------*/
//static uint8_t mb_timer_35;
static uint8_t mb_delay_timer;

//static void tim35_timer_func(void);

/* ----------------------- Start implementation -----------------------------*/
/*
void tim35_timer_func(void)
{
	pxMBPortCBTimerExpired();
}
*/

void MBPortTimerExpiredCB(void)
{
#if MB_TIM_DEBUG == 1
	HAL_GPIO_WritePin(MBTimerDebug_GPIO_Port, MBTimerDebug_Pin, GPIO_PIN_RESET);
#endif
	pxMBPortCBTimerExpired();
}

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	//mb_timer_35 = SwTimer_Add(SWTIMER_MODE_STOP, usTim1Timerout50us*MB_TIMER_TIMEOUT, tim35_timer_func);
	if ((hal_status = HAL_TIM_Base_DeInit(MB_TIM_HNDL)) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	MB_TIM_HNDL->Init.Period *= usTim1Timerout50us;
	if ((hal_status = HAL_TIM_Base_Init(MB_TIM_HNDL)) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	mb_delay_timer = SwTimer_Add(SWTIMER_MODE_STOP, MB_DELAY_TIMER_TIMEOUT, NULL);
	return HAL_OK == hal_status;
}

void vMBPortTimerClose(void)
{
	HAL_TIM_Base_DeInit(MB_TIM_HNDL);
}

void vMBPortTimersEnable()
{
	//SwTimer_SetCounter(mb_timer_35, 0);
	//SwTimer_SetMode(mb_timer_35, SWTIMER_MODE_CYCLE);
#if MB_TIM_DEBUG == 1
	HAL_GPIO_WritePin(MBTimerDebug_GPIO_Port, MBTimerDebug_Pin, GPIO_PIN_SET);
#endif
	if (HAL_IS_BIT_SET(MB_TIM_HNDL->Instance->CR1, TIM_CR1_CEN))
	{
		HAL_TIM_Base_Stop_IT(MB_TIM_HNDL);
	}
	//__HAL_TIM_SET_COUNTER(MB_TIM_HNDL, 0);
	__HAL_TIM_URS_ENABLE(MB_TIM_HNDL);
	MB_TIM_HNDL->Instance->EGR = TIM_EGR_UG;
	__HAL_TIM_URS_DISABLE(MB_TIM_HNDL);
	HAL_TIM_Base_Start_IT(MB_TIM_HNDL);
}

void vMBPortTimersDisable()
{
	//SwTimer_SetMode(mb_timer_35, SWTIMER_MODE_STOP);
	HAL_TIM_Base_Stop_IT(MB_TIM_HNDL);
#if MB_TIM_DEBUG == 1
	HAL_GPIO_WritePin(MBTimerDebug_GPIO_Port, MBTimerDebug_Pin, GPIO_PIN_RESET);
#endif
}

void vMBPortTimersDelay(USHORT usTimeOutMS)
{

	SwTimer_SetCompare(mb_delay_timer, usTimeOutMS*100);
	SwTimer_SetCounter(mb_delay_timer, 0);
	SwTimer_SetMode(mb_delay_timer, SWTIMER_MODE_SINGLE);
	while (SwTimer_GetStatus(mb_delay_timer))
	{
		;
	}
}


