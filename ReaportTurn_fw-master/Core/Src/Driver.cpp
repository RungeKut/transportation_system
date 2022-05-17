/*
 * Driver.cpp
 *
 *  Created on: 22 янв. 2021 г.
 *      Author: arkhipov.a
 */

#include "Driver.h"
#include "main.h"

Driver::Driver (Port port)
{
			this->port = port;
}

void Driver::resetActive ()
{
			port.resetPinState(RESET_GPIO_Port, RESET_Pin);
}

void Driver::resetInactive ()
{
			port.setPinState(RESET_GPIO_Port, RESET_Pin);
}

void Driver::setPhase ()
{
			port.setPinState(PHASE_GPIO_Port, PHASE_Pin);
}

void Driver::resetPhase ()
{
			port.resetPinState(PHASE_GPIO_Port, PHASE_Pin);
}

void Driver::setSr ()
{
			port.setPinState(SR_GPIO_Port, SR_Pin);
}

void Driver::resetSr ()
{
			port.resetPinState(SR_GPIO_Port, SR_Pin);
}

void Driver::setPwmL ()
{
			port.setPinState(PWM_L_GPIO_Port, PWM_L_Pin);
}

void Driver::resetPwmL ()
{
			port.resetPinState(PWM_L_GPIO_Port, PWM_L_Pin);
}

void Driver::setBrakeModeSDSR ()
{
			setPwmL( );
			setSr( );

}

void Driver::setBrakeModeFD ()
{
			resetPwmL( );
			resetSr( );

}


void Driver::setBrakeModeSD ()
{
			setPwmL( );
			resetSr( );

}
