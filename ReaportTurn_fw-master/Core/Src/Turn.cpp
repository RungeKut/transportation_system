/*
 * Turn.cpp
 *
 *  Created on: Dec 15, 2020
 *      Author: arkhipov.a
 */

#include "adc.h"
#include "main.h"
#include "Turn.h"
#include <vector>
#include "tim.h"
#include "stm32l0xx_hal_tim.h"
#include "Port.h"
#include "Driver.h"
#include "stm32l0xx_it.h"
#include "Flash.h"

using namespace constants;

extern TIM_HandleTypeDef htim6;

Turn::Turn ()
{

			driver.resetInactive( );
			getAngle(anglesPositiveSector, anglesNegativeSector);
			driver.setBrakeModeSDSR( );

}

Turn::Turn (Flash flash) : Turn( )
{
			this->flash = flash;

		  flash.flashRead(currentPositionNegative, flashAddress1);
			flash.flashRead(currentPositionPositive, flashAddress2);
			flash.flashRead(turnDirectionPrev, flashAddress3);
			flash.flashRead(isStopByButton, flashAddress4);

			//AutoMove(flashptr);

}
void Turn::setStopByButton ()
{
			isrun = false;
			isStopByButton = true;

}

bool Turn::isRunning ()
{

			return isrun;

}

void Turn::forward ()
{
			if (port.getPinState(RC_L_GPIO_Port, RC_L_Pin)) return;
			if ( !anglesPositiveSector.empty( ) || currentPositionNegative > 0)
			{
						if (currentPositionNegative > 0 || currentPositionPositive < anglesPositiveSector.size( ))
						{
									turnDirection = PROTATE;

									isrun = true;
									driver.setPhase( );
									HAL_TIM_PWM_Start( &htim21, TIM_CHANNEL_2);
									for (uint8_t comp = 0; comp <= MAX_PWM; comp += 20)
									{
												if (port.getPinState(RC_L_GPIO_Port, RC_L_Pin)) return;

												__HAL_TIM_SET_COMPARE( &htim21, TIM_CHANNEL_2, comp);
												HAL_Delay(100);
									}
						}

						else if (isStopByButton)
						{
									turnDirection = PROTATE;

									isrun = true;
									driver.setPhase( );
									HAL_TIM_PWM_Start( &htim21, TIM_CHANNEL_2);
									for (uint8_t comp = 0; comp <= MAX_PWM; comp += 20)
									{
												if (port.getPinState(RC_L_GPIO_Port, RC_L_Pin)) return;
												__HAL_TIM_SET_COMPARE( &htim21, TIM_CHANNEL_2, comp);
												HAL_Delay(100);
									}

						}

						return;
			}

			if (isStopByButton)
			{
						turnDirection = PROTATE;

						isrun = true;
						driver.setPhase( );
						HAL_TIM_PWM_Start( &htim21, TIM_CHANNEL_2);
						for (uint8_t comp = 0; comp <= MAX_PWM; comp += 20)
						{
									if (port.getPinState(RC_L_GPIO_Port, RC_L_Pin)) return;
									__HAL_TIM_SET_COMPARE( &htim21, TIM_CHANNEL_2, comp);
									HAL_Delay(100);
						}

			}

}
void Turn::backward ()
{
			if (port.getPinState(RC_R_GPIO_Port, RC_R_Pin)) return;
			if ( !anglesNegativeSector.empty( ) || currentPositionPositive > 0)
			{
						if (currentPositionPositive > 0 || currentPositionNegative < anglesNegativeSector.size( ))
						{
									turnDirection = NROTATE;

									isrun = true;
									driver.resetPhase( );

									HAL_TIM_PWM_Start( &htim21, TIM_CHANNEL_2);
									for (uint8_t comp = 0; comp <= MAX_PWM; comp += 20)
									{

												if (port.getPinState(RC_R_GPIO_Port, RC_R_Pin)) return;
												__HAL_TIM_SET_COMPARE( &htim21, TIM_CHANNEL_2, comp);
												HAL_Delay(100);

									}

						}
						else if (isStopByButton)
						{
									turnDirection = NROTATE;

									driver.resetPhase( );
									isrun = true;
									HAL_TIM_PWM_Start( &htim21, TIM_CHANNEL_2);
									for (uint8_t comp = 0; comp <= MAX_PWM; comp += 20)
									{
												if (port.getPinState(RC_R_GPIO_Port, RC_R_Pin)) return;
												__HAL_TIM_SET_COMPARE( &htim21, TIM_CHANNEL_2, comp);
												HAL_Delay(100);
									}

						}
						return;
			}

			if (isStopByButton)
			{
						turnDirection = NROTATE;

						driver.resetPhase( );
						isrun = true;
						HAL_TIM_PWM_Start( &htim21, TIM_CHANNEL_2);
						for (uint8_t comp = 0; comp <= MAX_PWM; comp += 20)
						{
									if (port.getPinState(RC_R_GPIO_Port, RC_R_Pin)) return;
									__HAL_TIM_SET_COMPARE( &htim21, TIM_CHANNEL_2, comp);
									HAL_Delay(100);
						}

			}
}

