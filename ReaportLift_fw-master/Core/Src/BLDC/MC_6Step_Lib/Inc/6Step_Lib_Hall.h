/**
 ******************************************************************************
 * @file    6Step_Lib.h
 * @author  System lab
 * @version V1.1.0
 * @date    23-Oct-2015
 * @brief   This header file provides the set of functions for Motor Control 
 library
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
#ifndef __6STEP_LIB_HALL_H
#define __6STEP_LIB_HALL_H

#define TEST 0
#define DEBUGGING 0

#include "stm32_nucleo_ihm08m1.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

#define MC_HARDWARE_OVERCURRENT_Pos	(0)
#define MC_OVERCURRENT_Pos		(1)
#define MC_SWITCH_Pos			(2)
#define MC_OVERHEAT_Pos (3)
#define MC_OVERCURRENT_Pos_bdc		(4)
#define MC_EMERGENCY_STOP_Pos           (5)
#define MC_NOT_INITIALIZED_Pos		(6)

// Зависит от частоты ШИМа, для счетчика 1999 -> 2000, 3999 -> 1000
#define MC_OVERCURRENT_MAX_CNT (300)
#define MC_HW_OVERCURRENT_MAX_CNT (1000)
#define MC_OVERHEAT_MAX_CNT (500)

#define BAT_LEVEL_MAX 25.2f
#define BAT_LEVEL_MEDIUM 21.0f
#define BAT_LEVEL_LOW 18.0f
#define BAT_LEVEL_WARN 16.5f

#define __PWR_12V_ON()       (HAL_GPIO_WritePin(CONTROL__12V_GPIO_Port,CONTROL__12V_Pin,GPIO_PIN_SET))
#define __PWR_12V_OFF()      (HAL_GPIO_WritePin(CONTROL__12V_GPIO_Port,CONTROL__12V_Pin,GPIO_PIN_RESET))

#define __PWR_5V_ON()        (HAL_GPIO_WritePin(CONTROL__5V_GPIO_Port,CONTROL__5V_Pin,GPIO_PIN_SET))
#define __PWR_5V_OFF()       (HAL_GPIO_WritePin(CONTROL__5V_GPIO_Port,CONTROL__5V_Pin,GPIO_PIN_RESET))

#define __PWR_3U_ON()        (HAL_GPIO_WritePin(CONTROL_3V_GPIO_Port,CONTROL_3V_Pin,GPIO_PIN_SET))
#define __PWR_3U_OFF()       (HAL_GPIO_WritePin(CONTROL_3V_GPIO_Port,CONTROL_3V_Pin,GPIO_PIN_RESET))

#define __BEEP_ON()          (HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET))
#define __BEEP_OFF()         (HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET))

#define __RED_LED_ON()       (HAL_GPIO_WritePin(RED_LED_GPIO_Port,RED_LED_Pin,GPIO_PIN_SET))
#define __RED_LED_OFF()      (HAL_GPIO_WritePin(RED_LED_GPIO_Port,RED_LED_Pin,GPIO_PIN_RESET))

#define __BLUE_LED_ON()    (HAL_GPIO_WritePin(BLUE_LED_GPIO_Port,BLUE_LED_Pin,GPIO_PIN_SET))
#define __BLUE_LED_OFF()   (HAL_GPIO_WritePin(BLUE_LED_GPIO_Port,BLUE_LED_Pin,GPIO_PIN_RESET))

#define __GREEN_LED_ON()     (HAL_GPIO_WritePin(GREEN_LED_GPIO_Port,GREEN_LED_Pin,GPIO_PIN_SET))
#define __GREEN_LED_OFF()    (HAL_GPIO_WritePin(GREEN_LED_GPIO_Port,GREEN_LED_Pin,GPIO_PIN_RESET))

#define __RELAY_ON()     (HAL_GPIO_WritePin(RELAY_ON_GPIO_Port,RELAY_ON_Pin,GPIO_PIN_SET))
#define __RELAY_OFF()    (HAL_GPIO_WritePin(RELAY_ON_GPIO_Port,RELAY_ON_Pin,GPIO_PIN_RESET))

#define __DRV_SR_ON()           (HAL_GPIO_WritePin(SR_MCU_GPIO_Port,SR_MCU_Pin,GPIO_PIN_SET))
#define __DRV_SR_OFF()          (HAL_GPIO_WritePin(SR_MCU_GPIO_Port,SR_MCU_Pin,GPIO_PIN_RESET))

#define __DRV_PSE_ON()         (HAL_GPIO_WritePin(PSE_MCU_GPIO_Port,PSE_MCU_Pin,GPIO_PIN_SET))
#define __DRV_PSE_OFF()        (HAL_GPIO_WritePin(PSE_MCU_GPIO_Port,PSE_MCU_Pin,GPIO_PIN_RESET))

#define __DRV_RST_ON()         (HAL_GPIO_WritePin(RST_MCU_GPIO_Port,RST_MCU_Pin,GPIO_PIN_RESET))
#define __DRV_RST_OFF()        (HAL_GPIO_WritePin(RST_MCU_GPIO_Port,RST_MCU_Pin,GPIO_PIN_SET))

#define __DRV_LOWB_ON()         (HAL_GPIO_WritePin(LB_MCU_GPIO_Port,LB_MCU_Pin,GPIO_PIN_SET))
#define __DRV_LOWB_OFF()        (HAL_GPIO_WritePin(LB_MCU_GPIO_Port,LB_MCU_Pin,GPIO_PIN_RESET))

#define __IS_EM_BUTTON_EN      (HAL_GPIO_ReadPin(EB_BUTTON_GPIO_Port,EB_BUTTON_Pin))

#define __FF2_PIN            (HAL_GPIO_ReadPin(FF2_MCU_GPIO_Port,FF2_MCU_Pin))

#define __POWER_LED_ON()       (HAL_GPIO_WritePin(POWER_LED_GPIO_Port,POWER_LED_Pin,GPIO_PIN_SET))
#define __POWER_LED_OFF()       (HAL_GPIO_WritePin(POWER_LED_GPIO_Port,POWER_LED_Pin,GPIO_PIN_RESET))

/** 
 * @brief  Six Step parameters
 */
