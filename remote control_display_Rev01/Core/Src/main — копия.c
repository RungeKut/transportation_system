/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735.h"
#include "bat.h"
#include "speed.h"
#include "weight.h"
//#include "digp.h"
#include "dig0.h"
#include "dig1.h"
#include "dig2.h"
#include "dig3.h"
#include "dig4.h"
#include "dig5.h"
#include "dig6.h"
#include "dig7.h"
#include "dig8.h"
#include "dig9.h"
uint16_t Disp_Colour = 0;
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static const uint8_t bufferSize = 32;
volatile char tx_str[bufferSize] = {//                                      |  GLOBAL_FLAG_TX  |bat |wght|sped|    
//1    2    3    4    5    6    7    8    9    10   11   12   13   14   15  |16   17   18   19 | 20 | 21 | 22 | 23   24   25   26   27   28   29   30   31   32
0x54,0x72,0x61,0x6e,0x73,0x70,0x6f,0x72,0x74,0x61,0x74,0x69,0x6f,0x6e,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x17,0x17,0x20,0x53,0x79,0x73,0x74,0x65,0x6d,0x0d
};
volatile char rx_str[bufferSize];
uint8_t flagRxEnable = 0;
uint8_t flagTxEnable = 0;
//volatile extern uint8_t timerOff;
volatile uint16_t weight = 0;
volatile uint16_t speed = 0;
volatile uint16_t battery = 0;
static const char rxStrKeyUp[7] = {0x53,0x79,0x73,0x74,0x65,0x6d,0x20};
static const char rxStrKeyDown[16] = {0x20,0x54,0x72,0x61,0x6e,0x73,0x70,0x6f,0x72,0x74,0x61,0x74,0x69,0x6f,0x6e,0x0d};
volatile uint32_t GLOBAL_FLAG_RX = 0;
volatile uint32_t GLOBAL_FLAG_TX = 0;
uint8_t WkupFlag=0;
uint8_t keystrokeCounter = 0;
volatile uint8_t digits[3] = {0,0,0};
uint16_t *disp_w[10] = {&width_dig0, &width_dig1, &width_dig2, &width_dig3, &width_dig4, &width_dig5, &width_dig6, &width_dig7, &width_dig8, &width_dig9};
uint16_t *disp_h[10] = {&height_dig0, &height_dig1, &height_dig2, &height_dig3, &height_dig4, &height_dig5, &height_dig6, &height_dig7, &height_dig8, &height_dig9};
uint16_t *disp_p[10] = {(uint16_t *)dig0, (uint16_t *)dig1, (uint16_t *)dig2, (uint16_t *)dig3, (uint16_t *)dig4, (uint16_t *)dig5, (uint16_t *)dig6, (uint16_t *)dig7, (uint16_t *)dig8, (uint16_t *)dig9};

