/**
 ******************************************************************************
 * @file    6Step_Lib.c
 * @author  System lab - Automation and Motion control team
 * @version V1.1.0
 * @date    23-Oct-2015
 * @brief   This file provides the set of functions for Motor Control library 
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

/*! ****************************************************************************
================================================================================   
                 ###### Main functions for 6-Step algorithm ######
================================================================================     
The main function are the following:

1) MC_SixStep_TABLE(...)        -> Set the peripherals (TIMx, GPIO etc.) for each step
2) MC_SixStep_ARR_step()        -> Generate the ARR value for Low Frequency TIM during start-up
3) MC_SixStep_INIT()            -> Init the main variables for motor driving from MC_SixStep_param.h
4) MC_SixStep_RESET()           -> Reset all variables used for 6Step control algorithm
5) MC_SixStep_Ramp_Motor_calc() -> Calculate the acceleration profile step by step for motor during start-up 
6) MC_SixStep_NEXT_step()       -> Generate the next step number according with the direction (CW or CCW)
7) MC_Task_Speed()              -> Speed Loop with PI regulator
8) MC_Set_Speed(...)            -> Set the new motor speed value
9) MC_StartMotor()              -> Start the Motor
10)MC_StopMotor()               -> Stop the Motor
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdbool.h>
#include "6Step_Lib_Hall.h"
#include "hall.h"
#include "atomic.h"
#include "bitbanding.h"
#include "adctenso.h"
#include "median.h"
#include "ramp.h"
#include "eprom.h"


/** @addtogroup MIDDLEWARES     MIDDLEWARES 
  * @brief  Middlewares Layer
  * @{ 
  */


/** @addtogroup MC_6-STEP_LIB       MC_6-STEP_LIB 
  * @brief  Motor Control driver
  * @{ 
  */


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Data struct ---------------------------------------------------------------*/


SIXSTEP_Base_InitTypeDef SIXSTEP_parameters;            /*!< Main SixStep structure*/ 
SIXSTEP_PI_PARAM_InitTypeDef_t PI_parameters;           /*!< SixStep PI regulator structure*/ 
static SIXSTEP_RealDirection_t SIXSTEP_real_direction;

bool temp_alarm = false;

bool com_flag = false;
uint32_t hard_overcurrent_cnt = 0;
uint32_t overcur_cnt = 0;
uint32_t heat_cnt = 0;

typedef void (*CommFunc_TypeDef)(uint8_t,uint8_t,uint16_t);

typedef struct
{
	CommFunc_TypeDef 	func;
	uint8_t				step;
	uint8_t				bemf_ch;
} CommTableEntry_TypeDef;

static void CommTableEntryDisabled(uint8_t,uint8_t,uint16_t);

static const CommTableEntry_TypeDef BLDC_COMM_TABLE_FORWARD[8] =
{
		{CommTableEntryDisabled, 		0,	0},
		{L6398_ECH2CH3_DCH1_IO_Write,	3,	1},
		{L6398_ECH1CH3_DCH2_IO_Write, 	5,	2},
		{L6398_ECH1CH2_DCH3_IO_Write,	4,	3},
		{L6398_ECH1CH2_DCH3_IO_Write, 	1,	3},
		{L6398_ECH1CH3_DCH2_IO_Write,	2,	2},
		{L6398_ECH2CH3_DCH1_IO_Write, 	6,	1},
		{CommTableEntryDisabled,		0,	0}
};

static const CommTableEntry_TypeDef BLDC_COMM_TABLE_BACKWARD[8] =
{
		{CommTableEntryDisabled, 		0,	0},
		{L6398_ECH2CH3_DCH1_IO_Write, 	6,	1},
		{L6398_ECH1CH3_DCH2_IO_Write, 	2,	2},
		{L6398_ECH1CH2_DCH3_IO_Write, 	1,	3},
		{L6398_ECH1CH2_DCH3_IO_Write,	4,	3},
		{L6398_ECH1CH3_DCH2_IO_Write,	5,	2},
		{L6398_ECH2CH3_DCH1_IO_Write,	3,	1},
		{CommTableEntryDisabled,		0}
};

/* Variables -----------------------------------------------------------------*/
extern EPROM_Driver_ConfigTypeDef EPROM_Data;
uint8_t Rotor_poles_pairs;                         /*!<  Number of pole pairs of the motor */
uint32_t mech_accel_hz = 0;                         /*!<  Hz -- Mechanical acceleration rate */
int32_t Mech_Speed_RPM = 0;                         /*!<  Mechanical motor speed */
int32_t El_Speed_Hz = 0;                            /*!<  Electrical motor speed */
uint8_t Enable_start_button = TRUE;                 /*!<  Start/stop button filter to avoid double command */
uint16_t shift_n_sqrt = 14;                         /*!<  Shift of bits for root square function */
/*__IO uint32_t uwTick = 0;*/                           /*!<  Tick counter - 1msec updated */
//int32_t speed_sum_sp_filt = 0;                      /*!<  Variable for speed filter */
//int32_t speed_sum_pot_filt = 0;                     /*!<  Variable for potentiometer filter */
//uint16_t index_pot_filt = 1;                        /*!<  Variable for potentiometer filter */
//int16_t potent_filtered = 0;                        /*!<  Variable for potentiometer filter */
uint8_t synchronous_rect = SYNCHRONOUS_RECTIFICATION; /*!<  Synchronous rectification enable flag */
/*
#define MECH_SPEED_UPDATE_Pos	(0)
static __IO uint32_t update_flags = 0;
*/
static median_context_t speed_filter_ctx;

static __IO int16_t commutation_count = 0;

static __IO uint32_t adc_ready_flag = 0;
static uint16_t index_adc_chn = 0;                         /*!<  Index of ADC channel selector for measuring */
static median_context_t median_filter_ctx[ADC_REG_CHANNEL_NUM];
static uint16_t ADC_SEQ_Buffer_Filtered[ADC_REG_CHANNEL_NUM];
static uint8_t speed_target_lock;
static uint32_t error;

/* 6Step Not-Exported functions ----------------------------------------------*/
int16_t MC_PI_Controller(SIXSTEP_PI_PARAM_InitTypeDef_t *, int16_t);
uint64_t MCM_Sqrt(uint64_t );
int32_t MC_GetElSpeedHz(void);
int32_t MC_GetMechSpeedRPM(void);
//void MC_Set_PI_param(SIXSTEP_PI_PARAM_InitTypeDef_t *);
void MC_TIMx_SixStep_timebase(void);
void MC_SixStep_DisableInput_CH1_D_CH2_D_CH3_D(void);
void MC_SixStep_Start_PWM_driving(void);
void MC_SixStep_Stop_PWM_driving(void);
void MC_SixStep_HF_TIMx_SetDutyCycle_CH1(uint16_t);    
void MC_SixStep_HF_TIMx_SetDutyCycle_CH2(uint16_t);    
void MC_SixStep_HF_TIMx_SetDutyCycle_CH3(uint16_t);   
void MC_SixStep_Current_Reference_Start(void);         
void MC_SixStep_Current_Reference_Stop(void);          
void MC_SixStep_Current_Reference_Setvalue(uint16_t);  
void MC_SixStep_Init_main_data(void);
void MC_Task_Speed(void);
void MC_Filter_Init(void);
void MC_Speed_Filter(void);



