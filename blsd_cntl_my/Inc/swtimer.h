//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 24.09.2016
//Изменен:                29.09.2016
//**************************************************************/

/**\author */
/**\file swtimer.h*/
/**\brief Реализация програмного таймера.*/

#ifndef SWTIMER_H
#define SWTIMER_H

#include <stdint.h>
#include "swtimer_table.h"

//**********	НАСТРОЙКИ	**********
	#define SWTIMER_SIZE 		32			//разрядность программных таймеров (8, 16 или 32 бит)
	#define SWTIMER_MAX		(SWTIMER_NUM)		//максимальное количество программных таймеров
	#define SWTIMER_FASTINT		1			//0 - обслуживание массива таймеров идет прямо в перывании, 1 - выставляется лишь флаг на обслуживание(само обслуживание вызывается SwTimer_Update())
	#define SYSTIMER_T		10    		//[мкс] - период переполнения аппаратного таймера
	#define SYSCLK			84			//[МГц] - системная таковая частота мк после делителей и прочего
	/*
		На самом деле точное значение периода переполнения часто установить нельзя. Особенно, если нет
		в таймере компаратора или регистра перезагрузки. Тогда приходится работать с его переполнением,
		а значит при этом COMP=255 всегда.
		Поэтому стоит иметь ввиду, что реальное значение периода в общем случае можно определеить по:
			
			T_REAL = (TIMER_DIV*COMP)/SYSCLK
	
	*/
//***********************************

	#define __SWTIMER_SIZE(N)		uint##N##_t
	#define _SWTIMER_SIZE(N)		__SWTIMER_SIZE(N)
	#define TSwTimerSize			_SWTIMER_SIZE(SWTIMER_SIZE)
			
	#define SWTIMER_NO_OVF			0
	#define SWTIMER_OVF			1
	
	#define SWTIMER_NO_HANDLER		0			
	/**Режимы работа таймера.*/
	typedef enum
	{
		SWTIMER_MODE_EMPTY,     //!< Таймер не добавлен и не инициализирован
		SWTIMER_MODE_STOP,      //!< Таймер остановлен
		SWTIMER_MODE_CYCLE,     //!< Таймер в цислическом режиме
		SWTIMER_MODE_SINGLE     //!< Таймер сработает однократно
	} TSwTimerMode;
	
        /**Функция, вызываемая при срабатывании таймера.*/
	typedef void (*TSwTimerFunc)(void);

	/**Инициализировать программные таймеры.*/
	void	 		SwTimer_Init(void);
        /**Синхронизировать програмные таймеры.*/
	void			SwTimer_Sync(void);
        /**Добавить программный таймер.
          *@param mode - режим запуска програмного таймера. 
                Допустимые значения SWTIMER_MODE_STOP, SWTIMER_MODE_CYCLE, SWTIMER_MODE_SINGLE
          *@param compare - значение, до которого таймер будет считать
          *@param handler - обработчик, вызываемый при условии, когда Counter >= compare
          *@return индекс таймера в массиве програмных таймеров или (uint8_t)-1 при неудачне.*/
	uint8_t 		SwTimer_Add(TSwTimerMode mode, TSwTimerSize compare, TSwTimerFunc handler);
        /**Удалить програмный таймер.
          *@param swtimer_id - индекс таймера в массиве програмных таймеров
          *@return 0 - при успешном удалении, (uint8_t)-1 - при ошибке */
	uint8_t 		SwTimer_Delete(uint8_t swtimer_id);
        
        /**Получить текущее значение счетчика програмного таймера.
          *@param swtimer_id - индекс таймера в массиве програмных таймеров
          *@return значение счетчика или (TSwTimerSize)-1 при ошибке*/
	TSwTimerSize 	SwTimer_GetCounter(uint8_t swtimer_id);
        /**Установить значение счетчика програмного таймера.
          *@param swtimer_id - индекс таймера в массиве програмных таймеров
          *@param counter - новое значение счетчика
          *@return 0 при успехе или (uint8_t)-1 при ошибке*/
	uint8_t	 		SwTimer_SetCounter(uint8_t swtimer_id, TSwTimerSize counter);
        /**Получить статус таймера.
          *@param swtimer_id - индекс таймера в массиве програмных таймеров
          *@return 0 - если таймер еще не сработал, 1 - если сработал, (uint8_t)-1 - при ошибке*/
	uint8_t 		SwTimer_GetStatus(uint8_t swtimer_id);
        
        /**Получить текущий режим работы таймера.
          *@param swtimer_id - индекс таймера в массиве програмных таймеров
          *@return режим работы таймера или (TSwTimerMode)-1 при ошибке*/        
	TSwTimerMode	SwTimer_GetMode(uint8_t swtimer_id);
        /**Установить режим работы таймера.
          *@param swtimer_id - индекс таймера в массиве програмных таймеров
          *@param mode - новый режим таймера
          *@return 0 - усли успех, (uint8_t)-1 - в случае ошибки*/
	uint8_t	 		SwTimer_SetMode(uint8_t swtimer_id, TSwTimerMode mode);
        
        /**Получить значение величины срабатывания таймера.
          *@param swtimer_id - индекс таймера в массиве програмных таймеров
          *@return значение Compare, (uint8_t)-1 при ошибке*/
	TSwTimerSize 	SwTimer_GetCompare(uint8_t swtimer_id);
        /**Установить значение, при ктором таймер сработает.
          *@param swtimer_id - индекс таймера в массиве програмных таймеров
          *@param compare - новое значение величины
          *@return 0 - в случае успеха, (uint8_t)-1 при ошибке*/
	uint8_t	 		SwTimer_SetCompare(uint8_t swtimer_id, TSwTimerSize compare);
	
	#if (SWTIMER_FASTINT) /*Режим быстрых прерываний*/
                /**Получить число срабатываний аппаратного таймера.
                  *@return число срабатываний аппаратного таймера*/
		TSwTimerSize            SwTimer_GetTickCounter(void);
                /**Обновить значения счетчиков всех активных программных таймеров в массиве.*/
		void			SwTimer_Update(void);
	#endif

#endif
