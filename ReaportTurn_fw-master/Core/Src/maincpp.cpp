/*
 * maincpp.cpp
 *
 *  Created on: Dec 15, 2020
 *      Author: arkhipov.a
 */
#include "maincpp.h"
#include "Turn.h"
#include "Events.h"
#include "Flash.h"
#include "tim.h"
#include <vector>

using namespace constants;

static float i = 0.0;
static int iCount = 0;
static int sCount = 0;
extern volatile bool bounceTimeOut;

void maincpp ()
{
			HAL_Delay(1000);
			Port port;
			Flash flash;
			Turn turn = Turn(flash);
			Adc adc = Adc(hadc);
			adc.calibration( );
			adc.start( );

			Events::setEvent(NO_EVENTS);
			HAL_TIM_Base_Start_IT( &htim7);

			while (1)
			{
						switch (Events::getEvent( ))
									{

									case OPTO_SENS:

												turn.stop( );
												Events::resetEvent( );

												if ( !port.getPinState(OPTO_IN_GPIO_Port, OPTO_IN_Pin))
												{
															if ( !port.getPinState(RC_L_GPIO_Port, RC_L_Pin) || ( !port.getPinState(RC_R_GPIO_Port, RC_R_Pin)))
															{
																		turn.setCurrentPosition( );
																		sCount = 0;
															}

												}

									break;

									case CURRENT:
												i = adc.getCurrent( );
												if (i >= ITHS)
												{
															iCount++;
															if (iCount >= MAX_I_COUNTER)
															{
																		iCount = 0;
																		turn.stop( );
																		turn.setStopByButton( );
																		turn.setTurnPrev( );
																		Events::resetEvent( );
																		break;
															}
												}
												else iCount = 0;
												if (turn.isRunning( ))
												{
															if (bounceTimeOut)
															{
																		bounceTimeOut = false;
																		sCount++;
																		if (sCount == 300)
																		{
																					sCount = 0;
																					if ( !port.getPinState(OPTO_IN_GPIO_Port, OPTO_IN_Pin))
																					{
																								turn.stop( );
																								turn.setStopByButton( );
																								turn.setTurnPrev( );
																								Events::resetEvent( );
																								break;
																					}

																		}

															}

												}

												if (port.getPinState(OPTO_IN_GPIO_Port, OPTO_IN_Pin) && port.getPinState(RC_L_GPIO_Port, RC_L_Pin)
																		&& (port.getPinState(RC_R_GPIO_Port, RC_R_Pin)) && turn.isRunning( ))
												{
															turn.stop( );
															turn.setStopByButton( );
															turn.setTurnPrev( );
															Events::resetEvent( );
															sCount = 0;

												}

									break;

									case RC_L:
												turn.forward( );
												Events::setEvent(CURRENT);
									break;

									case RC_R:

												turn.backward( );
												Events::setEvent(CURRENT);

									break;

									case NO_EVENTS:
									break;

									}

			}

}

