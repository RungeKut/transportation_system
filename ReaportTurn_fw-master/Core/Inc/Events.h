/*
 * Events.h
 *
 *  Created on: Jan 21, 2021
 *      Author: arkhipov.a
 */

#ifndef INC_EVENTS_H_
#define INC_EVENTS_H_

#include "stm32l0xx_hal.h"
#include "constatnts.h"
using namespace constants;

class Events {

			public:
						static Event_t getEvent ();
						static void setEvent (uint16_t GPIO_Pin);
						static void setEvent (Event_t event);
						static void resetEvent ();

			private:
						static Event_t event_;

};

#endif /* INC_EVENTS_H_ */
