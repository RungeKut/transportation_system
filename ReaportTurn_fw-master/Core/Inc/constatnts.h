/*
 * constatnts.h
 *
 *  Created on: Dec 18, 2020
 *      Author: arkhipov.a
 */

#ifndef INC_CONSTATNTS_H_
#define INC_CONSTATNTS_H_

#include "main.h"

namespace constants {

			typedef enum {
						NO_EVENTS, RC_L, RC_R, OPTO_SENS, CURRENT,
			} Event_t;

			typedef enum {
						NO_ROTATE, NROTATE, PROTATE,
			} Turn_t;


     #define MAX_PWM 60
     #define MAX_I_COUNTER 600
			const uint32_t flashAddress1 = 0x080103F8;
			const uint32_t flashAddress2 = 0x0801F000;
			const uint32_t flashAddress3 = 0x0801F080;
			const uint32_t flashAddress4 = 0x080101A8;


			constexpr float R1 = 10.0;
			constexpr float R2 = 1.0;
			constexpr float K = R1 / R2;
			constexpr float rSense = 0.01;
			constexpr float ITHS = 1500.0; // in mA

}

#endif /* INC_CONSTATNTS_H_ */
