//**************************************************************
//Организация:            кафедра ИТФИ ННГУ, г. Нижний Новгород
//Проект:                 pulse
//Версия проекта: 1.0
//Спецификация:   ANSI C++ 98
//Создан:                 24.09.2016
//Изменен:                29.09.2016
//**************************************************************/

/**\author Новиков В. А.*/
/**\file median.c*/
/**\brief Реализация медианного фильтра.*/

#include "median.h"
#include <stdint.h>
#include <string.h>


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define  STOPPER 0               /* Smaller than any datum */

/*****************************************************************************
 * Public functions
 ****************************************************************************/
 /*Инициализировать контекст медианного фильтра*/
void median_filter_init(median_context_t *pctx)
{
  //buffer[MEDIAN_FILTER_SIZE] = {0}; /* Buffer of nwidth pairs */   
  memset(pctx->buffer, 0, sizeof(pctx->buffer));
  //static pair_t *datpoint = buffer;               /* Pointer into circular buffer of data */
  pctx->datpoint = pctx->buffer;
  //static pair_t small = {NULL, STOPPER};          /* Chain stopper */
  pctx->small.point = NULL;
  pctx->small.value = STOPPER;
  //static pair_t big = {&small, 0};                /* Pointer to head (largest) of linked list.*/
  pctx->big.point = &(pctx->small);
  pctx->big.value = 0;
}
 
/*Филтровать текущение значение datum*/
uint16_t median_filter(uint16_t datum, median_context_t *pctx)
{
 pair_t *successor;                              /* Pointer to successor of replaced data item */
 pair_t *scan;                                   /* Pointer used to scan down the sorted list */
 pair_t *scanold;                                /* Previous value of scan */
 pair_t *median;                                 /* Pointer to median */
 uint16_t i;
 //median_context_t *pctx = &ctx[adc_ch];

 if (datum == STOPPER)
 {
   datum = STOPPER + 1;                         /* No stoppers allowed. */
 }

 if ( (++(pctx->datpoint) - pctx->buffer) >= MEDIAN_FILTER_SIZE)
 {
   pctx->datpoint = pctx->buffer;               /* Increment and wrap data in pointer.*/
 }

 pctx->datpoint->value = datum;                 /* Copy in new datum */
 successor = pctx->datpoint->point;             /* Save pointer to old value's successor */
 median = &(pctx->big);                         /* Median initially to first in chain */
 scanold = NULL;                                /* Scanold initially null. */
 scan = &(pctx->big);                           /* Points to pointer to first (largest) datum in chain */

 /* Handle chain-out of first item in chain as special case */
 if (scan->point == pctx->datpoint)
 {
   scan->point = successor;
 }
 scanold = scan;                                /* Save this pointer and   */
 scan = scan->point ;                           /* step down chain */

 /* Loop through the chain, normal loop exit via break. */
 for (i = 0; i < MEDIAN_FILTER_SIZE; ++i)
 {
   /* Handle odd-numbered item in chain  */
   if (scan->point == pctx->datpoint)
   {
     scan->point = successor;                      /* Chain out the old datum.*/
   }

   if (scan->value < datum)                        /* If datum is larger than scanned value,*/
   {
     pctx->datpoint->point = scanold->point;             /* Chain it in here.  */
     scanold->point = pctx->datpoint;                    /* Mark it chained in. */
     datum = STOPPER;
   };

   /* Step median pointer down chain after doing odd-numbered element */
   median = median->point;                       /* Step median pointer.  */
   if (scan == &(pctx->small))
   {
     break;                                      /* Break at end of chain  */
   }
   scanold = scan;                               /* Save this pointer and   */
   scan = scan->point;                           /* step down chain */

   /* Handle even-numbered item in chain.  */
   if (scan->point == pctx->datpoint)
   {
     scan->point = successor;
   }

   if (scan->value < datum)
   {
     pctx->datpoint->point = scanold->point;
     scanold->point = pctx->datpoint;
     datum = STOPPER;
   }

   if (scan == &(pctx->small))
   {
     break;
   }

   scanold = scan;
   scan = scan->point;
 }
 return median->value;
}