typedef enum {
			IDLE, /* 0 */
			STARTUP, /* 1 */
			VALIDATION, /* 2 */
			STOP, /* 3 */
			START, /* 4 */
			RUN, /* 5 */
			SPEEDFBKERROR, /* 7 */
			OVERCURRENT, /* 8 */
} SIXSTEP_Base_SystStatus_t;

/**
 * @}
 */

/** @defgroup Exported_types  Exported_types
 * @{
 */
/** 
 * @brief  Six Step parameters
 */

typedef struct {
			uint32_t LF_TIMx_PSC; /*!< Prescaler variable for low frequency timer*/
			uint32_t LF_TIMx_ARR; /*!< ARR variable for low frequency timer*/
			uint32_t HF_TIMx_PSC; /*!< Prescaler variable for high frequency timer*/
			uint32_t HF_TIMx_ARR; /*!< ARR variable for high frequency timer*/
			uint32_t HF_TIMx_CCR; /*!< CCR variable for high frequency timer*/
			uint8_t step_position; /*!< Step number variable for SixStep algorithm*/
			SIXSTEP_Base_SystStatus_t STATUS; /*!< Status variable for SixStep algorithm*/
			uint8_t status_prev; /*!< Previous status variable for SixStep algorithm*/
			uint16_t pulse_value; /*!< CCR value for SixStep algorithm*/
			uint16_t ARR_value; /*!< ARR vector for Accell compute*/

			uint8_t ADC_SEQ_Channel [ADC_REG_CHANNEL_NUM]; /*!< Buffer for ADC regular channel */
			uint16_t ADC_SEQ_Buffer [ADC_REG_CHANNEL_NUM]; /*!< Buffer for ADC regular channels used for non BEMF measurements */

			uint8_t Current_BEMF_ch; /*!< ADC regular channel to select for BEMF sensing*/
			uint8_t ADC_BEMF_Channel [ADC_BEMF_CHANNEL_NUM]; /*!< Buffer for BEMF channel */
			uint16_t ADC_BEMF_Buffer [ADC_BEMF_CHANNEL_NUM]; /*!< Buffer for ADC regular channels used for BEMF sensing */

			uint32_t hall_capture; /*!< Input capture register value when hall status changes */
			uint32_t commutation_delay; /*!< Delay between hall capture and step commutation */
			uint8_t hall_ok; /*!< Indication of hall capture */
			uint8_t hall_ko_successive; /*!< Number of successive no hall capture before timer expiration */
			uint8_t start_attempts; /*!< Number of start attempts */
			uint8_t run_attempts; /*!< Number of run attempts */
			volatile int32_t start_cnt; /*!< Counter used to compute initial commutation delays */

			int16_t speed_fdbk; /*!< Motor speed variable*/
			int16_t speed_fdbk_filtered; /*!< Filtered Motor speed variable*/
			int16_t filter_depth; /*!< Filter depth for speed measuring*/
			uint16_t Current_Reference; /*!< Currrent reference for SixStep algorithm*/
			uint16_t Ireference; /*!< Currrent reference for SixStep algorithm*/
			uint16_t current_fdbk;
			uint16_t current_fdbk_filtered;
			uint16_t current_fdbk_bdc;
			uint16_t current_fdbk_filtered_bdc;
			uint16_t current_limit;
			uint16_t current_limit_bdc;
			uint16_t temperature_fdbk_filtered;
			uint16_t temperature_limit;
//  int32_t Integral_Term_sum;           /*!< Global Integral part for PI*/
			uint8_t CMD; /*!< Flag control for Motor Start/Stop*/
			uint16_t Speed_Loop_Time; /*!< Speed loop variable for timing */
			uint16_t Speed_Ref_filtered; /*!< Filtered Reference Motor Speed variable */
			uint8_t RUN_Motor; /*!< Flag for Motor status */
			uint8_t VALIDATION_OK; /*!< Validation flag for Closed loop control begin */
			uint8_t SPEED_VALIDATED; /*!< Validation flag for Speed before closed loop control */
			uint16_t speed_target; /*!< Target Motor Speed */
			uint16_t speed_target_time; /*!< Target Motor Ramp time */
			uint16_t Ramp_Start; /*!< Ramp time start*/
			uint16_t Bemf_delay_start; /*!< Bemf variable */
			uint32_t SYSCLK_frequency; /*!< System clock main frequency */
			uint8_t Button_ready; /*!<  */
			uint16_t IREFERENCE; /*!< Currrent reference*/
			uint8_t NUMPOLESPAIRS; /*!< Number of motor pole pairs  */
			uint32_t ACCEL; /*!< Acceleration start-up parameter*/
			uint16_t KP; /*!< KP parameter for PI regulator */
			uint16_t KI; /*!< KI parameter for PI regulator */
			uint8_t CW_CCW; /*!< Set the motor direction */
			uint8_t target_CW_CWW; /*!< Set the target motor direction */
			uint8_t Potentiometer; /*!< Enable/Disable potentiometer for speed control */
			uint8_t voltage_mode; /*!< Mode of operation: 1 - no feedback, 0 - current feedback */

} SIXSTEP_Base_InitTypeDef; /*!< Six Step Data Structure */

