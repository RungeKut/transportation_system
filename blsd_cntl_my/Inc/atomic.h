//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 24.09.2016
//Изменен:                29.09.2016
//**************************************************************/

/**\author Новиков В. А.*/
/**\file atomic.h*/
/**\brief */

#ifndef ATOMIC_H
#define ATOMIC_H
#include "stm32f4xx_hal.h"



#define barrier() do {__asm volatile("":::"memory");}while(0) // ?? нет логики в цикле do_while ??
// #define barrier() __asm volatile("":::"memory")

#define ATOMIC_BLOCK_FORCEON \
            for (int _flag = _irqDis(); \
                 _flag != 0; \
                 _flag = _irqEn())

#define ATOMIC_BLOCK_RESTORE \
              for(unsigned long _mask = _iDisGetPrimask(), _flag = 1; \
                  _flag != 0; \
                  _flag = _iSetPrimask(_mask))


#define ATOMIC_BLOCK_IRQ(IRQ) \
                for(int _flag = _nvic_irqDis(IRQ);      \
                        _flag != 0;                     \
                        _flag = _nvic_irqEn(IRQ))
                
__STATIC_INLINE int _irqDis(void)
{
    __disable_irq();
    return 1;
}

__STATIC_INLINE int _irqEn(void)
{
    __enable_irq();
    return 0;
}

__STATIC_INLINE unsigned long _iDisGetPrimask(void)
{
    unsigned long result = __get_PRIMASK(); 
    __disable_irq();
    return result;
}

__STATIC_INLINE unsigned long _iSetPrimask(unsigned long priMask)
{
    __set_PRIMASK(priMask);
    return 0;
}

__STATIC_INLINE int _nvic_irqEn(IRQn_Type IRQn)
{
  NVIC_EnableIRQ(IRQn);
  return 1;
}

__STATIC_INLINE int _nvic_irqDis(IRQn_Type IRQn)
{
  NVIC_DisableIRQ(IRQn);
  return 0;
}

#endif /*ATOMIC_H*/
