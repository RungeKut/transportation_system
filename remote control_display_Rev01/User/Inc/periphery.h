#ifndef PERIPHERY_H
#define PERIPHERY_H

#include "main.h"

/**********Begin**********Button_Status********************/
typedef enum
{
  BUT_OK         = 0x00, //
  BUT_UP         = 0x01, //Кнопка подъемник вверх
  BUT_DOWN       = 0x02, //Кнопка подъемник вниз
  BUT_FORWARD    = 0x03, //Кнопка подъемник вперед
  BUT_BACKWARD   = 0x04, //Кнопка подъемник назад
  BUT_STRONG     = 0x05, //Кнопка разгрузка увеличить вес
  BUT_WEAK       = 0x06, //Кнопка разгрузка уменьшить вес
  BUT_SPEED_UP   = 0x07, //Кнопка скорость каретки увеличить
  BUT_SPEED_DOWN = 0x08  //Кнопка скорость каретки уменьшить
} Button_StatusTypeDef;
extern volatile Button_StatusTypeDef Button_Status;
/**********End**********Batton_Status********************/

void Vibro_touch(void);
void ScanButton(void);

#endif /* PERIPHERY_H */
