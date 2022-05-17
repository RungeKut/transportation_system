//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 24.09.2016
//Изменен:                29.09.2016
//**************************************************************/

/**\author */
/**\file swtimer.c*/
/**\brief Реализация програмного таймера.*/

#include "swtimer.h"
#include "atomic.h"

/**Програмный таймер*/
typedef struct
{
   TSwTimerMode	        Mode;           //!< Режим запуска таймера                   
   TSwTimerSize 	Counter;        //!< Текущее значение счетчика таймера
   TSwTimerSize 	Compare;        //!< Значение до которого таймер будет считать
   uint8_t		Status;         //!< Флаг, сигнализирующий о событии, когда Counter >= Compare
   TSwTimerFunc 	Handler;        //!< Обработчик, вызываемый при срабатывании таймера
} TSwTimer;

void HAL_SwTimer_Init(void);
void HAL_SwTimer_Sync(void);
void HAL_SwTimer_Lock(void);
void HAL_SwTimer_Unlock(void);

/*Массив програмных таймеров*/
TSwTimer SwTimer_Array[SWTIMER_MAX];

#if (SWTIMER_FASTINT) /*Режим быстрых прерываний*/
	/*Счетчик тиков таймера*/
        static TSwTimerSize tick_counter;
	/*Нарастить счетчик.
         *Функция вызывается из обработчика прерывания аппаратного таймера.*/
	void SwTimer_IntHandler(void)
	{
		tick_counter++;
		return;
	}
	/**Получить число срабатываний аппаратного таймера.
          *@return число срабатываний аппаратного таймера*/
	TSwTimerSize SwTimer_GetTickCounter(void)
	{	
		TSwTimerSize result;
		HAL_SwTimer_Lock();
		if (tick_counter) 
		{
			result = tick_counter;
			tick_counter = 0;
			barrier();
			HAL_SwTimer_Unlock();
			return result;
		}
		else 
		{
			HAL_SwTimer_Unlock();
			return 0;
		}
	}
	/*Обновить значения счетчиков всех активных таймеров в массиве.*/
	void SwTimer_Update(void)
	{	
		uint8_t i;
		TSwTimerSize tick_cnt = SwTimer_GetTickCounter();
		if (tick_cnt > 0)
		{
			for(i=0; i < SWTIMER_MAX; i++)
			{
				if ((SwTimer_Array[i].Mode == SWTIMER_MODE_STOP)||
						(SwTimer_Array[i].Mode == SWTIMER_MODE_EMPTY))
					continue;
				else
				{
					if(SwTimer_Array[i].Counter >= SwTimer_Array[i].Compare)
					{
						SwTimer_Array[i].Counter = 0;
						SwTimer_Array[i].Status = SWTIMER_OVF;
						if(SwTimer_Array[i].Mode == SWTIMER_MODE_SINGLE)
						{
							SwTimer_Array[i].Mode = SWTIMER_MODE_STOP;
						}

						if (SwTimer_Array[i].Handler)
						{
							SwTimer_Array[i].Status = SWTIMER_NO_OVF;
							SwTimer_Array[i].Handler();
						}
					}
					else SwTimer_Array[i].Counter += tick_cnt;
				}
			}
		}
		return;
	}
	
#else   /*Режим, когда обновление всех проограмных таймеров производиться непосредственно в аппаратном прерывании*/
	void SwTimer_IntHandler(void)
	{	
		uint8_t i;
		for(i=0; i < SWTIMER_MAX; i++)
		{
			if ((SwTimer_Array[i].Mode == SWTIMER_MODE_STOP)||
                            (SwTimer_Array[i].Mode == SWTIMER_MODE_EMPTY)) 
                          continue;
			else
			{
				if(SwTimer_Array[i].Counter >= SwTimer_Array[i].Compare)
				{
					SwTimer_Array[i].Counter = 0;
					SwTimer_Array[i].Status = SWTIMER_OVF;
					if(SwTimer_Array[i].Mode == SWTIMER_MODE_SINGLE)
					{
						SwTimer_Array[i].Mode = SWTIMER_MODE_STOP;
					}
				
					if (SwTimer_Array[i].Handler)
					{
						SwTimer_Array[i].Status = SWTIMER_NO_OVF;
						SwTimer_Array[i].Handler();
					}
				}
				else SwTimer_Array[i].Counter++;
			}
		}
		return;
	}
#endif
/**Инициализировать программные таймеры.*/
void SwTimer_Init(void)
{
	uint8_t i;
	for (i=0; i < SWTIMER_MAX; i++)
	{
		SwTimer_Delete(i);
	}
	HAL_SwTimer_Init();
	return;
}

/**Синхронизировать програмные таймеры.*/
void SwTimer_Sync(void)
{
	uint8_t i;
	HAL_SwTimer_Lock();
	for (i=0; i < SWTIMER_MAX; i++)
	{
		SwTimer_Array[i].Counter = 0;
		SwTimer_Array[i].Status = SWTIMER_NO_OVF;
	}
	HAL_SwTimer_Sync();
	HAL_SwTimer_Unlock();
	return;
}

/**Добавить программный таймер.
  *@param mode - режим запуска програмного таймера. 
                Допустимые значения SWTIMER_MODE_STOP, SWTIMER_MODE_CYCLE, SWTIMER_MODE_SINGLE
  *@param compare - значение, до которого таймер будет считать
  *@param handler - обработчик, вызываемый при условии, когда Counter >= compare
  *@return индекс таймера в массиве програмных таймеров или (uint8_t)-1 при неудачне.*/