uint8_t fl=0;
uint8_t dt1;	
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*----------------------------------------------------------------*/
void irq_enable(void)
/*----------------------------------------------------------------*/
{
  /* Peripheral interrupt init*/
  /* PVD_IRQn interrupt configuration */
  NVIC_SetPriority(PVD_IRQn, 0);
  NVIC_EnableIRQ(PVD_IRQn);
  /* RCC_CRS_IRQn interrupt configuration */
  NVIC_SetPriority(RCC_IRQn, 0);
  NVIC_EnableIRQ(RCC_IRQn);
}
/*----------------------------------------------------------------*/
void irq_desable(void)
/*----------------------------------------------------------------*/
{
  /* Peripheral interrupt init*/
  /* PVD_IRQn interrupt configuration */
//  NVIC_SetPriority(PVD_IRQn, 0);
  NVIC_DisableIRQ(PVD_IRQn);
  /* RCC_CRS_IRQn interrupt configuration */
//  NVIC_SetPriority(RCC_CRS_IRQn, 0);
  NVIC_DisableIRQ(RCC_IRQn);
}
/*----------------------------------------------------------------*/
void DebugOnStandByMode(void)
/*----------------------------------------------------------------*/
{
	DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP;
	DBGMCU->CR |= DBGMCU_CR_DBG_STOP;
	DBGMCU->CR |= DBGMCU_CR_DBG_STANDBY;
}
/*----------------------------------------------------------------*/
void GoToStandbyMode(void)
/*----------------------------------------------------------------*/
{
	//DebugOnStandByMode();
//  irq_desable();

/* с точки зрения ядра Cortex-M, что Stop, что Standby - это режим Deep Sleep */
/* поэтому надо в ядре включить Deep Sleep */
	SCB->SCR |=  SCB_SCR_SLEEPDEEP_Msk;
/* флаг PDDS определяет выбор между Stop и Standby, его надо сбросить */
	PWR->CR |= PWR_CR_PDDS;
            
/* флаг Wakeup должн быть очищен, иначе есть шанс проснуться немедленно */    
	PWR->CR |= PWR_CR_CWUF;

/* стабилизатор питания в low-power режим, у нас в Stop потребления-то почти не будет */
//	PWR->CR |= PWR_CR_LPSDSR;

/* источник опорного напряжения Vref выключить автоматически */
//	PWR->CR |= PWR_CR_ULP;

/* выключили прерывания; пробуждению по ним это не помешает */
//unsigned state = irq_disable();

/* завершили незавершённые операция сохранения данных */
	__DSB();

/* заснули */
	__WFI();

/* переинициализация рабочих частот */
//init_clk();
//	SystemClock_Config();
	/* включаем источник 5V */
//	LL_GPIO_SetOutputPin(EN_5V_GPIO_Port, EN_5V_Pin);
/* после просыпания восстановили прерывания */
//irq_restore(state);
//	irq_enable();
}
//=======================================================================
void DMA1_RX_ClearFlag(void) {
	LL_DMA_ClearFlag_TE3(DMA1); //флаг прерывания в случае возникновения ошибки передачи данных в канале
	LL_DMA_ClearFlag_HT3(DMA1); //флаг прерывания по окончанию передачи половины передаваемых данных
	LL_DMA_ClearFlag_TC3(DMA1); //флаг прерывания по окончанию передачи всех данных
	LL_DMA_ClearFlag_GI3(DMA1); //флаг глобального прерывания от событий TE, HT или TC
	return;
}
void DMA1_TX_ClearFlag(void) {
	LL_DMA_ClearFlag_TE2(DMA1); //флаг прерывания в случае возникновения ошибки передачи данных в канале
	LL_DMA_ClearFlag_HT2(DMA1); //флаг прерывания по окончанию передачи половины передаваемых данных
	LL_DMA_ClearFlag_TC2(DMA1); //флаг прерывания по окончанию передачи всех данных
	LL_DMA_ClearFlag_GI2(DMA1); //флаг глобального прерывания от событий TE, HT или TC
	return;
}
void DMA1_RX_InitStart(void) {
//	RCC->AHBENR	|= RCC_AHBENR_DMA1EN;					// Подали тактирование на DMA
	LL_USART_Disable(USART1);
	
	DMA_Disable(DMA1_Channel5);								// Выключили канал.
	DMA_DeInit(DMA1_Channel5);								// Обнулили DMA канал
	
// Настраиваем канал
	DMA_Init( DMA1_Channel5,									// 3-й канал 1 контроллера. 
              (uint32_t)&(USART1->RDR),			// Адрес откуда брать -- адрес регистра DR  в USART1
              (uint32_t)rx_str,							// Адрес куда класть результат
              bufferSize,										// Указываем размер буфера.
              TransCompl_Int_Enable     	|	// Прерывание по окончанию
              HalfCompl_Int_Disable       |	// Прерывание по половине
              TransError_Int_Disable      |	// Прерывание по ошибке
              ReadPerif                   |	// Читаем из периферии
              CircularMode_Disable        |	// Цикличный режим включен
              PeripheralInc_Disable       |	// Адрес периферии не увеличиваем
              MemoryInc_Enable            |	// А вот адрес приемного буфера увеличиваем, перебирая байт за байтом его
              PDataSize_B                 |	// Размер данных из периферии - байт
              MDataSize_B                 |	// Размер данных в памяти - байт
              DMA_Priority_Low            |	// Низкий приоритет
              M2M_Disable                 );// Режим копирования память-память выключен. 
 
//	USART1->ISR &=~(USART_ISR_TC);						// Сбросили флаг передачи
	USART1->CR3 |=USART_CR3_DMAR;							// Включили сигнал от приема на DMA. В данном случае за UART1 на RX закреплен DMA1_Channel5)
	USART1->CR3 &=~USART_CR3_DMAT;
	USART1->ICR |=USART_ICR_EOBCF;
	USART1->ICR |=USART_ICR_ORECF;
	LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_RX);
	LL_USART_EnableDMAReq_RX(USART1);
