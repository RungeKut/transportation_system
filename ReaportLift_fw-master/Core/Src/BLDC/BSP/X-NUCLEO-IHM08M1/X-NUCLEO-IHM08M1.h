/**
 ******************************************************************************
 * @file    X-NUCLEO-IHM08M1.h
 * @author  System lab - Automation and Motion control team
 * @version V1.0.0
 * @date    29-Sept-2015
 * @brief   This file provides the set of functions to manage the X-Nucleo board
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

/* Includes ------------------------------------------------------------------*/
#ifndef __X_NUCLEO_IHM08M1_H
#define __X_NUCLEO_IHM08M1_H

#include "l6398.h"

void L6398_ECH1CH2_DCH3_IO_Write(uint8_t,uint8_t,uint16_t);
void L6398_ECH1CH3_DCH2_IO_Write(uint8_t,uint8_t,uint16_t);
void L6398_ECH2CH3_DCH1_IO_Write(uint8_t,uint8_t,uint16_t);
void L6398_DCH1CH2CH3_IO_Write(void);
void L6398_Start_PWM_generation(void); 
void L6398_Stop_PWM_generation(void);
void L6398_HFTIM_DC_CH1(uint16_t CCR_value);
void L6398_HFTIM_DC_CH2(uint16_t CCR_value);
void L6398_HFTIM_DC_CH3(uint16_t CCR_value);

void BSP_X_NUCLEO_FAULT_LED_ON(void);
void BSP_X_NUCLEO_FAULT_LED_OFF(void);

#endif /*__X_NUCLEO_IHM08M1_H*/
