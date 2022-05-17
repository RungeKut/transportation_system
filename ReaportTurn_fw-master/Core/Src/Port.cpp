/*
 * Port.cpp
 *
 *  Created on: 22 янв. 2021 г.
 *      Author: arkhipov.a
 */
#include "Port.h"



void Port::setPinState (GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin)
{
			HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

void Port::resetPinState (GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin)
{
			HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

GPIO_PinState Port::getPinState (GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin)
{

			return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