static void CommTableEntryDisabled(uint8_t step_position,uint8_t sync_rect, uint16_t CCR_value)
{
	UNUSED(step_position);
	UNUSED(sync_rect);
	UNUSED(CCR_value);
	L6398_DCH1CH2CH3_IO_Write();
}

void MC_Commutate(void)
{
	if (!(error & 0x1F)) {
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	}

	HF_TIMx.Instance->CR2 &= ~TIM_CR2_CCPC;
	HF_TIMx.Instance->CCMR1 &= ~TIM_CCMR1_OC1PE;
	HF_TIMx.Instance->CCMR1 &= ~TIM_CCMR1_OC2PE;
	HF_TIMx.Instance->CCMR2 &= ~TIM_CCMR2_OC3PE;
//	HF_TIMx.Instance->CR2 |= TIM_CR2_CCPC;
//	HF_TIMx.Instance->CCMR1 |= TIM_CCMR1_OC1PE;
//	HF_TIMx.Instance->CCMR1 |= TIM_CCMR1_OC2PE;
//	HF_TIMx.Instance->CCMR2 |= TIM_CCMR2_OC3PE;

//	HF_TIMx.Instance->BDTR |= TIM_BDTR_BKE;


	static uint8_t lasthallpos = 0;
	const CommTableEntry_TypeDef *entry = NULL;
	uint8_t hallpos = GetHallState();
	int32_t speed_tmp = Mech_Speed_RPM;
	SIXSTEP_parameters.speed_fdbk = speed_tmp;

	if (SIXSTEP_parameters.RUN_Motor == 0) return;
	if (speed_tmp < 0)	speed_tmp = -speed_tmp;
	//if (SIXSTEP_parameters.pulse_value < 199) lasthallpos = 0;
	if (speed_tmp < MIN_SPEED) lasthallpos = 0;
	if (lasthallpos == hallpos) {
//		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		return;
	}
	lasthallpos = hallpos;
	if (hallpos > 0 && hallpos < 7)
	{
		if (SIXSTEP_parameters.CW_CCW == 0)
		{
			entry = &BLDC_COMM_TABLE_FORWARD[hallpos];
		}
		else
		{
			entry = &BLDC_COMM_TABLE_BACKWARD[hallpos];
		}
//		if (SIXSTEP_parameters.pulse_value != 0) {
//			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
//			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//		}
		entry->func(entry->step, synchronous_rect, SIXSTEP_parameters.pulse_value);
		SIXSTEP_parameters.Current_BEMF_ch = entry->bemf_ch;
	}
	HF_TIMx.Instance->EGR = TIM_EVENTSOURCE_COM;
//	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	//	com_flag = true;
}

/*
void MC_HallTimerOerloadCB(void)
{
	SIXSTEP_parameters.hall_capture = 0xFFFFFFFF;
}
*/
/*
void MC_IncCommCount(void)
{
	HAL_TIM_Base_Stop(&HALL_COUNTER_TIMx);
	SIXSTEP_parameters.hall_capture = __HAL_TIM_GET_COUNTER(&HALL_COUNTER_TIMx);
	__HAL_TIM_URS_ENABLE(&HALL_COUNTER_TIMx);
	HALL_COUNTER_TIMx.Instance->EGR = TIM_EVENTSOURCE_UPDATE;
	__HAL_TIM_URS_DISABLE(&HALL_COUNTER_TIMx);
	__HAL_TIM_ENABLE_IT(&HALL_COUNTER_TIMx, TIM_IT_UPDATE);
	HAL_TIM_Base_Start(&HALL_COUNTER_TIMx);
	if (SIXSTEP_parameters.CW_CCW == 0)
	{
		commutation_count++;
	}
	else
	{
		commutation_count--;
	}
	SIXSTEP_parameters.hall_ok = 1;
	BIT_BAND_SRAM(update_flags, MECH_SPEED_UPDATE) = 1;
}
*/
void MC_IncCommCount(void)
{
	uint8_t direction;

	BIT_BAND_SRAM(SIXSTEP_parameters.hall_ok, 0) = 1;
	LF_TIMx.Instance->EGR = TIM_EVENTSOURCE_CC1;
	//LF_TIMx.Instance->EGR = TIM_EVENTSOURCE_TRIGGER;
	SIXSTEP_parameters.hall_capture = __HAL_TIM_GET_COMPARE(&LF_TIMx,TIM_CHANNEL_1);
	SIXSTEP_parameters.commutation_delay = COMMUTATION_DELAY/*SIXSTEP_parameters.hall_capture>>1*/;
	__HAL_TIM_SET_COMPARE(&LF_TIMx,TIM_CHANNEL_2,SIXSTEP_parameters.commutation_delay);
	LF_TIMx.Instance->EGR = TIM_EVENTSOURCE_UPDATE;
	if (MC_GetRealDirection(&direction))
	{
		if (direction == 0)
		{
			commutation_count++;
		}
		else
		{
			commutation_count--;
		}
	}
}

int16_t MC_GetCommCounter(void)
{
	int16_t cnt = 0;
	ATOMIC_BLOCK_RESTORE
	{
		cnt = commutation_count;
		//commutation_count = 0;
	}
	return cnt;
}

void MC_ResetCommCounter(void)
{
	ATOMIC_BLOCK_RESTORE
	{
		commutation_count = 0;
	}
}


void MC_CommutationCallBack(void)
{
	BIT_BAND_SRAM(SIXSTEP_parameters.hall_ok, 0) = 0;
}

void MC_ChangeDirection(void)
{
	//ATOMIC_BLOCK_RESTORE
	{
		BIT_BAND_SRAM(SIXSTEP_parameters.CW_CCW, 0) ^= 1;
		BIT_BAND_SRAM(SIXSTEP_parameters.target_CW_CWW, 0) = BIT_BAND_SRAM(SIXSTEP_parameters.CW_CCW, 0);
	}
}

uint8_t MC_GetDirection(void)
{
	return BIT_BAND_SRAM(SIXSTEP_parameters.CW_CCW, 0);
}

void MC_SetDirection(uint8_t dir)
{
	dir = dir ? 1 : 0;
	BIT_BAND_SRAM(SIXSTEP_parameters.CW_CCW, 0) = BIT_BAND_SRAM(SIXSTEP_parameters.target_CW_CWW, 0) = dir;
}


uint8_t MC_GetTargetDirection(void)
{
	return BIT_BAND_SRAM(SIXSTEP_parameters.target_CW_CWW, 0);
}