// Включаем DMA
	DMA_Enable(DMA1_Channel5);
	LL_USART_Enable(USART1);
	return;
}
void DMA1_RX_Start(void) {
	DMA_Enable(DMA1_Channel5);
	return;
}
void DMA1_TX_InitStart(void) {
//	RCC->AHBENR	|= RCC_AHBENR_DMA1EN;					// Подали тактирование на DMA
	LL_USART_Disable(USART1);
	
	DMA_Disable(DMA1_Channel4);								// Выключили канал.
	DMA_DeInit(DMA1_Channel4);								// Сбросили все настройки
  
	// Настраиваем канал
  DMA_Init( DMA1_Channel4,									// Канал будет 2 контроллера 1. именно этот канал пинает USART1 при передаче
              (uint32_t)&(USART1->TDR),			// Указываем адрес регистра данных USART
              (uint32_t)tx_str,							// Указываем адрес буфера для передачи. В него предварительно записаны данные.
              bufferSize,										// Указываем размер буфера. 
              TransCompl_Int_Enable      	|	// Прерывание по окончанию
              HalfCompl_Int_Disable       |	// Прерывание по половине посылке
              TransError_Int_Disable      |	// Прерывание по ошибке
              ReadMemory                  |	// Читаем из памяти (а пишем в периферию)
              CircularMode_Disable        |	// Циклический режим
              PeripheralInc_Disable       |	// Адрес периферии не меняем
              MemoryInc_Enable            |	// А вот адрес памяти наоборот увеличиваем
              PDataSize_B                 |	// Данные размером в байт в периферии
              MDataSize_B                 |	// Данные размером в байт в памяти
              DMA_Priority_Low            |	// Низкий приоритет
              M2M_Disable                 );// Не из памяти в память.
 
//	DMA1->IFCR = DMA_IFCR_CTCIF4;							// Сбрасываем флажок бита пустого регистра. Чтобы не отправить сразу нулевой байт
	USART1->CR3 |= USART_CR3_DMAT;						// Подключаем канал DMA к UART. Установив бит отвечающий за пинание уартом дма канала.
  USART1->CR3 &= ~USART_CR3_DMAR;
	LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX);
	LL_USART_EnableDMAReq_TX(USART1);
