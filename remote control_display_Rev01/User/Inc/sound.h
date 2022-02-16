// http://www.muzoborudovanie.ru/articles/midi/midi5.php
// http://www.deep-shadows.com/hax/wordpress/?page_id=1111
// http://forum.easyelectronics.ru/viewtopic.php?f=35&t=34191
// https://habr.com/ru/post/271693/

#ifndef SOUND_H
#define SOUND_H

#include "main.h"

/**********Begin**********Sound_Status********************/
typedef enum
{
  Sound_OK   = 0x00,
  Sound_BUSY = 0x01,
  Sound_GEN  = 0x02
} Sound_StatusTypeDef;
extern volatile Sound_StatusTypeDef Sound_Status;
/**********End**********Sound_Status********************/

extern volatile uint8_t *Sound_track;
extern uint8_t track_up[6];
extern uint8_t track_down[6];
extern uint8_t track_charge[4];
extern uint8_t track_charging[4];
extern uint8_t track_limit[6];

//uint32_t MidiNum_to_freq(uint8_t);
void Sound_PWM(uint16_t, uint16_t);
void Sound_IRQHandler(void);
void Sound_Play(uint8_t *);
void Sound_Player(uint8_t *);

#endif /* SOUND_H */
