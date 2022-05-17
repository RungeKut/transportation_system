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


/* ----------------------- Platform includes --------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "bitbanding.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define UART_HNDL	(&huart2)
#define TXNE		(0)
/* ----------------------- Static variables ---------------------------------*/
extern UART_HandleTypeDef huart2;

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{

	if (xRxEnable)
	{
		//__HAL_UART_DISABLE(UART_HNDL);
		SET_BIT(UART_HNDL->Instance->CR1, UART_MODE_RX);
		/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
		//SET_BIT(UART_HNDL->Instance->CR3, USART_CR3_EIE);
		/* Enable the UART Parity Error and Data Register not empty Interrupts */
		//SET_BIT(UART_HNDL->Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);
		__HAL_UART_ENABLE_IT(UART_HNDL, UART_IT_ERR);
		__HAL_UART_ENABLE_IT(UART_HNDL, UART_IT_PE);
		__HAL_UART_ENABLE_IT(UART_HNDL, UART_IT_RXNE);
	}
	else
	{
		//__HAL_UART_DISABLE(UART_HNDL);
		/*
		CLEAR_BIT(UART_HNDL->Instance->CR1,	(USART_CR1_PEIE | USART_CR1_RXNEIE));
		CLEAR_BIT(UART_HNDL->Instance->CR3, USART_CR3_EIE);
		*/
		__HAL_UART_DISABLE_IT(UART_HNDL, UART_IT_RXNE);
		__HAL_UART_DISABLE_IT(UART_HNDL, UART_IT_PE);
		__HAL_UART_DISABLE_IT(UART_HNDL, UART_IT_ERR);
		CLEAR_BIT(UART_HNDL->Instance->CR1, UART_MODE_RX);
		//__HAL_UART_ENABLE(UART_HNDL);
	}

	if (xTxEnable)
	{
		//__HAL_UART_DISABLE(UART_HNDL);
		HAL_GPIO_WritePin(RS_485_DIR_GPIO_Port, RS_485_DIR_Pin, GPIO_PIN_SET);
		__HAL_UART_ENABLE_IT(UART_HNDL, UART_IT_TXE);
		__HAL_UART_DISABLE_IT(UART_HNDL, UART_IT_TC);
		//__HAL_UART_ENABLE(UART_HNDL);
	}
	else
	{
		//__HAL_UART_DISABLE(UART_HNDL);
		__HAL_UART_DISABLE_IT(UART_HNDL, UART_IT_TXE);
		__HAL_UART_ENABLE_IT(UART_HNDL, UART_IT_TC);
		//__HAL_UART_ENABLE(UART_HNDL);
	}
}

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
	UART_InitTypeDef uart_config;
	BOOL bStatus = FALSE;

	(void) ucPORT;
	memcpy(&uart_config, &(UART_HNDL->Init), sizeof(UART_InitTypeDef));
	bStatus = TRUE;
	uart_config.BaudRate = ulBaudRate;

	switch (eParity)
	{
	case MB_PAR_NONE:
		uart_config.Parity = UART_PARITY_NONE;
		uart_config.WordLength = (ucDataBits <=8) ? UART_WORDLENGTH_8B :
				(bStatus = FALSE, UART_WORDLENGTH_9B);
		uart_config.StopBits = UART_STOPBITS_2;
		break;
	case MB_PAR_ODD:
		uart_config.Parity = UART_PARITY_ODD;
		uart_config.WordLength = (ucDataBits < 8) ? UART_WORDLENGTH_8B :
				(ucDataBits == 8 ) ? UART_WORDLENGTH_9B :
						(bStatus = FALSE, UART_WORDLENGTH_9B);
		uart_config.StopBits = UART_STOPBITS_1;
		break;
	case MB_PAR_EVEN:
		uart_config.Parity = UART_PARITY_EVEN;
		uart_config.WordLength = (ucDataBits < 8) ? UART_WORDLENGTH_8B :
				(ucDataBits == 8 ) ? UART_WORDLENGTH_9B :
						(bStatus = FALSE, UART_WORDLENGTH_9B);
		uart_config.StopBits = UART_STOPBITS_1;
		break;
	default:
		bStatus = FALSE;
		break;
	}

	if ( TRUE == bStatus)
	{
		memcpy(&(UART_HNDL->Init), &uart_config, sizeof(UART_InitTypeDef));
		HAL_UART_DeInit(UART_HNDL);
		HAL_UART_Init(UART_HNDL);
	}

	return bStatus;
}

void vMBPortSerialClose(void)
{
	HAL_UART_DeInit(UART_HNDL);
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
	UART_HNDL->Instance->DR = (uint16_t)((uint16_t)ucByte & (uint16_t)(0x00FF));
	return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR * pucByte)
{
	*pucByte = (CHAR)(READ_REG(UART_HNDL->Instance->DR) & (uint16_t)0x00FF);
	return TRUE;
}

void UART_IRQHandler(UART_HandleTypeDef *huart)
{
	uint32_t isrflags   = READ_REG(huart->Instance->SR);
	uint32_t cr1its     = READ_REG(huart->Instance->CR1);
	uint32_t cr3its     = READ_REG(huart->Instance->CR3);
	uint32_t errorflags = 0x00U;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
	if(errorflags == RESET)
	{
		/* UART in mode Receiver -------------------------------------------------*/
		if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
		{
			pxMBFrameCBByteReceived();
			return;
		}
	}
	/* If some errors occur */
	if((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
	{
		/* UART parity error interrupt occurred ----------------------------------*/
		if(((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
		{
			huart->ErrorCode |= HAL_UART_ERROR_PE;
		}

		/* UART noise error interrupt occurred -----------------------------------*/
		if(((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			huart->ErrorCode |= HAL_UART_ERROR_NE;
		}

		/* UART frame error interrupt occurred -----------------------------------*/
		if(((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			huart->ErrorCode |= HAL_UART_ERROR_FE;
		}

		/* UART Over-Run interrupt occurred --------------------------------------*/
		if(((isrflags & USART_SR_ORE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			huart->ErrorCode |= HAL_UART_ERROR_ORE;
		}

		/* Call UART Error Call back function if need be --------------------------*/
		if(huart->ErrorCode != HAL_UART_ERROR_NONE)
		{
			/* UART in mode Receiver -----------------------------------------------*/
			if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
			{
				pxMBFrameCBByteReceived();
			}
			HAL_UART_ErrorCallback(huart);
			huart->ErrorCode = HAL_UART_ERROR_NONE;
		}
		return;
	}
	/* UART in mode Transmitter ------------------------------------------------*/
	if(((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
	{
		pxMBFrameCBTransmitterEmpty();
		return;
	}

	/* UART in mode Transmitter end --------------------------------------------*/
	if(((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
	{
		HAL_GPIO_WritePin(RS_485_DIR_GPIO_Port, RS_485_DIR_Pin, GPIO_PIN_RESET);
		//CLEAR_BIT(UART_HNDL->Instance->CR1, USART_CR1_TCIE);
		__HAL_UART_DISABLE_IT(UART_HNDL, UART_IT_TC);
		return;
	}

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_CLEAR_PEFLAG(huart);
}
