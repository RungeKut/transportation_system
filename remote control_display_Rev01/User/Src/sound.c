#include "sound.h"
#include "main.h"

volatile Sound_StatusTypeDef Sound_Status = Sound_OK;

// Треки 5bit - номер ноты 3bit - длительность
uint8_t track_up[6] = {5, 0x09, 0x01, 0x19, 0x01, 0x29};
uint8_t track_down[6] = {5, 0x29, 0x01, 0x19, 0x01, 0x09};
uint8_t track_charging[4] = {3, 0x0F, 0x07, 0x0F};
uint8_t track_charge[4] = {3, 0x0E, 0x06, 0x0E};
uint8_t track_limit[6] = {5, 0x0E, 0x06, 0x0E, 0x06, 0x0E};
uint8_t track_sw[45] = {44,0x2B,0x1,0x2B,0x1,0x2B,0x1,0xB,0x41,0x2B,0x1,0xB,0x41,0x2C,0x2B,0x1,0x63,0x1,0x63,0x1,0x63,0x1,0x6B,0x41,0x23,0x1,0xB,0x41,0x2C,0x2B,0x1,0x8B,0x1,0x2A,0x1,0x29,0x8B,0x1,0x82,0x1,0x79,0x71,0x69,0x72,0x2};

uint16_t MidiNum_to_freq[31] = {1760,1864,1975,2093,2217,2349,2489,2637,2793,2959,3135,3322,3520,3729,3951,4186,4434,4698,4978,5274,5587,5919,6271,6644,7040,7458,7902,8372,8869,9397,9956};
uint32_t Sound_tick = 0;
uint32_t Sound_time = 0;
uint32_t Sound_count = 0;
uint32_t Sound_Num = 0;
volatile uint8_t *Sound_track;
uint16_t MidiNum;

///*----------------------------------------------------------------*/
//uint32_t MidiNum_to_freq(uint8_t num)
///*----------------------------------------------------------------*/
//{
//  return 2^((num - 69)/12) * 440;
//}

/*----------------------------------------------------------------*/
void Sound_PWM(uint16_t freq, uint16_t time)
/*----------------------------------------------------------------*/
{
  uint32_t reg = 0;
  if (freq > 5) 
  {
    reg = 72000000 / (TIM_Sound_PSC + 1) / freq;
    LL_TIM_CC_EnableChannel(TIM_Sound, TIM_Sound_CH);
    LL_TIM_OC_SetCompareCH1(TIM_Sound, reg / 2);
    LL_TIM_SetAutoReload(TIM_Sound, reg);
    Sound_time = time * reg * freq / (TIM_Sound_PSC + 1) / Sound_bpm;
  }
  else
  {
    reg = 72000000 / (TIM_Sound_PSC + 1) / 1000;
    LL_TIM_SetAutoReload(TIM_Sound, reg);
    Sound_time = time * reg * 1000 / (TIM_Sound_PSC + 1) / Sound_bpm;
  }
  LL_TIM_EnableIT_UPDATE(TIM_Sound);
  LL_TIM_EnableCounter(TIM_Sound);
  Sound_Status = Sound_GEN;
}
/*----------------------------------------------------------------*/
void Sound_IRQHandler(void)
/*----------------------------------------------------------------*/
{
  if (LL_TIM_IsActiveFlag_UPDATE(TIM_Sound))
	{
		LL_TIM_ClearFlag_UPDATE(TIM_Sound);
    if (Sound_time == 0) return;
    Sound_tick++;
    if (Sound_tick > Sound_time)
    {
      Sound_tick = 0;
      LL_TIM_DisableCounter(TIM_Sound);
      LL_TIM_DisableIT_UPDATE(TIM_Sound);
      LL_TIM_CC_DisableChannel(TIM_Sound, TIM_Sound_CH);
      Sound_Status = Sound_BUSY;
    }
  }
}

/*----------------------------------------------------------------*/
void Sound_Play(uint8_t *track)// 5bit - номер ноты 3bit - длительность
/*----------------------------------------------------------------*/
{
  if (Sound_Status != Sound_OK) return;
  Sound_Status = Sound_BUSY;
  Sound_Num = track[0];
  Sound_count = track[0];
  Sound_track = track;
}

/*----------------------------------------------------------------*/
void Sound_Player(uint8_t *track)
/*----------------------------------------------------------------*/
{
  if (Sound_Status == Sound_OK) return;
  if (Sound_Status == Sound_GEN) return;
  if (Sound_count < 1)
  {
    Sound_Status = Sound_OK;
    return;
  }
  // Номер ноты
  MidiNum = track[Sound_Num - (Sound_count - 1)] >> 3;
  uint32_t freq;
  if (MidiNum == 0)
  {
    freq = 1;
  }
  else
  {
    freq = MidiNum_to_freq[MidiNum];
  }
  // Номер паузы
  uint16_t time = track[Sound_Num - (Sound_count - 1)] & 0x07;
  if (time >= 7) time = 30;
  Sound_PWM(freq, time);
  Sound_count--;
}
