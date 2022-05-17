//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 25.11.2017
//Изменен:                25.11.2017
//**************************************************************/

/**\author Новиков В. А.*/
/**\file adctenso.c*/
/**\brief Реализация АЦП отцифровки внешних сигналов тензодатчиков и инклинометров.*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "atomic.h"
#include "proto.h"
#include "adctenso.h"
#include "bitbanding.h"
#include "median.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DECIMATION (1)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t adc_tenso_ready_flag = 0;
static __IO uint8_t CurCh = 0;
static uint8_t decimation_cnt = DECIMATION;
static uint16_t TensoADCBuff[TENSO_CHANNEL_NUM];
static uint16_t TensoBuffFiltered[TENSO_CHANNEL_NUM];
static median_context_t tenso_filter_ctx[TENSO_CHANNEL_NUM];
extern Driver_TelemetryTypeDef DriverTelemetry;
extern ADC_HandleTypeDef hadc1;

/* Private function prototypes -----------------------------------------------*/
static void SelectChannel(uint8_t NewCh);

/* Private functions ---------------------------------------------------------*/
static void SelectChannel(uint8_t NewCh)
{
	uint8_t ch = NewCh & TENSO_CHANNEL_Msk;

	HAL_GPIO_WritePin(ANALOG_SEL_0_GPIO_Port, ANALOG_SEL_0_Pin, (ch & (1<<0)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ANALOG_SEL_1_GPIO_Port, ANALOG_SEL_1_Pin, (ch & (1<<1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Exported functions --------------------------------------------------------*/
/*
void SwTimer_ADCTensoCB(void)
{
	ADC_ChannelConfTypeDef sConfig;

	ATOMIC_BLOCK_RESTORE{
		memcpy(&DriverTelemetry.ADCCode[0], TensoADCBuff, sizeof(TensoADCBuff));
	}
	sConfig.Channel = TENSO_CH;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_ADC_Start_IT(&hadc1);
}
*/
/*----------------------------------------------------------------------------*/
void ADCTenso_Init(void)
{
	for (uint8_t i = 0; i<TENSO_CHANNEL_NUM; i++)
	{
		median_filter_init(&tenso_filter_ctx[i]);
	}
	SelectChannel(CurCh);
}
/*----------------------------------------------------------------------------*/
void ADCTenso_ConvCpltCB(ADC_HandleTypeDef* hadc)
{
	uint8_t ch = CurCh;
	uint8_t tmp_adc_res;

	if (decimation_cnt-- > 0)
	{
		tmp_adc_res = HAL_ADC_GetValue(hadc);
		UNUSED(tmp_adc_res);
	}
	else
	{
		TensoADCBuff[ch] = HAL_ADC_GetValue(hadc);
		BIT_BAND_SRAM(adc_tenso_ready_flag, ch) = 1;
		CurCh = (++ch) & TENSO_CHANNEL_Msk;
		SelectChannel(CurCh);
		decimation_cnt = DECIMATION;
	}
//	uint8_t ch = CurCh;
//
//	TensoADCBuff[ch] = HAL_ADC_GetValue(hadc);
//	BIT_BAND_SRAM(adc_tenso_ready_flag, ch) = 1;
//	CurCh = (++ch) & TENSO_CHANNEL_Msk;
//	SelectChannel(CurCh);
}
/*----------------------------------------------------------------------------*/
void ADCTenso_Task(void)
{
	if (adc_tenso_ready_flag == 0) return;
	for (uint8_t i = 0; i < TENSO_CHANNEL_NUM; i++)
	{
		if (BIT_BAND_SRAM(adc_tenso_ready_flag, i) != 0)
		{
			BIT_BAND_SRAM(adc_tenso_ready_flag, i) = 0;
			TensoBuffFiltered[i] = median_filter(TensoADCBuff[i], &tenso_filter_ctx[i]);
		}
	}
}
/*----------------------------------------------------------------------------*/
uint32_t GetTensoADCBuff(uint16_t **buff_addr_ptr, uint32_t *buff_size_ptr)
{
	*buff_addr_ptr = TensoBuffFiltered;
	*buff_size_ptr = TENSO_CHANNEL_NUM * sizeof(TensoBuffFiltered[0]);
	return 0;
}
/*----------------------------------------------------------------------------*/