void Turn::setCurrentPosition ()
{
			if (isrun)
			{
						isrun = false;
						if ( !isStopByButton)
						{

									if (turnDirection == PROTATE && currentPositionNegative > 0)
									{

												currentPositionNegative--;
												flash.flashWrite(flashAddress1, currentPositionNegative);
									}

									else if (turnDirection == NROTATE && currentPositionPositive > 0)
									{

												currentPositionPositive--;
												flash.flashWrite(flashAddress2, currentPositionPositive);
									}
									else if (turnDirection == NROTATE && currentPositionPositive == 0)
									{

												currentPositionNegative++;
												flash.flashWrite(flashAddress1, currentPositionNegative);
									}
									else if (turnDirection == PROTATE && currentPositionNegative == 0)
									{

												currentPositionPositive++;
												flash.flashWrite(flashAddress2, currentPositionPositive);
									}
								  HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
									turnDirection = NO_ROTATE;
									turnDirectionPrev = NO_ROTATE;
									flash.flashWrite(flashAddress3, turnDirectionPrev);
									 HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

						}

						else
						{
									isStopByButton = false;
									if (turnDirectionPrev == turnDirection)
									{
												if (turnDirection == PROTATE && currentPositionNegative > 0)
												{

															currentPositionNegative--;
															flash.flashWrite(flashAddress1, currentPositionNegative);
												}

												else if (turnDirection == NROTATE && currentPositionPositive > 0)
												{

															currentPositionPositive--;
															flash.flashWrite(flashAddress2, currentPositionPositive);
												}
												else if (turnDirection == NROTATE && currentPositionPositive == 0)
												{

															currentPositionNegative++;
															flash.flashWrite(flashAddress1, currentPositionNegative);
												}
												else if (turnDirection == PROTATE && currentPositionNegative == 0)
												{

															currentPositionPositive++;
															flash.flashWrite(flashAddress2, currentPositionPositive);
												}
									}
									 HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
									turnDirection = NO_ROTATE;
									turnDirectionPrev = NO_ROTATE;
									flash.flashWrite(flashAddress3, turnDirectionPrev);
									flash.flashWrite(flashAddress4, isStopByButton);
									HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

						}
			}

}

void Turn::getAngle (vector<int> &vp , vector<int> &vn)
{
			if (port.getPinState(A_30p_GPIO_Port, A_30p_Pin)) vp.push_back(30);
			if (port.getPinState(A_60p_GPIO_Port, A_60p_Pin)) vp.push_back(60);
			if (port.getPinState(A_90p_GPIO_Port, A_90p_Pin)) vp.push_back(90);
			if (port.getPinState(A_120p_GPIO_Port, A_120p_Pin)) vp.push_back(120);
			if (port.getPinState(A_150p_GPIO_Port, A_150p_Pin)) vp.push_back(150);

			if (port.getPinState(A_30n_GPIO_Port, A_30n_Pin)) vn.push_back( -30);
			if (port.getPinState(A_60n_GPIO_Port, A_60n_Pin)) vn.push_back( -60);
			if (port.getPinState(A_90n_GPIO_Port, A_90n_Pin)) vn.push_back( -90);
			if (port.getPinState(A_120n_GPIO_Port, A_120n_Pin)) vn.push_back( -120);
			if (port.getPinState(A_150n_GPIO_Port, A_150n_Pin)) vn.push_back( -150);

}


void Turn::stop ()
{

			HAL_TIM_PWM_Stop( &htim21, TIM_CHANNEL_2);
			__HAL_TIM_SET_COMPARE( &htim21, TIM_CHANNEL_2, 0);

}

void Turn::setTurnPrev ()
{

			switch (turnDirection)
						{
						case NROTATE:
									turnDirectionPrev = NROTATE;

						break;
						case PROTATE:
									turnDirectionPrev = PROTATE;
						break;
						default:
						break;

						}
			HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
			flash.flashWrite(flashAddress3, turnDirectionPrev);
			flash.flashWrite(flashAddress4, isStopByButton);
			HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

//*****************************************************************************************

Adc::Adc (ADC_HandleTypeDef hadc)
{
			this->hadc = hadc;
}
void Adc::start ()
{

			HAL_TIM_Base_Start( &htim6);
			HAL_ADC_Start_DMA( &hadc, &adcValue, 1);

}

void Adc::stop ()
{

			HAL_TIM_Base_Stop( &htim6);
			HAL_ADC_Stop_DMA( &hadc);

}

void Adc::calibration ()
{

			HAL_ADCEx_Calibration_Start( &hadc, ADC_SINGLE_ENDED);

}

float Adc::getCurrent ()
{

			HAL_Delay(5);

			return ((((3.32 * adcValue) / 4095) / K) / (rSense)) * 1000; // motor current in mA;

}

