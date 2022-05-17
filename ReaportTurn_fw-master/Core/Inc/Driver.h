/*
 * Driver.h
 *
 *  Created on: 22 янв. 2021 г.
 *      Author: arkhipov.a
 */

#ifndef SRC_DRIVER_H_
#define SRC_DRIVER_H_

#include "Port.h"

class Driver {

			public:
						Driver (Port port);
						void resetActive ();
						void resetInactive ();
						void setPhase ();
						void resetPhase ();
						void setSr ();
						void resetSr ();
						void setPwmL ();
						void resetPwmL ();
						void setBrakeModeSDSR ();
						void setBrakeModeSD ();
						void setBrakeModeFD ();
			private:
						Port port;
};

#endif /* SRC_DRIVER_H_ */
