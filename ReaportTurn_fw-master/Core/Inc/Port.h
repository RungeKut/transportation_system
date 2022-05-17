/*
 * Port.h
 *
 *  Created on: 22 янв. 2021 г.
 *      Author: arkhipov.a
 */

#ifndef INC_PORT_H_
#define INC_PORT_H_

#include "gpio.h"

class Port {

			public:

						void setPinState (GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin);
						void resetPinState (GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin);
						GPIO_PinState getPinState (GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin);

};

#endif /* INC_PORT_H_ */