uint8_t SwTimer_Add(TSwTimerMode mode, TSwTimerSize compare, void (*handler)())
{
	uint8_t i;
	if(mode != SWTIMER_MODE_EMPTY)
	{
		for (i=0; i < SWTIMER_MAX; i++)
		{
			if (SwTimer_Array[i].Mode == SWTIMER_MODE_EMPTY)
			{
				SwTimer_Array[i].Compare = compare;
				SwTimer_Array[i].Counter = 0; 
				SwTimer_Array[i].Handler = handler;
				SwTimer_Array[i].Status = SWTIMER_NO_OVF;
				SwTimer_Array[i].Mode = mode;
				return i;
			}
		}
	}
	return (uint8_t)-1;
}

/**Удалить програмный таймер.
  *@param swtimer_id - индекс таймера в массиве програмных таймеров
  *@return 0 - при успешном удалении, (uint8_t)-1 - при ошибке */
uint8_t SwTimer_Delete(uint8_t swtimer_id)
{
	if (swtimer_id < SWTIMER_MAX)
	{
		SwTimer_Array[swtimer_id].Mode = SWTIMER_MODE_EMPTY;
		SwTimer_Array[swtimer_id].Compare = 0;
		SwTimer_Array[swtimer_id].Counter = 0; 
		SwTimer_Array[swtimer_id].Handler = 0;
		SwTimer_Array[swtimer_id].Status = 0;
		return 0;
	}
	return (uint8_t)-1;
}

/**Получить текущее значение счетчика програмного таймера.
  *@param swtimer_id - индекс таймера в массиве програмных таймеров
  *@return значение счетчика или (TSwTimerSize)-1 при ошибке*/
TSwTimerSize SwTimer_GetCounter(uint8_t swtimer_id)
{	
	TSwTimerSize counter;
	if (swtimer_id < SWTIMER_MAX)
	{
		HAL_SwTimer_Lock();
		counter = SwTimer_Array[swtimer_id].Counter; 
		HAL_SwTimer_Unlock();
		return counter;		
	}
	return (TSwTimerSize)-1;

}

/**Установить значение счетчика програмного таймера.
  *@param swtimer_id - индекс таймера в массиве програмных таймеров
  *@param counter - новое значение счетчика
  *@return 0 при успехе или (uint8_t)-1 при ошибке*/
uint8_t SwTimer_SetCounter(uint8_t swtimer_id, TSwTimerSize counter)
{
	if (swtimer_id<SWTIMER_MAX)
	{
		HAL_SwTimer_Lock();
		SwTimer_Array[swtimer_id].Counter = counter; 
		HAL_SwTimer_Unlock();
		return 0;
	}
	return (uint8_t)-1;
}

/**Получить статус таймера.
  *@param swtimer_id - индекс таймера в массиве програмных таймеров
  *@return 0 - если таймер еще не сработал, 1 - если сработал, (uint8_t)-1 - при ошибке*/
uint8_t SwTimer_GetStatus(uint8_t swtimer_id)
{
	if (swtimer_id<SWTIMER_MAX)
	{	
          HAL_SwTimer_Lock();
          if (SwTimer_Array[swtimer_id].Status) 
          {
            SwTimer_Array[swtimer_id].Status = SWTIMER_NO_OVF;
            HAL_SwTimer_Unlock();
            return 1;
          }
          else
          {
            HAL_SwTimer_Unlock();
            return 0;
          }
	}
	return (uint8_t)-1;
}

/**Получить текущий режим работы таймера.
  *@param swtimer_id - индекс таймера в массиве програмных таймеров
  *@return режим работы таймера или (TSwTimerMode)-1 при ошибке*/
TSwTimerMode SwTimer_GetMode(uint8_t swtimer_id)
{
	if (swtimer_id<SWTIMER_MAX)
	{
		return SwTimer_Array[swtimer_id].Mode;
	}
	return (TSwTimerMode)-1;
}

/**Установить режим работы таймера.
  *@param swtimer_id - индекс таймера в массиве програмных таймеров
  *@param mode - новый режим таймера
  *@return 0 - усли успех, (uint8_t)-1 - в случае ошибки*/
uint8_t SwTimer_SetMode(uint8_t swtimer_id, TSwTimerMode mode)
{
	if (swtimer_id<SWTIMER_MAX)
	{
		SwTimer_Array[swtimer_id].Mode = mode;
		return 0;
	}
	return (uint8_t)-1;
}

/**Получить значение величины срабатывания таймера.
  *@param swtimer_id - индекс таймера в массиве програмных таймеров
  *@return значение Compare, (uint8_t)-1 при ошибке*/
TSwTimerSize SwTimer_GetCompare(uint8_t swtimer_id)
{
	if (swtimer_id<SWTIMER_MAX)
	{
		return SwTimer_Array[swtimer_id].Compare;
	}
	return (uint8_t)-1;
}

/**Установить значение, при ктором таймер сработает.
  *@param swtimer_id - индекс таймера в массиве програмных таймеров
  *@param compare - новое значение величины
  *@return 0 - в случае успеха, (uint8_t)-1 при ошибке*/
uint8_t SwTimer_SetCompare(uint8_t swtimer_id, TSwTimerSize compare)
{
	if (swtimer_id<SWTIMER_MAX)
	{
		SwTimer_Array[swtimer_id].Compare = compare;
		return 0;
	}
	return (uint8_t)-1;
}
