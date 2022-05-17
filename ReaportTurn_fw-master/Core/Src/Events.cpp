/*
 * Events.cpp
 *
 *  Created on: Jan 21, 2021
 *      Author: arkhipov.a
 */
#include "Events.h"
#include "main.h"
#include "stm32l0xx_it.h"
#include "constatnts.h"
#include "Turn.h"
#include "Port.h"
#include "tim.h"
using namespace constants;
extern volatile bool bounceTimeOut;

Port port;
int icount = 0;

Event_t Events::getEvent ()
{

			return event_;

}

void Events::resetEvent ()
{

			event_ = NO_EVENTS;

}

void Events::setEvent (uint16_t GPIO_Pin)
{

			switch (GPIO_Pin)
						{
						case RC_L_Pin:
									if (port.getPinState(RC_R_GPIO_Port, RC_R_Pin))
									event_ = RC_L;
						break;
						case RC_R_Pin:
									if (port.getPinState(RC_L_GPIO_Port, RC_L_Pin))
									event_ = RC_R;
						break;

						case OPTO_IN_Pin:

									bounceTimeOut = false;
									__HAL_TIM_SET_COUNTER( &htim7, 0);

									do
									{
									}while ( !bounceTimeOut);
									if ( !port.getPinState(OPTO_IN_GPIO_Port, OPTO_IN_Pin))
									{
											icount++;
												if (icount == 2)
												{
															icount = 0;
															return;
												}
												event_ = OPTO_SENS;
									}

						break;
						default:
									event_ = NO_EVENTS;
						break;

						}

}

void Events::setEvent (Event_t event)
{

			event_ = event;
}

Event_t Events::event_ = NO_EVENTS;
