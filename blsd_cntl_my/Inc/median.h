//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 24.09.2016
//Изменен:                29.09.2016
//**************************************************************/

/**\author Новиков В. А.*/
/**\file median.h*/
/**\brief Реализация медианного фильтра.*/

#ifndef MEDIAN_H
#define MEDIAN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**Размер окна фильтра*/  
#define         MEDIAN_FILTER_SIZE    (7)

/**Тип записи в буфере филтра*/  
typedef struct pair
{
   struct pair   *point;                /* Pointers forming list linked in sorted order */
   uint16_t  value;                     /* Values to sort */
} pair_t;

/**Контекст фильтра*/
typedef struct median_context
{
  pair_t *datpoint;                     /* Pointer into circular buffer of data */
  pair_t small;                         /* Chain stopper */
  pair_t big;                           /* Pointer to head (largest) of linked list.*/
  pair_t buffer[MEDIAN_FILTER_SIZE];    /* Buffer of nwidth pairs */ 
} median_context_t;

/**Инициализировать медианный фильтр
  *@param pctx - указатель на контекст фильтра.
*/
void median_filter_init(median_context_t *pctx);
/**Запустить фильтрацию
  *@param datum - текущее значение, требующее фильтрации
  *@param pctx - контекст фильтра
  *@return отфильтрованное значение*/
uint16_t median_filter(uint16_t datum, median_context_t *pctx);

#ifdef __cplusplus
}
#endif

#endif /*MEDIAN_H*/