void MC_SetTargetDirection(uint8_t dir)
{
	dir = dir ? 1 : 0;

	if (MC_GetDirection() != dir && MC_GetDirection() == MC_GetTargetDirection())
	{
		BIT_BAND_SRAM(SIXSTEP_parameters.target_CW_CWW, 0) = dir;
//		RampMotorSetFlag(RAMP_REVERSE_Pos);
		MC_ChangeDirection();
	}
}

/*
void MC_SetMechSpeedUpdate(void)
{
	BIT_BAND_SRAM(update_flags, MECH_SPEED_UPDATE) = 1;
}
*/

void MC_SetAcceleration(uint32_t accel)
{
	ATOMIC_BLOCK_RESTORE
	{
		SIXSTEP_parameters.ACCEL = accel;
	}
}

uint32_t MC_GetAcceleration(void)
{
	uint32_t accel = 0;
	ATOMIC_BLOCK_RESTORE
	{
		accel = SIXSTEP_parameters.ACCEL;
	}
	return accel;
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void MC_EnableBrake(void)
{

}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

uint8_t MC_GetPolePairs(void)
{
	uint8_t polepairs;
	polepairs = SIXSTEP_parameters.NUMPOLESPAIRS;
	return polepairs;
}

void MC_SetPolePairs(uint8_t polepairs)
{
	SIXSTEP_parameters.NUMPOLESPAIRS = Rotor_poles_pairs = polepairs;
}

void MC_EnablePotentiometer(uint8_t on_off)
{
	SIXSTEP_parameters.Potentiometer = on_off;
}

bool MC_IsPotentiometerActive(void)
{
	return SIXSTEP_parameters.Potentiometer != 0;
}

SIXSTEP_Base_SystStatus_t MC_GetMotorStatus()
{
	return SIXSTEP_parameters.STATUS;
}

uint8_t MC_IsMotorRun()
{
	return SIXSTEP_parameters.RUN_Motor;
}
/*----------------------------------------------------------------------------*/
void MC_ShortCoils(void)
{
	uint32_t tmpccmrx = 0;

	HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1E;  //disable
	HF_TIMx.Instance->CCER &= ~TIM_CCER_CC1NE;  //disable
	HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2E;  //disable
	HF_TIMx.Instance->CCER &= ~TIM_CCER_CC2NE;  //disable
	HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3E;  //disable
	HF_TIMx.Instance->CCER &= ~TIM_CCER_CC3NE;  //disable

	HF_TIMx.Instance->HF_TIMx_CCR1 =
			HF_TIMx.Instance->HF_TIMx_CCR2 =
					HF_TIMx.Instance->HF_TIMx_CCR3 = HF_TIMx.Instance->ARR;

	tmpccmrx = HF_TIMx.Instance->CCMR1;
	tmpccmrx &= ~TIM_CCMR1_OC1M;
	tmpccmrx &= ~TIM_CCMR1_CC1S;
	tmpccmrx |= TIM_OCMODE_PWM1;
	HF_TIMx.Instance->CCMR1 = tmpccmrx;
	HF_TIMx.Instance->CCER |= TIM_CCER_CC1NE;   //enable

	tmpccmrx = HF_TIMx.Instance->CCMR1;
	tmpccmrx &= ~TIM_CCMR1_OC2M;
	tmpccmrx &= ~TIM_CCMR1_CC2S;
	tmpccmrx |= TIM_OCMODE_PWM1<<8;
	HF_TIMx.Instance->CCMR1 = tmpccmrx;
	HF_TIMx.Instance->CCER |= TIM_CCER_CC2NE;	//enable

	tmpccmrx = HF_TIMx.Instance->CCMR2;
	tmpccmrx &= ~TIM_CCMR2_OC3M;
	tmpccmrx &= ~TIM_CCMR2_CC3S;
	tmpccmrx |= TIM_OCMODE_PWM1;
	HF_TIMx.Instance->CCMR2 = tmpccmrx;
	HF_TIMx.Instance->CCER |= TIM_CCER_CC3NE;	//enable
}
/*----------------------------------------------------------------------------*/

void MC_SetRawPulseValue(uint16_t pulse_value)
{
	ATOMIC_BLOCK_RESTORE
	{
		SIXSTEP_parameters.pulse_value = pulse_value;
	}
}

uint16_t MC_GetRawPulseValue(void)
{
	return SIXSTEP_parameters.pulse_value;
}

void MC_SetSpeedTarget(uint16_t speed)
{
/*
	if (BIT_BAND_SRAM(speed_target_lock, 0) == 0)
	{
		uint16_t pulse_value = MC_GetRawPulseValue();
		SIXSTEP_parameters.speed_target = speed;
		if (speed > pulse_value)
		{
			RampMotorSetFlag(RAMP_SPEED_UP_Pos);
		}
		else if (speed < pulse_value)
		{
			RampMotorSetFlag(RAMP_SLOW_DOWN_Pos);
		}
	}
*/
	SIXSTEP_parameters.pulse_value = speed;
}

void MC_SetSpeedTargetLocked(uint16_t value)
{
	BIT_BAND_SRAM(speed_target_lock, 0) = 1;
	SIXSTEP_parameters.speed_target = value;
}

void MC_UnlockSpeedTarget(uint16_t value)
{
	SIXSTEP_parameters.speed_target = value;
	BIT_BAND_SRAM(speed_target_lock, 0) = 0;
}

bool MC_IsSpeedTargetLocked()
{
	return BIT_BAND_SRAM(speed_target_lock, 0);
}

uint16_t MC_GetSpeedTarget(void)
{
	return SIXSTEP_parameters.speed_target;
}


bool MC_FindRealDirection(void)
{
  static uint8_t prev_hall_sector = 0;
  uint8_t current_hall_sector = GetHallState();

  if (prev_hall_sector == 0)
  {
    prev_hall_sector = current_hall_sector;
    BIT_BAND_SRAM(SIXSTEP_real_direction, REALDIR_ACTUAL_Pos) = 0;
    return false;
  }
  if (GetNextHallSector(prev_hall_sector) == current_hall_sector)
  {
    prev_hall_sector = current_hall_sector;
    BIT_BAND_SRAM(SIXSTEP_real_direction, REALDIR_DIRECTION_Pos) = 0;
    BIT_BAND_SRAM(SIXSTEP_real_direction, REALDIR_ACTUAL_Pos) = 1;
    return true;
  }
  else if (GetPrevHallSector(prev_hall_sector) == current_hall_sector)
  {
    prev_hall_sector = current_hall_sector;
    BIT_BAND_SRAM(SIXSTEP_real_direction, REALDIR_DIRECTION_Pos) = 1;
    BIT_BAND_SRAM(SIXSTEP_real_direction, REALDIR_ACTUAL_Pos) = 1;
    return true;
  }
  prev_hall_sector = current_hall_sector;
  BIT_BAND_SRAM(SIXSTEP_real_direction, REALDIR_ACTUAL_Pos) = 0;
  return false;
}

bool MC_GetRealDirection(uint8_t *dir)
{
  if (BIT_BAND_SRAM(SIXSTEP_real_direction, REALDIR_ACTUAL_Pos) != 0)
  {
    *dir = BIT_BAND_SRAM(SIXSTEP_real_direction, REALDIR_DIRECTION_Pos);
    return true;
  }
  return false;
}

void MC_SetCurrentLimit(uint16_t current)
{
	SIXSTEP_parameters.current_limit = current;
}

uint16_t MC_GetCurrentLimit(void)
{
	return SIXSTEP_parameters.current_limit;
}

void MC_SetTemperatureLimit(uint16_t temp)
{
	SIXSTEP_parameters.temperature_limit = temp;
}

uint16_t MC_GetTemperatureLimit(void)
{
	return SIXSTEP_parameters.temperature_limit;
}


void MC_CheckCurrent(uint16_t current)
{
	if (current > MC_GetCurrentLimit())
	{
		MC_StopMotor();
		MC_SetError(MC_OVERCURRENT_Pos);
//		BIT_BAND_SRAM(error, MC_OVERCURRENT_Pos) = 1;
	}
}

void MC_CheckTemperature(uint16_t temperature)
{
	if (temperature > MC_GetTemperatureLimit())
	{
		MC_StopMotor();
		MC_SetError(MC_OVERHEAT_Pos);
//		BIT_BAND_SRAM(error, MC_OVERHEAT_Pos) = 1;
	}
}

void MC_SetError(uint8_t bit_pos)
{
	if (bit_pos != MC_NOT_INITIALIZED_Pos) {
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		BIT_BAND_SRAM(error, MC_EMERGENCY_STOP_Pos) = 1;
	}
	BIT_BAND_SRAM(error, bit_pos) = 1;
}

void MC_ClearError(uint8_t bit_pos)
{
	BIT_BAND_SRAM(error, bit_pos) = 0;
}

uint32_t MC_GetError()
{
	return error;
}

void MC_ResetError()
{
//	error = 0;
	BIT_BAND_SRAM(error, MC_OVERCURRENT_Pos) = 0;
	BIT_BAND_SRAM(error, MC_OVERHEAT_Pos) = 0;
	BIT_BAND_SRAM(error, MC_HARDWARE_OVERCURRENT_Pos) = 0;
}

void MC_ResetAllErrors()
{
	error &= ~0x1F;
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
}

/*----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------*/

void MC_SixStep_RESET()
{   
 SIXSTEP_parameters.CMD = TRUE; 
 SIXSTEP_parameters.Ireference = STARTUP_CURRENT_REFERENCE;  
 SIXSTEP_parameters.Speed_Loop_Time = SPEED_LOOP_TIME;
 SIXSTEP_parameters.Speed_Loop_Time = SPEED_LOOP_TIME;
 if(!VOLTAGE_MODE)
 {
   SIXSTEP_parameters.pulse_value = SIXSTEP_parameters.HF_TIMx_CCR;
 }
 else 
 {
   SIXSTEP_parameters.pulse_value = DUTY_CYCLE_INIT_VALUE;
 }
 SIXSTEP_parameters.speed_target = MAX_POT_SPEED;
 SIXSTEP_parameters.Speed_Ref_filtered = 0;

 SIXSTEP_parameters.status_prev = 0;
 SIXSTEP_parameters.step_position = 0;

 
 LF_TIMx.Init.Prescaler = SIXSTEP_parameters.LF_TIMx_PSC;
 LF_TIMx.Instance->PSC =  SIXSTEP_parameters.LF_TIMx_PSC;
 LF_TIMx.Init.Period =    SIXSTEP_parameters.LF_TIMx_ARR;
 LF_TIMx.Instance->ARR =  SIXSTEP_parameters.LF_TIMx_ARR; 
 HF_TIMx.Init.Prescaler = SIXSTEP_parameters.HF_TIMx_PSC;
 HF_TIMx.Instance->PSC =  SIXSTEP_parameters.HF_TIMx_PSC;
 HF_TIMx.Init.Period =    SIXSTEP_parameters.HF_TIMx_ARR;
 HF_TIMx.Instance->ARR =  SIXSTEP_parameters.HF_TIMx_ARR;
 HF_TIMx.Instance->HF_TIMx_CCR1 = SIXSTEP_parameters.HF_TIMx_CCR;  

 Rotor_poles_pairs = SIXSTEP_parameters.NUMPOLESPAIRS; 
 SIXSTEP_parameters.SYSCLK_frequency = HAL_RCC_GetSysClockFreq();
 
 MC_SixStep_HF_TIMx_SetDutyCycle_CH1(0);
 MC_SixStep_HF_TIMx_SetDutyCycle_CH2(0);
 MC_SixStep_HF_TIMx_SetDutyCycle_CH3(0); 

 SIXSTEP_parameters.ADC_BEMF_Channel[1] = ADC_Bemf_CH1;   /*BEMF1*/
 SIXSTEP_parameters.ADC_BEMF_Channel[2] = ADC_Bemf_CH2;   /*BEMF2*/
 SIXSTEP_parameters.ADC_BEMF_Channel[3] = ADC_Bemf_CH3;   /*BEMF3*/
 SIXSTEP_parameters.ADC_SEQ_Channel[0] = ADC_CH_1;       /*CURRENT*/
 SIXSTEP_parameters.ADC_SEQ_Channel[1] = ADC_CH_2;       /*SPEED*/
 SIXSTEP_parameters.ADC_SEQ_Channel[2] = ADC_CH_3;       /*VBUS*/
 SIXSTEP_parameters.ADC_SEQ_Channel[3] = ADC_CH_4;       /*TEMP*/
 SIXSTEP_parameters.ADC_SEQ_Channel[4] = ADC_CH_5;       /*TENSO*/
 
 SIXSTEP_parameters.step_position = 0;
 SIXSTEP_parameters.VALIDATION_OK = 0;
 SIXSTEP_parameters.speed_fdbk_filtered = 0;
// SIXSTEP_parameters.Integral_Term_sum = 0;
 SIXSTEP_parameters.Current_Reference = 0;
 SIXSTEP_parameters.Ramp_Start = 0;
 SIXSTEP_parameters.RUN_Motor = 0;
 SIXSTEP_parameters.speed_fdbk = 0;
 SIXSTEP_parameters.SPEED_VALIDATED = FALSE;

 synchronous_rect = SYNCHRONOUS_RECTIFICATION;   
 //uwTick = 0;

 index_adc_chn = 0;
 Mech_Speed_RPM = 0;
 El_Speed_Hz = 0;
 mech_accel_hz = 0;           
 Enable_start_button = TRUE; 
 /*
 speed_sum_sp_filt = 0;
 speed_sum_pot_filt = 0; 
 index_pot_filt = 1;  
 potent_filtered = 0;
 */
 if(PI_parameters.Reference < 0)  
 {
   SIXSTEP_parameters.step_position = 1;
 }
  //MC_Set_PI_param(&PI_parameters);

  if(!VOLTAGE_MODE)
  {
   MC_SixStep_Current_Reference_Setvalue(SIXSTEP_parameters.Ireference); 
  }
  else
  {
   MC_SixStep_Current_Reference_Setvalue(SIXSTEP_parameters.pulse_value); 
  }
  //MC_SixStep_Ramp_Motor_calc();
}

/**
  * @brief  It calculates the square root of a non-negative s64. 
  *   It returns 0 for negative s64.
  * @param  Input uint64_t number
  * @retval int32_t Square root of Input (0 if Input<0)
  */
uint64_t MCM_Sqrt(uint64_t wInput)
{
  uint8_t biter = 0u;
  uint64_t wtemproot;
  uint64_t wtemprootnew;
  
    if (wInput <= (uint64_t)((uint64_t)2097152<<shift_n_sqrt))  
    {
      wtemproot = (uint64_t)((uint64_t)128<<shift_n_sqrt);  
    }
    else
    {
      wtemproot = (uint64_t)((uint64_t)8192<<shift_n_sqrt);  
    }
    
    do
    {
      wtemprootnew = (wtemproot + wInput/wtemproot)>>1;
      if (wtemprootnew == wtemproot)
      {
        biter = (shift_n_sqrt-1);
      }
      else
      {
        biter ++;
        wtemproot = wtemprootnew;
      }
    }
    while (biter < (shift_n_sqrt-1));
  
  return (wtemprootnew); 
}

/** @defgroup MC_Set_PI_param    MC_Set_PI_param
  *  @{
    * @brief Set all parameters for PI regulator
    * @param  PI_PARAM
    * @retval None
*/


/** @defgroup MC_PI_Controller    MC_PI_Controller
  *  @{
    * @brief Compute the PI output for the Current Reference
    * @param  PI_PARAM PI parameters structure
    * @param  speed_fdb motor_speed_value
    * @retval int16_t Currente reference 
*/

//int16_t MC_PI_Controller(SIXSTEP_PI_PARAM_InitTypeDef_t *PI_PARAM, int16_t speed_fdb)
//{
//  int32_t wProportional_Term=0, wIntegral_Term=0, wOutput_32=0,wIntegral_sum_temp=0;
//  int32_t Error =0;
//
//  Error = (PI_PARAM->Reference - speed_fdb);
//
//  /* Proportional term computation*/
//  wProportional_Term = PI_PARAM->Kp_Gain * Error;
//
//  /* Integral term computation */
//  if (PI_PARAM->Ki_Gain == 0)
//  {
//    SIXSTEP_parameters.Integral_Term_sum = 0;
//  }
//  else
//  {
//    wIntegral_Term = PI_PARAM->Ki_Gain * Error;
//    wIntegral_sum_temp = SIXSTEP_parameters.Integral_Term_sum + wIntegral_Term;
//    SIXSTEP_parameters.Integral_Term_sum = wIntegral_sum_temp;
//  }
//
//  if(!VOLTAGE_MODE)
//  {
//    if(SIXSTEP_parameters.Integral_Term_sum> KI_DIV * PI_PARAM->Upper_Limit_Output)
//       SIXSTEP_parameters.Integral_Term_sum = KI_DIV* PI_PARAM->Upper_Limit_Output;
//
//    if(SIXSTEP_parameters.Integral_Term_sum<-KI_DIV* PI_PARAM->Upper_Limit_Output)
//       SIXSTEP_parameters.Integral_Term_sum = -KI_DIV* PI_PARAM->Upper_Limit_Output;
//
//   /* WARNING: the below instruction is not MISRA compliant, user should verify
//               that Cortex-M3 assembly instruction ASR (arithmetic shift right)
//               is used by the compiler to perform the shifts (instead of LSR
//               logical shift right)*/
//
//    wOutput_32 = (wProportional_Term/KP_DIV) + (SIXSTEP_parameters.Integral_Term_sum/KI_DIV);
//  }
//  else
//  {
//    if(SIXSTEP_parameters.Integral_Term_sum> KI_DIV_VM * PI_PARAM->Upper_Limit_Output)
//       SIXSTEP_parameters.Integral_Term_sum = KI_DIV_VM* PI_PARAM->Upper_Limit_Output;
//
//    if(SIXSTEP_parameters.Integral_Term_sum<-KI_DIV_VM* PI_PARAM->Upper_Limit_Output)
//       SIXSTEP_parameters.Integral_Term_sum = -KI_DIV_VM* PI_PARAM->Upper_Limit_Output;
//
//   /* WARNING: the below instruction is not MISRA compliant, user should verify
//               that Cortex-M3 assembly instruction ASR (arithmetic shift right)
//               is used by the compiler to perform the shifts (instead of LSR
//               logical shift right)*/
//
//    wOutput_32 = (wProportional_Term/KP_DIV_VM) + (SIXSTEP_parameters.Integral_Term_sum/KI_DIV_VM);
//  }
//
//  if(PI_PARAM->Reference>0)
//  {
//   if (wOutput_32 > PI_PARAM->Upper_Limit_Output)
//    {
//     wOutput_32 = PI_PARAM->Upper_Limit_Output;
//    }
//   else if (wOutput_32 < PI_PARAM->Lower_Limit_Output)
//    {
//     wOutput_32 = PI_PARAM->Lower_Limit_Output;
//    }
//  }
//  else
//{
//   if (wOutput_32 < (- PI_PARAM->Upper_Limit_Output) )
//    {
//     wOutput_32 = - (PI_PARAM->Upper_Limit_Output);
//    }
//   else if (wOutput_32 > (-PI_PARAM->Lower_Limit_Output))
//    {
//     wOutput_32 = (-PI_PARAM->Lower_Limit_Output);
//    }
//  }
//  return((int16_t)(wOutput_32));
//}

    
/**
  * @} 
  */

/** @defgroup MC_Set_Speed    MC_Set_Speed
  *  @{
    * @brief Set the new motor speed value
    * @param  speed_value:  set new motor speed
    * @retval None
*/
void MC_Set_Speed(uint16_t speed_value)
{
 
  uint8_t change_target_speed = 0;
  int16_t reference_tmp = 0;
  if (SIXSTEP_parameters.Potentiometer)
  {
	  if (SIXSTEP_parameters.Speed_Ref_filtered > SIXSTEP_parameters.speed_target)
	  {
		  if ((SIXSTEP_parameters.Speed_Ref_filtered - SIXSTEP_parameters.speed_target) > ADC_SPEED_TH)
		  {
			  change_target_speed = 1;
		  }
		  else
		  {
			  /* Not change target speed because less than threshold */
		  }
	  }
	  else
	  {
		  if ((SIXSTEP_parameters.speed_target - SIXSTEP_parameters.Speed_Ref_filtered) > ADC_SPEED_TH)
		  {
			  change_target_speed = 1;
		  }
		  else
		  {
			  /* Not change target speed because less than threshold */
		  }
	  }
	  if (change_target_speed == 1)
	  {
		  SIXSTEP_parameters.speed_target = SIXSTEP_parameters.Speed_Ref_filtered;

		  if(SIXSTEP_parameters.CW_CCW == 0)
		  {
			  reference_tmp = SIXSTEP_parameters.Speed_Ref_filtered * MAX_POT_SPEED / 4096;
			  if(reference_tmp <= MIN_POT_SPEED)
			  {
				  PI_parameters.Reference = MIN_POT_SPEED;
			  }
			  else
			  {
				  PI_parameters.Reference =  reference_tmp;
			  }
		  }
		  else
		  {
			  reference_tmp = -(SIXSTEP_parameters.Speed_Ref_filtered * MAX_POT_SPEED / 4096);
			  if(reference_tmp >=- MIN_POT_SPEED)
			  {
				  PI_parameters.Reference = -MIN_POT_SPEED;
			  }
			  else
			  {
				  PI_parameters.Reference=  reference_tmp;
			  }
		  }
	  }
  }
  else
  {
	  //if(speed_value != 0)
		  PI_parameters.Reference = speed_value;
  }
  
}


/** @defgroup MC_StartMotor    MC_StartMotor
  *  @{
    * @brief Start the Motor
    * @retval None
*/
void MC_StartMotor()
{ 
	if (MC_GetError()) {
		return;
	}
	SIXSTEP_parameters.hall_ok = 0;
	SIXSTEP_parameters.hall_ko_successive = 0;
	SIXSTEP_parameters.start_attempts = NUMBER_OF_STARTS;
	SIXSTEP_parameters.run_attempts = SIXSTEP_parameters.start_attempts;
	SIXSTEP_parameters.start_cnt = NUMBER_OF_STEPS;
	//uwTick = 0;
	SIXSTEP_parameters.STATUS = START;
	MC_SixStep_Current_Reference_Start();
	MC_SixStep_Current_Reference_Setvalue(SIXSTEP_parameters.pulse_value);
	MC_SixStep_Start_PWM_driving();
	//HAL_TIM_Base_Start_IT(&LF_TIMx);
	//MC_SixStep_Current_Reference_Start();

	SIXSTEP_parameters.RUN_Motor = 1;
	//BSP_X_NUCLEO_FAULT_LED_ON();
}
/**
  * @} 
  */

/** @defgroup MC_StopMotor    MC_StopMotor
  *  @{
    * @brief Stop the Motor
    * @retval None  
*/
void MC_StopMotor()
{     
	SIXSTEP_parameters.hall_ok = 0;
	//uwTick = 0;
	SIXSTEP_parameters.STATUS = STOP;
	SIXSTEP_parameters.RUN_Motor = 0;
	MC_SixStep_Stop_PWM_driving();
	//MC_SixStep_DisableInput_CH1_D_CH2_D_CH3_D();
	//HAL_TIM_Base_Stop_IT(&LF_TIMx);
	//HAL_ADC_Stop_IT(&ADCx);
	MC_SixStep_Current_Reference_Stop();
	//BSP_X_NUCLEO_FAULT_LED_OFF();
	MC_SixStep_RESET();
}

/**
  * @} 
  */

/** @defgroup MC_GetElSpeedHz    MC_GetElSpeedHz
  *  @{
    * @brief Get the Eletrical Motor Speed from ARR value of LF TIM
    * @retval int32_t Return the electrical motor speed
*/
int32_t MC_GetElSpeedHz()
{   
	uint8_t real_dir;

	if(SIXSTEP_parameters.hall_capture > STEP_DURATION_MINIMUM && SIXSTEP_parameters.hall_capture < STEP_DURATION_MAXIMUM)
	{
		uint16_t prsc = LF_TIMx.Instance->PSC;
		El_Speed_Hz = (int32_t)((SIXSTEP_parameters.SYSCLK_frequency)/(++prsc)/(SIXSTEP_parameters.hall_capture*6));
	}
	else
	{
		El_Speed_Hz = 0;
	}
	if (MC_GetRealDirection(&real_dir))
	{
		if(real_dir != 0)
			return (-El_Speed_Hz);
	}
	return (El_Speed_Hz);
}
/**
  * @} 
  */

/** @defgroup MC_GetMechSpeedRPM    MC_GetMechSpeedRPM
  *  @{
    * @brief Get the Mechanical Motor Speed (RPM)
    * @retval int32_t Return the mechanical motor speed (RPM
*/

int32_t MC_GetMechSpeedRPM()
{  
	Mech_Speed_RPM = (int32_t)(MC_GetElSpeedHz() *  60 / Rotor_poles_pairs);
	return (Mech_Speed_RPM);
}

int32_t MC_GetMechSpeedRPMValue()
{
	return Mech_Speed_RPM;
}


/**
  * @} 
  */

/** @defgroup MC_SixStep_Init_main_data    MC_SixStep_Init_main_data
  *  @{
    * @brief Init the main variables for motor driving from MC_SixStep_param.h
    * @retval None
*/

void MC_SixStep_Init_main_data()
{ 
	SIXSTEP_parameters.Ireference = STARTUP_CURRENT_REFERENCE;
	SIXSTEP_parameters.NUMPOLESPAIRS = NUM_POLE_PAIRS;
	SIXSTEP_parameters.ACCEL = ACC;
	SIXSTEP_parameters.voltage_mode = EPROM_IsInitialized() ? EPROM_Data.FeedBackMode : VOLTAGE_MODE;
	SIXSTEP_parameters.current_limit = EPROM_IsInitialized() ? EPROM_Data.CurrentLimit : CURRENT_LIMIT;
	SIXSTEP_parameters.temperature_limit = EPROM_IsInitialized() ? EPROM_Data.TempearatureLimit : TEMPERATURE_LIMIT;
	if(!VOLTAGE_MODE)
	{
		SIXSTEP_parameters.KP = KP_GAIN;
		SIXSTEP_parameters.KI = KI_GAIN;
	}
	else
	{
		SIXSTEP_parameters.KP = KP_GAIN_VM;
		SIXSTEP_parameters.KI = KI_GAIN_VM;
	}
	SIXSTEP_parameters.CW_CCW = DIRECTION;
	SIXSTEP_parameters.Potentiometer = POTENTIOMETER;
}

/**
  * @} 
  */


/** @defgroup MC_SixStep_INIT    MC_SixStep_INIT
  *  @{
    * @brief Initialitation function for SixStep library
    * @retval None
*/

void MC_SixStep_INIT()
{
    MC_SixStep_Nucleo_Init();
    SIXSTEP_parameters.HF_TIMx_CCR  = HF_TIMx.Instance->HF_TIMx_CCR1;
    SIXSTEP_parameters.HF_TIMx_ARR  = HF_TIMx.Instance->ARR;
    SIXSTEP_parameters.HF_TIMx_PSC  = HF_TIMx.Instance->PSC;  
    SIXSTEP_parameters.LF_TIMx_ARR  = LF_TIMx.Instance->ARR;
    SIXSTEP_parameters.LF_TIMx_PSC  = LF_TIMx.Instance->PSC;

//    MC_SixStep_Current_Reference_Start(); 
//    MC_SixStep_Current_Reference_Setvalue(SIXSTEP_parameters.Ireference); 
    
    //HAL_GPIO_WritePin(GPIO_PORT_BEMF,GPIO_CH_BEMF,GPIO_PIN_SET);

  #ifdef UART_COMM  
    SIXSTEP_parameters.Button_ready = FALSE;
    MC_UI_INIT();                       /*!<  Start the UART Communication Task*/
  #endif    
    
  MC_SixStep_Init_main_data();

  #ifndef UART_COMM
    SIXSTEP_parameters.Button_ready = TRUE;
  #endif
    MC_Filter_Init();
    MC_SixStep_RESET();
    __HAL_TIM_ENABLE_IT(&LF_TIMx, TIM_IT_CC1);
    __HAL_TIM_ENABLE_IT(&LF_TIMx, TIM_IT_CC2);
    HAL_TIM_Base_Start_IT(&LF_TIMx);
    HAL_ADC_Start_IT(&ADCx);
}
      
/**
  * @} 
  */


/** @defgroup MC_TIMx_SixStep_timebase    MC_TIMx_SixStep_timebase
  *  @{
    * @brief Low Frequency Timer Callback - Call the next step and request the filtered speed value
    * @retval None
*/

void MC_TIMx_SixStep_timebase()
{
	//Если датчики Холла не срабатывают, т.е. мотор не крутиться, то пишем в значение
	//максимальную величину
	if (BIT_BAND_SRAM(SIXSTEP_parameters.hall_ok,0) == 0)
	{
		SIXSTEP_parameters.hall_capture = 0xffff;
	}
	Mech_Speed_RPM = MC_GetMechSpeedRPM();
	MC_Commutate();
  //MC_SixStep_NEXT_step(PI_parameters.Reference);                                /*Change STEP number  */
/*
#ifndef HALL_SENSORS
  (SIXSTEP_parameters.ARR_OK == 0)
  {
    MC_SixStep_ARR_step();                                                       //BASE TIMER - ARR modification for STEP frequency changing
  }
#endif
*/
	MC_Speed_Filter();                                                            /*Calculate SPEED filtered  */
}


/** @defgroup MC_Speed_Filter    MC_Speed_Filter
  *  @{
    * @brief Calculate the speed filtered
    * @retval None
*/
void MC_Speed_Filter()
{/*
  speed_sum_sp_filt = 0;
  speed_tmp_array[index_array] = SIXSTEP_parameters.speed_fdbk;
  if(array_completed == FALSE)
  {
    for(int16_t i = (index_array-1); i>=0; i--)
     {
       speed_sum_sp_filt = speed_sum_sp_filt + speed_tmp_array[i];
    }
     index_array++;
      if(index_array >= FILTER_DEEP)
       {
      index_array = 0;
         array_completed = TRUE;
       }
  }
  else
  {
    for(int16_t i = (FILTER_DEEP-1); i >= 0; i--)
  {
      speed_sum_sp_filt = speed_sum_sp_filt + speed_tmp_array[i];
    }
     index_array++;
    if(index_array >= FILTER_DEEP)
       {
      index_array = 0;
       }
  }
  */
  SIXSTEP_parameters.speed_fdbk_filtered = median_filter(SIXSTEP_parameters.speed_fdbk, &speed_filter_ctx);
}



/** @defgroup MC_EXT_button_SixStep    MC_EXT_button_SixStep
  *  @{
    * @brief GPIO EXT Callback - Start or Stop the motor through the Blue push button on STM32Nucleo
    * @retval None
*/

void MC_EXT_button_SixStep()
{
  if(Enable_start_button == TRUE)
  {
    if(SIXSTEP_parameters.RUN_Motor == 0 && SIXSTEP_parameters.Button_ready == TRUE) 
    {    
      MC_StartMotor();
      Enable_start_button = FALSE;      
    }
    else  
    {     
      MC_StopMotor(); 
      Enable_start_button = FALSE;
    }
  }
}

/**
  * @} 
  */
void MC_ADCx_SixStep_Hall()
{
	//uint8_t hall = GetHallState();
	uint8_t bemf_ch_indx = SIXSTEP_parameters.Current_BEMF_ch;
	/*SIXSTEP_parameters.CW_CCW == 0 ? BLDC_COMM_TABLE_FORWARD[hall].bemf_ch :
			BLDC_COMM_TABLE_BACKWARD[hall].bemf_ch;*/

	if (bemf_ch_indx == 0) bemf_ch_indx = 1;
	bemf_ch_indx -= 1;
	if(__HAL_TIM_DIRECTION_STATUS(&HF_TIMx))
	{
		SIXSTEP_parameters.ADC_BEMF_Buffer[bemf_ch_indx] = HAL_ADC_GetValue(&ADCx);

		/******************* SET ADC CHANNEL FOR SPEED/CURRENT/VBUS *******************/
		/* Set the channel for next ADC Regular reading */
		MC_SixStep_ADC_Channel(SIXSTEP_parameters.ADC_SEQ_Channel[index_adc_chn]);
		/******************************************************************************/
	}
	else
	{
		/* Down-counting direction started */
		/* Set the channel for next ADC Regular reading */
		if (SIXSTEP_parameters.ADC_SEQ_Channel[index_adc_chn] == ADC_CH_5)
		{
			ADCTenso_ConvCpltCB(&ADCx);
		}
		else
		{
			SIXSTEP_parameters.ADC_SEQ_Buffer[index_adc_chn] = HAL_ADC_GetValue(&ADCx);
			BIT_BAND_SRAM(adc_ready_flag, index_adc_chn) = 1;
		}
		index_adc_chn++;
		if(index_adc_chn >= ADC_REG_CHANNEL_NUM) index_adc_chn = 0;
		MC_SixStep_ADC_Channel(SIXSTEP_parameters.ADC_BEMF_Channel[bemf_ch_indx]);
	}
}

/*
void MechSpeedTask()
{
	//if (update_flags != 0)
		{
			if (BIT_BAND_SRAM(update_flags, MECH_SPEED_UPDATE) != 0)
			{
				BIT_BAND_SRAM(update_flags, MECH_SPEED_UPDATE) = 0;
				Mech_Speed_RPM = MC_GetMechSpeedRPM();
			}
		}
}
*/


void MC_Reset_Counters(){
//	if (overcur_cnt < MC_OVERCURRENT_MAX_CNT) {
//		MC_ClearError(MC_OVERCURRENT_Pos);
//	}
//	if (hard_overcurrent_cnt < MC_HW_OVERCURRENT_MAX_CNT) {
//		MC_ClearError(MC_HARDWARE_OVERCURRENT_Pos);
//	}
//	if (heat_cnt < MC_OVERHEAT_MAX_CNT) {
//		MC_ClearError(MC_OVERHEAT_Pos);
//	}
	overcur_cnt = 0;
	hard_overcurrent_cnt = 0;
	heat_cnt = 0;
}
/*----------------------------------------------------------------------------*/
void MC_ADC_Task(void)
{
//	static uint32_t temp_cnt = 0;
//	static bool temp_alarm = false;
	//MechSpeedTask();
	ADCTenso_Task();
	if (adc_ready_flag == 0) return;
	for (uint8_t i = 0; i < ADC_REG_CHANNEL_NUM; i++)
	{
		if (BIT_BAND_SRAM(adc_ready_flag, i) != 0)
		{
			BIT_BAND_SRAM(adc_ready_flag, i) = 0;
			ADC_SEQ_Buffer_Filtered[i] = median_filter(SIXSTEP_parameters.ADC_SEQ_Buffer[i], &median_filter_ctx[i]);
			if (i == 0)
			{
				SIXSTEP_parameters.current_fdbk = SIXSTEP_parameters.ADC_SEQ_Buffer[i];
			}

			if (SIXSTEP_parameters.ADC_SEQ_Channel[i] == ADC_CH_4) {
				if (SIXSTEP_parameters.ADC_SEQ_Buffer[i] > SIXSTEP_parameters.temperature_limit) {
//					if (!temp_alarm) {
						if (heat_cnt > MC_OVERHEAT_MAX_CNT) { //2000
//							temp_alarm = true;
							MC_StopMotor();
							MC_SetError(MC_OVERHEAT_Pos);
//							HAL_GPIO_WritePin(PWR_EN_GPIO_Port, PWR_EN_Pin, GPIO_PIN_RESET);
						} else {
							heat_cnt++;
						}
//					}
					HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
				} else {
					if (!(error & 0x1F)) {
						HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
					}
//					temp_cnt = 0;
				}

			}

			if (SIXSTEP_parameters.ADC_SEQ_Channel[i] == ADC_CH_1) {
				if (ADC_SEQ_Buffer_Filtered[i] > SIXSTEP_parameters.current_limit) { // 400
//					if (!temp_alarm) {
						if (overcur_cnt > MC_OVERCURRENT_MAX_CNT) {
//							temp_alarm = true;
							MC_StopMotor();
							MC_SetError(MC_OVERCURRENT_Pos);
//							HAL_GPIO_WritePin(PWR_EN_GPIO_Port, PWR_EN_Pin, GPIO_PIN_RESET);
						} else {
							overcur_cnt++;
						}
//					}
					HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
				} else {
					if (!(error & 0x1F)) {
						HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
					}
//					overcur_cnt = 0;
				}

			}


		}
	}
	SIXSTEP_parameters.current_fdbk_filtered = ADC_SEQ_Buffer_Filtered[0];
	if (SIXSTEP_parameters.Potentiometer != 0)
	{
		SIXSTEP_parameters.Speed_Ref_filtered = ADC_SEQ_Buffer_Filtered[1];
		MC_Set_Speed(SIXSTEP_parameters.Speed_Ref_filtered);
		//MC_SetSpeedTarget((uint16_t)(((uint32_t)SIXSTEP_parameters.Speed_Ref_filtered * SIXSTEP_parameters.HF_TIMx_ARR)>>12));
		MC_Task_Speed();
	}
	//MC_SixStep_Current_Reference_Setvalue((uint16_t)((uint32_t)SIXSTEP_parameters.Speed_Ref_filtered * SIXSTEP_parameters.HF_TIMx_ARR / 4096 ));
}

/*----------------------------------------------------------------------------*/
void MC_Filter_Init(void)
{
	for (uint8_t i = 0; i < ADC_REG_CHANNEL_NUM; i++)
	{
		median_filter_init(&median_filter_ctx[i]);
	}
	ADCTenso_Init();
	median_filter_init(&speed_filter_ctx);
}

/*----------------------------------------------------------------------------*/
/** @defgroup MC_Task_Speed    MC_Task_Speed
  *  @{
    * @brief Main task: Speed Loop with PI regulator
    * @retval None
*/
void MC_Task_Speed()
{
	/*
	if((SIXSTEP_parameters.speed_fdbk_filtered > (target_speed) || SIXSTEP_parameters.speed_fdbk_filtered < (-target_speed)) && SIXSTEP_parameters.VALIDATION_OK !=TRUE)
	{
		SIXSTEP_parameters.STATUS = VALIDATION;
		SIXSTEP_parameters.SPEED_VALIDATED = TRUE;
	}

	if(SIXSTEP_parameters.SPEED_VALIDATED == TRUE && SIXSTEP_parameters.BEMF_OK == TRUE && SIXSTEP_parameters.CL_READY != TRUE)
	{
		SIXSTEP_parameters.CL_READY = TRUE;
	}
	*/
	//if(SIXSTEP_parameters.VALIDATION_OK == TRUE)
	{
		/*****************************************************************************/
		//SIXSTEP_parameters.STATUS = RUN;
		/*****************************************************************************/

		if(PI_parameters.Reference>=0)
		{
			SIXSTEP_parameters.Current_Reference = PI_parameters.Reference;
					/*(uint16_t)MC_PI_Controller(&PI_parameters,(int16_t)SIXSTEP_parameters.speed_fdbk_filtered);*/
		}
		else
		{
			SIXSTEP_parameters.Current_Reference = -PI_parameters.Reference;
					/*(uint16_t)(-MC_PI_Controller(&PI_parameters,(int16_t)SIXSTEP_parameters.speed_fdbk_filtered));*/
		}

		MC_SixStep_Current_Reference_Setvalue((uint16_t)((uint32_t)SIXSTEP_parameters.Current_Reference * SIXSTEP_parameters.HF_TIMx_ARR / MAX_POT_SPEED ));
	}
}

/**
  * @brief This function is called to increment  a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in Systick ISR.
  * @note This function is declared as __weak to be overwritten in case of other 
  *       implementations in user file.
  * @retval None
  */
/*
void HAL_IncTick(void)
{
  uwTick++;
}
*/
/**
  * @brief  Povides a tick value in millisecond.
  * @note   The function is declared as __Weak  to be overwritten  in case of other 
  *       implementations in user file.
  * @retval tick value
  */
/*
uint32_t HAL_GetTick(void)
{
  return uwTick;
}
*/

/**
  * @}  end MC_6-STEP_LIB 
  */

/**
  * @}  end MIDDLEWARES
  */