/**
 * @}
 */

/** @defgroup Exported_types  Exported_types
 * @{
 */
/** 
 * @brief  Six PI regulator parameters
 */

typedef struct {
			int16_t Reference; /*!< Refence value for PI regulator */
			int16_t Kp_Gain; /*!< Kp value for PI regulator */
			int16_t Ki_Gain; /*!< Ki value for PI regulator */
			int16_t Lower_Limit_Output; /*!< Min output value for PI regulator */
			int16_t Upper_Limit_Output; /*!< Max output value for PI regulator */
			int8_t Max_PID_Output; /*!< Max Saturation indicator flag */
			int8_t Min_PID_Output; /*!< Min Saturation indicator flag */
} SIXSTEP_PI_PARAM_InitTypeDef_t, *SIXSTEP_pi_PARAM_InitTypeDef_t; /*!< PI Data Structure */

typedef union {
#define REALDIR_DIRECTION_Pos	(0)
#define REALDIR_DIRECTION_Msk	(1<<REALDIR_DIRECTION_Pos)
#define REALDIR_ACTUAL_Pos		(1)
#define REALDIR_ACTUAL_Msk		(1<<REALDIR_ACTUAL_Pos)
			uint8_t direction_flags;
			struct {
						uint8_t direction :1;
						uint8_t actual :1;
			};
} SIXSTEP_RealDirection_t, *SIXSTEP_RealDirection_Ptr_t;

