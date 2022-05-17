/*
 * Turn.h
 *
 *  Created on: Dec 15, 2020
 *      Author: arkhipov.a
 */
#include "stm32l0xx_hal.h"
#include "Events.h"
#include <vector>
#include "adc.h"
#include "Port.h"
#include "Driver.h"
#include "Flash.h"
using namespace std;
using namespace constants;
#ifndef INC_TURN_H_
#define INC_TURN_H_

class Turn {

			public:
						Turn ();
						Turn (Flash flash);
						void forward();
						void backward ();
						void stop ();
						void setCurrentPosition();
						void setStopByButton();
						void setTurnPrev();
						bool isRunning();
			private:
						Port port;
						Flash flash;
						Driver driver = Driver(port);
						vector<int> anglesPositiveSector;
						vector<int> anglesNegativeSector;
						void getAngle (vector<int> &vp , vector<int> &vn);
						unsigned int currentPositionPositive=0,currentPositionNegative=0;
						Turn_t turnDirection=NO_ROTATE;
						Turn_t turnDirectionPrev=NO_ROTATE;
						bool isStopByButton=false;
						bool isrun=false;

};
//****************************************************************

class Adc {

			public:
						Adc (ADC_HandleTypeDef hadc);
						void start ();
						void stop ();
						uint32_t getAdcValue ();
						void calibration ();
						float getCurrent ();
			private:
						ADC_HandleTypeDef hadc;
						uint32_t adcValue=0;


};

#endif /* INC_TURN_H_ */