// Включаем DMA
	DMA_Enable(DMA1_Channel4);
	LL_USART_Enable(USART1);
	return;
}
void DMA1_TX_Start(void) {
	DMA_Enable(DMA1_Channel4);
	return;
}
void DMA1_RX_Stop(void) {
//	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
	LL_USART_Disable(USART1);
	DMA_Disable(DMA1_Channel5);
//  (void) USART1->TDR;
	return;
}
void DMA1_TX_Stop(void) {
//	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
	LL_USART_Disable(USART1);
	DMA_Disable(DMA1_Channel4);
//	(void) USART1->RDR;
	return;
}
void USART1_IRQEnable(void) {
	LL_USART_EnableIT_TXE(USART1); //Регистр данных передачи пуст
	LL_USART_EnableIT_CTS(USART1); //Можно продолжить передачу
	LL_USART_EnableIT_RXNE(USART1);//Принятые данные готовы для чтения либо ошибка переполнения(прерывание по заполнению буфера)
	LL_USART_EnableIT_TC(USART1);  //Передача завершена
	LL_USART_EnableIT_ERROR(USART1); //вкл прерывание при обнаружении ошибок
	return;
}
//=======================================================================
void Vibro_touch(void) {
	LL_GPIO_TogglePin(VIBRO_GPIO_Port, VIBRO_Pin);
	return;
}
//=======================================================================
void ScanButton(void) {
	if	(LL_GPIO_IsInputPinSet(SA1_GPIO_Port, SA1_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_DOWN_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
	} else {
		GLOBAL_FLAG_RX &= ~BUT_DOWN_FLAG;
	}
	if	(LL_GPIO_IsInputPinSet(SA2_GPIO_Port, SA2_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_UP_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
	} else {
		GLOBAL_FLAG_RX &= ~BUT_UP_FLAG;
	}
	if	(LL_GPIO_IsInputPinSet(SA3_GPIO_Port, SA3_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_BACKWARD_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
	} else {
		GLOBAL_FLAG_RX &= ~BUT_BACKWARD_FLAG;
	}
	if	(LL_GPIO_IsInputPinSet(SA4_GPIO_Port, SA4_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_FORWARD_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
	} else {
		GLOBAL_FLAG_RX &= ~BUT_FORWARD_FLAG;
	}
	if	(LL_GPIO_IsInputPinSet(SA5_GPIO_Port, SA5_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_WEAK_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
//		keystrokeCounter++;
	} else {
		GLOBAL_FLAG_RX &= ~BUT_WEAK_FLAG;
	}
	if	(LL_GPIO_IsInputPinSet(SA6_GPIO_Port, SA6_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_STRONG_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
//		keystrokeCounter++;
	} else {
		GLOBAL_FLAG_RX &= ~BUT_STRONG_FLAG;
	}
		if	(LL_GPIO_IsInputPinSet(SA7_GPIO_Port, SA7_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_SPEED_DOWN_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
			speed--;
	} else {
		GLOBAL_FLAG_RX &= ~BUT_SPEED_DOWN_FLAG;
	}
		if	(LL_GPIO_IsInputPinSet(SA8_GPIO_Port, SA8_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_SPEED_UP_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
			speed++;
	} else {
		GLOBAL_FLAG_RX &= ~BUT_SPEED_UP_FLAG;
	}
/*		if	(LL_GPIO_IsInputPinSet(SA9_GPIO_Port, SA9_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_-_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
		keystrokeCounter++;
	} else {
		GLOBAL_FLAG_RX &= ~BUT_-_FLAG;
	}
		if	(LL_GPIO_IsInputPinSet(SA10_GPIO_Port, SA10_Pin) == 1) {
		GLOBAL_FLAG_RX |= BUT_-_FLAG;
		LL_TIM_SetCounter(TIM3, 0xFFFF);
		keystrokeCounter++;
	} else {
		GLOBAL_FLAG_RX &= ~BUT_-_FLAG;
	}*/
}
void ClearGlobalFlagTX(void) {
	GLOBAL_FLAG_TX &= ~0x7f;
}
void ButtonControlWithRule(void) {
	if ((flagRxEnable == 0) && (flagTxEnable == 0)) { 
//		ClearGlobalFlagTX();
		if (((GLOBAL_FLAG_RX & BUT_STRONG_FLAG))		&&	
				((GLOBAL_FLAG_RX & BUT_WEAK_FLAG))			&&
				(!(GLOBAL_FLAG_RX & BUT_UP_FLAG))				&&
				(!(GLOBAL_FLAG_RX & BUT_DOWN_FLAG))			&&
				(!(GLOBAL_FLAG_RX & BUT_FORWARD_FLAG))	&&
				(!(GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG)))
		{
//			GLOBAL_FLAG_TX |= INITIALIZATION_FLAG;
//			flagTxEnable = 1;
//			DMA1_TX_InitStart();
		}
		else if (((GLOBAL_FLAG_RX & BUT_STRONG_FLAG))		&&	
						(!(GLOBAL_FLAG_RX & BUT_WEAK_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_UP_FLAG))				&&
						(!(GLOBAL_FLAG_RX & BUT_DOWN_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_FORWARD_FLAG))	&&
						(!(GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG)))
		{
			GLOBAL_FLAG_TX |= BUT_STRONG_FLAG;
			Vibro_touch();
//			timerOff = 50;
//			flagTxEnable = 1;
//			DMA1_TX_InitStart();
		}
		else if ((!(GLOBAL_FLAG_RX & BUT_STRONG_FLAG))	&&	
						((GLOBAL_FLAG_RX & BUT_WEAK_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_UP_FLAG))				&&
						(!(GLOBAL_FLAG_RX & BUT_DOWN_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_FORWARD_FLAG))	&&
						(!(GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG)))
		{
			GLOBAL_FLAG_TX |= BUT_WEAK_FLAG;
			Vibro_touch();
//			timerOff = 50;
//			flagTxEnable = 1;
//			DMA1_TX_InitStart();
		}
		else if ((!(GLOBAL_FLAG_RX & BUT_STRONG_FLAG))	&&	
						(!(GLOBAL_FLAG_RX & BUT_WEAK_FLAG))			&&
						((GLOBAL_FLAG_RX & BUT_UP_FLAG))				&&
						(!(GLOBAL_FLAG_RX & BUT_DOWN_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_FORWARD_FLAG))	&&
						(!(GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG)))
		{
			GLOBAL_FLAG_TX |= BUT_UP_FLAG;
			Vibro_touch();
//			flagTxEnable = 1;
//			DMA1_TX_InitStart();
		}
		else if ((!(GLOBAL_FLAG_RX & BUT_STRONG_FLAG))	&&	
						(!(GLOBAL_FLAG_RX & BUT_WEAK_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_UP_FLAG))				&&
						((GLOBAL_FLAG_RX & BUT_DOWN_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_FORWARD_FLAG))	&&
						(!(GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG)))
		{
			GLOBAL_FLAG_TX |= BUT_DOWN_FLAG;
			Vibro_touch();
//			flagTxEnable = 1;
//			DMA1_TX_InitStart();
		}
		else if ((!(GLOBAL_FLAG_RX & BUT_STRONG_FLAG))	&&	
						(!(GLOBAL_FLAG_RX & BUT_WEAK_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_UP_FLAG))				&&
						(!(GLOBAL_FLAG_RX & BUT_DOWN_FLAG))			&&
						((GLOBAL_FLAG_RX & BUT_FORWARD_FLAG))		&&
						(!(GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG)))
		{
			GLOBAL_FLAG_TX |= BUT_FORWARD_FLAG;
			Vibro_touch();
//			flagTxEnable = 1;
//			DMA1_TX_InitStart();
		}
		else if ((!(GLOBAL_FLAG_RX & BUT_STRONG_FLAG))	&&	
						(!(GLOBAL_FLAG_RX & BUT_WEAK_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_UP_FLAG))				&&
						(!(GLOBAL_FLAG_RX & BUT_DOWN_FLAG))			&&
						(!(GLOBAL_FLAG_RX & BUT_FORWARD_FLAG))	&&
						((GLOBAL_FLAG_RX & BUT_BACKWARD_FLAG)))
		{
			GLOBAL_FLAG_TX |= BUT_BACKWARD_FLAG;
			Vibro_touch();
//			flagTxEnable = 1;
//			DMA1_TX_InitStart();
		}
		else
		{
			ClearGlobalFlagTX();
		}
	}
	else
	{
//		GLOBAL_FLAG_TX = 0;
	}
}
/*----------------------------------------------------------------*/
void Convert2Array(uint8_t num) //Конвертирует десятичное число в массив по разрядам Ex. 257 -> {2,5,7}
/*----------------------------------------------------------------*/
{
	uint8_t digits_count=0;	
	uint8_t limit=3;
	digits[0]=0;
	digits[1]=0;		
	digits[2]=0;
	if (num <= 0) { return; }
	while (num > 0 && limit) 
	{
		digits[2 - digits_count++] = num % 10;
    num /= 10;
    limit--;
  }
	return;
};
void DisplayOut(uint8_t bat, uint8_t spd, uint8_t wgt)
{
//	lcd_st7735s_fillrect(0,0,127,159,0xFFFF);
	Convert2Array(bat);
	lcd_st7735s_img(66,117, *disp_w[digits[0]], *disp_h[digits[0]], disp_p[digits[0]]);
	lcd_st7735s_img(39,117, *disp_w[digits[1]], *disp_h[digits[1]], disp_p[digits[1]]);
	lcd_st7735s_img(12,117, *disp_w[digits[2]], *disp_h[digits[2]], disp_p[digits[2]]);
	Convert2Array(spd);
	lcd_st7735s_img(39,62, *disp_w[digits[1]], *disp_h[digits[1]], disp_p[digits[1]]);
	lcd_st7735s_img(12,62, *disp_w[digits[2]], *disp_h[digits[2]], disp_p[digits[2]]);
	Convert2Array(wgt);
	lcd_st7735s_img(39,7, *disp_w[digits[1]], *disp_h[digits[1]], disp_p[digits[1]]);
	lcd_st7735s_img(12,7, *disp_w[digits[2]], *disp_h[digits[2]], disp_p[digits[2]]);
}
//=======================================================================
uint16_t  USART_RX_TX_Str (uint8_t* tx_dt, uint8_t* rx_dt)
{
	uint16_t ind = 0;
  while (!fl) {}
  fl=0;
  rx_dt[ind] = dt1;
	while (!LL_USART_IsActiveFlag_TXE(USART1)) {}
  LL_USART_TransmitData8(USART1,*(uint8_t*)(tx_dt+ind));
	while(rx_dt[ind])
  {
		ind++;
		while (!fl) {}
		fl=0;
		rx_dt[ind] = dt1;
    while (!LL_USART_IsActiveFlag_TXE(USART1)) {}
    LL_USART_TransmitData8(USART1,*(uint8_t*)(tx_dt+ind));
  }
	return ind;
}
//=======================================================================
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
/*
if(LL_PWR_IsActiveFlag_SB() != RESET)//Если МК вышел из режима standby
{
	LL_PWR_ClearFlag_SB(); //Сброс флага, который указывает на то, что МК вышел из режима standby
	WkupFlag=1;
}
else
WkupFlag=0;

RCC_MSICmd(ENABLE);
while (RCC_GetFlagStatus(RCC_FLAG_MSIRDY) == RESET)
{}

RCC_MSIRangeConfig(RCC_MSIRange_2MHz);
RCC_SYSCLKConfig(RCC_SYSCLKSource_MSI);
RCC_HSICmd(DISABLE);
PWR_PVDCmd(DISABLE);
PWR_UltraLowPowerCmd(ENABLE);
PWR_WakeUpPinCmd(PWR_WakeUpPin_1,ENABLE);
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
PWR_UltraLowPowerCmd(ENABLE);

if(WkupFlag == 0) //Если МК до этого не находился в режиме STANDBY
{
	PWR_EnterSTANDBYMode(); //Перейди в режим STANDBY	
}*/
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* Peripheral interrupt init*/
  /* PVD_IRQn interrupt configuration */
  NVIC_SetPriority(PVD_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(PVD_IRQn);
  /* RCC_IRQn interrupt configuration */
  NVIC_SetPriority(RCC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(RCC_IRQn);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_DMA_Init();
//  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
/*	
LL_GPIO_SetOutputPin(Seg_1_GPIO_Port, Seg_1_Pin);
LL_GPIO_SetOutputPin(Seg_2_GPIO_Port, Seg_2_Pin);
LL_GPIO_SetOutputPin(Seg_3_GPIO_Port, Seg_3_Pin);
LL_GPIO_SetOutputPin(Seg_4_GPIO_Port, Seg_4_Pin);
		LL_GPIO_ResetOutputPin(Seg_a_GPIO_Port, Seg_a_Pin);
		LL_GPIO_ResetOutputPin(Seg_b_GPIO_Port, Seg_b_Pin);
		LL_GPIO_ResetOutputPin(Seg_c_GPIO_Port, Seg_c_Pin);
		LL_GPIO_SetOutputPin(Seg_d_GPIO_Port, Seg_d_Pin);
*/
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;//вкл тактирование PWR
	LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
	
//	LL_PWR_EnableUltraLowPower();
//	LL_PWR_EnableLowPowerRunMode();
//	LL_PWR_EnableFastWakeUp();
	
/*	DMA1_RX_Stop();
	DMA1_TX_Stop();
	
	DMA1_RX_ClearFlag();
	DMA1_TX_ClearFlag();
	
  DMA1_TX_InitStart();
	DMA1_TX_Stop();
	DMA1_RX_InitStart();
	DMA1_RX_Stop();*/
	


	LL_TIM_EnableIT_UPDATE(TIM2);
//	LL_TIM_EnableCounter(TIM2);
	LL_TIM_EnableIT_UPDATE(TIM3);
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_EnableIT_UPDATE(TIM6);
//	LL_TIM_EnableCounter(TIM6);
						
	lcd_st7735s_CS_1();
	LL_SPI_Enable(SPI1);
	LL_SPI_TransmitData16(SPI1, 0x01FF);
//	LL_mDelay(10);
	lcd_st7735s_init();
	delaySetNs(1000);
	LL_GPIO_SetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
	LL_GPIO_SetOutputPin(Led1_GPIO_Port, Led1_Pin);
	LL_GPIO_SetOutputPin(Led2_GPIO_Port, Led2_Pin);
	LL_GPIO_SetOutputPin(Led3_GPIO_Port, Led3_Pin);
	lcd_st7735s_fillrect(0,0,127,159,0xFFFF);
	lcd_st7735s_img(100,117, width_bat_img, height_bat_img, bat_img);
	lcd_st7735s_img(70,57, width_speed_img, height_speed_img, speed_img);
	lcd_st7735s_img(70,7, width_weight_img, height_weight_img, weight_img);
	
	LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableIT_ERROR(USART1);
//	lcd_st7735s_img(1,117, width_digp, height_digp, digp);
					
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	ScanButton();
	ButtonControlWithRule();
	DisplayOut(battery, speed, weight);
	LL_GPIO_TogglePin(Led1_GPIO_Port, Led1_Pin);
	USART_RX_TX_Str((uint8_t*)tx_str,(uint8_t*)rx_str);

		
//		LL_mDelay(200);
//		lcd_st7735s_fillrect(0,0,127,159,Disp_Colour);
	//	Disp_Colour++;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(72000000);
  LL_SetSystemCoreClock(72000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
//=======================================================================
void USART1_RX_Callback(void)
{
	dt1 = LL_USART_ReceiveData8(USART1);
  fl=1;
}
void USART_TX (uint8_t* dt, uint16_t sz)
{
	uint16_t ind = 0;
	while (ind<sz)
  {
    while (!LL_USART_IsActiveFlag_TXE(USART1)) {}
		LL_USART_TransmitData8(USART1,*(uint8_t*)(dt+ind));
    ind++;
	}
}

//=======================================================================
void CorrectRX(void)
{
	uint8_t i;
	for (i = 0; i<=6; i++) {
		if (rx_str[i] == rxStrKeyUp[i]) {}
		else {
			return;
		}
	}
	for (i = 0; i<=15; i++) {
		if (rx_str[i + 16] == rxStrKeyDown[i]) {}
		else {
			return;
		}
	}
	
	weight = rx_str[11] * 256 + rx_str[12];
	
	GLOBAL_FLAG_RX = 0;
	GLOBAL_FLAG_RX |= rx_str[7]  * 0x1000000;
	GLOBAL_FLAG_RX |= rx_str[8]  * 0x10000;
	GLOBAL_FLAG_RX |= rx_str[9]  * 0x100;
	GLOBAL_FLAG_RX |= rx_str[10] * 0x1;
	
	return;
}
void CorrectTX(void)
{
	tx_str[15] = GLOBAL_FLAG_TX / 0x1000000;
	tx_str[16] = GLOBAL_FLAG_TX / 0x10000;
	tx_str[17] = GLOBAL_FLAG_TX / 0x100;
	tx_str[18] = GLOBAL_FLAG_TX % 0x100;
}
//=======================================================================
void DMA1_RecieveComplete(void)
{ 
//	LL_mDelay(10);
	DMA1_RX_Stop();
//	DMA1_TX_Stop();
	DMA1_RX_ClearFlag();
	flagRxEnable = 0;
  if (flagTxEnable == 0) {
		CorrectRX();
//		DMA1_TX_InitStart();
//		flagTxEnable = 1;
	}
	return;
}
//=======================================================================
void DMA1_TransmitComplete(void)
{
	CorrectTX();
	DMA1_TX_Stop();
//	DMA1_RX_Stop();
	DMA1_TX_ClearFlag();
	flagTxEnable = 0;
  if (flagRxEnable == 0) {
//		LL_mDelay(100);
		DMA1_RX_InitStart();
//		flagRxEnable = 1;
	}
	return;
}
//=======================================================================
void DMA1_Errors(void)
{
	DMA1_RX_Stop();
	DMA1_TX_Stop();

	return;
}
//=======================================================================
void USART_TransmitComplete(void)
{
	CorrectTX();
	CorrectRX();
//	USART_TX((uint8_t*)tx_str,32);
	USART_RX_TX_Str((uint8_t*)tx_str,(uint8_t*)rx_str);
}
//=======================================================================

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