/**
 * @}
 */

/** @defgroup Exported_function_6StepLib  Exported_function_6StepLib
 * @{
 */

typedef enum {
			I_BLDC_CH, BAT_CH, I_BDC_CH, I_CHG, SPEED_CH
} adc_ch_t;

void MC_SixStep_INIT (void);
void MC_SixStep_RESET (void);
void MC_StartMotor_BLDC (void);
void MC_StartMotor_BDC (bool dir);
float Ubat_Measure (void);
void Show_Battery_Level (float level);
void MC_StopMotor_BLDC (void);
void MC_StopMotor_BDC (void);
void MC_Set_Speed (uint16_t);
void MC_SetSpeedTarget (uint16_t speed);
uint16_t MC_GetSpeedTarget (void);
void MC_EXT_button_SixStep (void);
void MC_TIMx_SixStep_timebase (void);
void MC_ADC_Task_BLDC (void);
void MC_ADC_Task_BDC (void);
void MC_Commutate (void);
void MC_IncCommCount (void);
int16_t MC_GetCommCounter (void);
void MC_ResetCommCounter (void);
void MC_ChangeDirection (void);
void MC_ChangeDirection_reverse (void);
uint8_t MC_GetDirection (void);
void MC_SetDirection (uint8_t dir);
uint8_t MC_GetTargetDirection (void);
void MC_SetTargetDirection (uint8_t dir);
bool MC_FindRealDirection (void);
bool MC_GetRealDirection (uint8_t *dir);
void MC_SetAcceleration (uint32_t accel);
uint32_t MC_GetAcceleration (void);
uint8_t MC_GetPolePairs (void);
void MC_SetPolePairs (uint8_t polepairs);
int32_t MC_GetMechSpeedRPMValue ();
void MC_EnablePotentiometer (uint8_t on_off);
bool MC_IsPotentiometerActive (void);
void MC_ShortCoils (void);
void MC_EnableBrake (void);
SIXSTEP_Base_SystStatus_t MC_GetMotorStatus (void);
uint8_t MC_IsMotorRun (void);
void MC_SetRawPulseValue (uint16_t pulse_value);
uint16_t MC_GetRawPulseValue (void);
uint16_t MC_GetCurrent (void);
void MC_SetCurrentLimit (uint16_t current);
uint16_t MC_GetCurrentLimit (void);
void MC_SetTemperatureLimit (uint16_t temp);
uint16_t MC_GetTemperatureLimit (void);
uint32_t MC_GetError ();
void MC_SetError (uint8_t bit_pos);
void MC_ResetError ();
void MC_ClearError (uint8_t bit_pos);
void MC_ResetAllErrors ();
void MC_Filter_Init (void);
/* 6Step Not-Exported functions ----------------------------------------------*/
//int16_t MC_PI_Controller(SIXSTEP_PI_PARAM_InitTypeDef_t *, int16_t);
uint64_t MCM_Sqrt (uint64_t);
int32_t MC_GetElSpeedHz (void);
int32_t MC_GetMechSpeedRPM (void);
//void MC_Set_PI_param(SIXSTEP_PI_PARAM_InitTypeDef_t *);

void MC_SixStep_DisableInput_CH1_D_CH2_D_CH3_D (void);
void MC_SixStep_Start_PWM_driving (void);
void MC_SixStep_Stop_PWM_driving (void);
void MC_SixStep_HF_TIMx_SetDutyCycle_CH1 (uint16_t);
void MC_SixStep_HF_TIMx_SetDutyCycle_CH2 (uint16_t);
void MC_SixStep_HF_TIMx_SetDutyCycle_CH3 (uint16_t);
void MC_SixStep_Current_Reference_Start (void);
void MC_SixStep_Current_Reference_Stop (void);
void MC_SixStep_Current_Reference_Setvalue (uint16_t);
void MC_SixStep_Init_main_data (void);
void MC_Task_Speed (void);
void MC_CommutationCallBack (void);
void MC_Speed_Filter (void);

extern volatile uint32_t hard_overcurrent_cnt;

/**
 * @}
 */

/**  MC_6-STEP_LIB 
 * @}
 */

/**  MIDDLEWARES
 * @}
 */

#endif /*__6STEP_LIB_HALL_H*/
