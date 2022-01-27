#include "rs485.h"
#include "main.h"

volatile RingBuffer_StatusTypeDef RingBuffer_flag = RINGBUFFER_OK;
volatile uint16_t RingBuffer_head = 0; //Номер записываемой ячейки приемного кольцевого буфера (Голова)
volatile uint16_t RingBuffer_tail = 0; //Номер ячейки приемного кольцевого буфера из которой нужно забрать байт (Хвост)
volatile uint16_t RingBuffer_temp = 0;
volatile char RingBuffer[RingBuffer_size];
volatile uint16_t TxBuffer_Count = 0;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Старт микроконтроллера
void rs485_StartUp(void)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
{
  LL_USART_EnableIT_RXNE(USART1);
//  LL_USART_EnableIT_TC(USART1);
//  LL_USART_EnableIT_IDLE(USART1);
  LL_USART_EnableDirectionTx(USART1);
  LL_USART_EnableDirectionRx(USART1);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Старт микроконтроллера
void rs485_IRQHandler(void)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
{
	if (LL_USART_IsActiveFlag_RXNE(USART1))
  {
    if (RingBuffer_flag != RINGBUFFER_ERR_FULL)
    {
      RingBuffer_flag = RingBuffer_trywrite();
    }
  }
  else if (LL_USART_IsActiveFlag_TC(USART1))
  {
    if (RingBuffer_flag != RINGBUFFER_ERR_EMPTY)
    {
      rs485_transmiting();
    }
  }
}
//=========================================================
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Запись в кольцевой буфер
RingBuffer_StatusTypeDef RingBuffer_trywrite(void)
{
  if (((RingBuffer_tail + 1) & (RingBuffer_size - 1)) == RingBuffer_head)
  {
    LL_USART_ReceiveData8(USART1);
    if (RingBuffer_flag == RINGBUFFER_OK)
      return RINGBUFFER_ERR_FULL;
    else
      return RingBuffer_flag;
  }
  RingBuffer[RingBuffer_tail] = LL_USART_ReceiveData8(USART1);
  //Увеличиваем хвост на 1
  RingBuffer_tail = (RingBuffer_tail + 1) & (RingBuffer_size - 1);
  if ((RingBuffer_tail == 0) && (bdcSpeed_flag > 0))
    bdcSpeed_flag--;
  return RINGBUFFER_OK;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Чтение из кольцевого буфера
RingBuffer_StatusTypeDef RingBuffer_tryread(void)
{
/*  if (PERIPHERY_FLAG == 0)
//    return RingBuffer_flag;
  else*/ if (RingBuffer_head == RingBuffer_tail)
  {
    if (RingBuffer_flag == RINGBUFFER_OK)
      return RINGBUFFER_ERR_EMPTY;
    else
      return RingBuffer_flag;
  }
  else if (RingBuffer_getNewDataCount() >= SendSize)
  {
    if (RingBuffer_flag != RINGBUFFER_ERR_SYNC)
      RingBuffer_temp = RingBuffer_head;
    for (uint8_t i = 0; i < RingBuffer_headsize; i++)
    {
      if ((RingBuffer[i + RingBuffer_temp]) != RingBuffer_header[i])
      { //Увеличиваем голову на 1
        RingBuffer_temp = (RingBuffer_temp + 1) & (RingBuffer_size - 1);
        if (RingBuffer_temp == RingBuffer_tail)
        {
          RingBuffer_head = RingBuffer_temp;
          return RINGBUFFER_ERR_EMPTY;
        }
        return RINGBUFFER_ERR_SYNC;
      }
    }
    //Увеличиваем голову на размер заголовка
    RingBuffer_temp = (RingBuffer_temp + RingBuffer_headsize) & (RingBuffer_size - 1);
    GLOBAL_FLAG_RX = (RingBuffer[RingBuffer_temp]                                << 24) +
                     (RingBuffer[(RingBuffer_temp + 1) & (RingBuffer_size - 1)]  << 16) +
                     (RingBuffer[(RingBuffer_temp + 2) & (RingBuffer_size - 1)]  <<  8) +
                     (RingBuffer[(RingBuffer_temp + 3) & (RingBuffer_size - 1)]  <<  0);
//            battery = RingBuffer[(RingBuffer_temp + 4) & (RingBuffer_size - 1)];
//	           weight = RingBuffer[(RingBuffer_tempd + 5) & (RingBuffer_size - 1)];
//	            speed = RingBuffer[(RingBuffer_temp + 6) & (RingBuffer_size - 1)];
//     Battery_Status = RingBuffer[(RingBuffer_temp + 7) & (RingBuffer_size - 1)];
    
    RingBuffer_head = (RingBuffer_head + SendSize) & (RingBuffer_size - 1);
    rs485_transmit_on();
  }
  return RINGBUFFER_OK;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void rs485_transmit_on(void)
{
  rs485_transmit_preparing();
  LL_USART_DisableIT_RXNE(USART1);
  LL_USART_EnableIT_TC(USART1);
//  LL_USART_EnableDirectionTx(USART1);
}
void rs485_transmit_off(void)
{
  TxBuffer_Count = 0;
  LL_USART_DisableIT_TC(USART1);
//  LL_USART_DisableDirectionTx(USART1);
  LL_USART_EnableIT_RXNE(USART1);
}
void rs485_transmiting(void)
{
  if (TxBuffer_Count < SendSize)
  {
    LL_USART_TransmitData8(USART1, tx_str[TxBuffer_Count]);
    TxBuffer_Count++;
  }
  else
  {
    rs485_transmit_off();
  }
}
void rs485_transmit_preparing(void)
{
  weight = MIN_WEIGHT + step * (MAX_WEIGHT - MIN_WEIGHT) / MAX_STEP;
  tx_str[7]  =  GLOBAL_FLAG_TX >> 24;
	tx_str[8]  = (GLOBAL_FLAG_TX << 8) >> 24;
	tx_str[9]  = (GLOBAL_FLAG_TX << 16) >> 24;
	tx_str[10] =  GLOBAL_FLAG_TX;
  tx_str[11] = battery;
  tx_str[12] = weight;
  tx_str[13] = speed;
  tx_str[14] = Battery_Status;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
uint16_t RingBuffer_getNewDataCount(void)
{
  if (RingBuffer_tail < RingBuffer_head)
    return RingBuffer_size + RingBuffer_tail - RingBuffer_head;
  else
    return RingBuffer_tail - RingBuffer_head;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
