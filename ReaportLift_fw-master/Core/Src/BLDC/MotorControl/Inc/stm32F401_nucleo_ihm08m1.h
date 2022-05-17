/**
 ******************************************************************************
 * @file    stm32F401_nucleo_ihm08m1.h
 * @author  System lab - Automation and Motion control team
 * @version V1.1.0
 * @date    23-Oct-2015
 * @brief   This file provides the interface between the MC-lib and STM Nucleo
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F401_NUCLEO_IHM08M1_H
#define __STM32F401_NUCLEO_IHM08M1_H

#include "stm32f4xx_hal.h"
#include "main_F401.h"
#include "X-NUCLEO-IHM08M1.h"

#define HF_TIMx                 htim1
#define ADC_TIMx              htim3
#define LF_TIMx                  htim4
#define SW_TIMx                htim5

#define ADCx                       hadc1
#define BDC_TIMx              htim2

#define ADC_REG_CHANNEL_NUM   	5
#define ADC_BEMF_CHANNEL_NUM  3

#define RSENSE_BLDC         0.001f
#define RSENSE_BDC          0.004f
#define RSENSE_CHG          0.004f
#define K                   50
#define RDIV             10.1010f

#define ADC_V_BAT_CH           ADC_CHANNEL_3     /*VBAT*/
#define ADC_CURRENT_BLDC_CH    ADC_CHANNEL_4    /*CURRENT*/
#define ADC_CURRENT_BDC_CH     ADC_CHANNEL_5   /*CURRENT BLDC*/
#define ADC_CURRENT_CHG        ADC_CHANNEL_11   /*CURRENT */
#define ADC_SPEED_CH           ADC_CHANNEL_13   /*SPEED*/

#define HF_TIMx_CH1           TIM_CHANNEL_1
#define HF_TIMx_CH2           TIM_CHANNEL_2
#define HF_TIMx_CH3           TIM_CHANNEL_3
#define HF_TIMx_CCR1          CCR1            /*Channel 1*/
#define HF_TIMx_CCR2          CCR2            /*Channel 2*/
#define HF_TIMx_CCR3          CCR3            /*Channel 3*/

/** @defgroup Exported_function_F401  Exported_function_F401
 * @{
 */
/**
 * @brief  API function for STM32 instruction
 */
void MC_SixStep_ADC_Channel (uint32_t);
void MC_SixStep_Nucleo_Init (void);
void START_Ref_Generation (void);
void STOP_Ref_Generation (void);
void Set_Ref_Generation (uint16_t);
void START_DAC (void);
void STOP_DAC (void);
void SET_DAC_value (uint16_t);
void MC_SixStep_EnableInput_CH1_E_CH2_E_CH3_D (uint8_t , uint8_t , uint16_t);
void MC_SixStep_EnableInput_CH1_E_CH2_D_CH3_E (uint8_t , uint8_t , uint16_t);
void MC_SixStep_EnableInput_CH1_D_CH2_E_CH3_E (uint8_t , uint8_t , uint16_t);
void MC_SixStep_DisableInput_CH1_D_CH2_D_CH3_D (void);
void MC_SixStep_Start_PWM_driving (void);
void MC_SixStep_Stop_PWM_driving (void);
void MC_SixStep_HF_TIMx_SetDutyCycle_CH1 (uint16_t);
void MC_SixStep_HF_TIMx_SetDutyCycle_CH2 (uint16_t);
void MC_SixStep_HF_TIMx_SetDutyCycle_CH3 (uint16_t);
void MC_SixStep_Current_Reference_Start (void);
void MC_SixStep_Current_Reference_Stop (void);
void MC_SixStep_Current_Reference_Setvalue (uint16_t);

#endif
