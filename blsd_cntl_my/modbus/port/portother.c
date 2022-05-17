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


/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "stm32f4xx_hal.h"

/* ----------------------- Variables ----------------------------------------*/
static ULONG ulNesting;
static uint32_t xOldState;

/* ----------------------- Start implementation -----------------------------*/
void vMBPortEnterCritical(void)
{
	uint32_t xCurState;
	xCurState = __get_PRIMASK();
	__disable_irq();
	if (ulNesting == 0)
	{
		xOldState = xCurState;
	}
	ulNesting++;
}

void vMBPortExitCritical(void)
{
	ulNesting--;
	if (0 == ulNesting)
	{
		__set_PRIMASK(xOldState);
	}
}

void vMBPortClose(void)
{
	extern void vMBPortSerialClose(void);
	extern void vMBPortTimerClose(void);
	vMBPortSerialClose();
	vMBPortTimerClose();
}
