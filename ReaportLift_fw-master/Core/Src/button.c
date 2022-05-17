/*
 * button.c
 *
 *  Created on: 18 мая 2020 г.
 *      Author: arkhipov.a
 */

#include "button.h"
#include "main.h"


ButtonState_TypeDef ButtonState;
ButtonID_TypeDef ButtonID;



void PressButtonCB (uint16_t GPIO_Pin)
{

			switch (GPIO_Pin)
						{


						case LEFT_BUTTON_Pin:

									if (ButtonID == NONE_BUTTON || ButtonID == LEFT_BUTTON)
									{
												if ((HAL_GPIO_ReadPin(LEFT_BUTTON_GPIO_Port, LEFT_BUTTON_Pin) == GPIO_PIN_RESET))
												{
															if (ButtonState == BUTTON_RELEASE)
															{
																		ButtonState = BUTTON_PRESS;
																		ButtonID = LEFT_BUTTON;
																		// barrier();
															}

												}
												else
												{
															ButtonState = BUTTON_RELEASE;
															ButtonID = NONE_BUTTON;
												}

									}

						break;

						case RIGHT_BUTTON_Pin:
									if (ButtonID == NONE_BUTTON || ButtonID == RIGHT_BUTTON)
									{
												if ((HAL_GPIO_ReadPin(RIGHT_BUTTON_GPIO_Port, RIGHT_BUTTON_Pin) == GPIO_PIN_RESET))
												{
															if (ButtonState == BUTTON_RELEASE)
															{
																		ButtonState = BUTTON_PRESS;
																		ButtonID = RIGHT_BUTTON;
																		//  barrier();
															}
												}
												else
												{
															ButtonState = BUTTON_RELEASE;
															ButtonID = NONE_BUTTON;
												}

									}

						break;

						case UP_BUTTON_Pin:
									if (ButtonID == NONE_BUTTON || ButtonID == UP_BUTTON)

									{
												if ((HAL_GPIO_ReadPin(UP_BUTTON_GPIO_Port, UP_BUTTON_Pin) == GPIO_PIN_RESET))
												{
															if (ButtonState == BUTTON_RELEASE)
															{
																		ButtonState = BUTTON_PRESS;
																		ButtonID = UP_BUTTON;
															}
												}
												else
												{
															ButtonState = BUTTON_RELEASE;
															ButtonID = NONE_BUTTON;
												}
									}

						break;

						case DOWN_BUTTON_Pin:
									if (ButtonID == NONE_BUTTON || ButtonID == DOWN_BUTTON)
									{
												if ((HAL_GPIO_ReadPin(DOWN_BUTTON_GPIO_Port, DOWN_BUTTON_Pin) == GPIO_PIN_RESET))
												{
															if (ButtonState == BUTTON_RELEASE)
															{
																		ButtonState = BUTTON_PRESS;
																		ButtonID = DOWN_BUTTON;
															}
												}
												else
												{
															ButtonState = BUTTON_RELEASE;
															ButtonID = NONE_BUTTON;
												}
									}

						break;

						default:
						break;
						}

}
