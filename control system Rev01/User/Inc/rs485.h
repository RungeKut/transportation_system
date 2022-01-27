#ifndef RS485_H
#define RS485_H

#include "main.h"

typedef enum
{
  RINGBUFFER_OK        = 0x00U, //Готов
  RINGBUFFER_ERR_BUSY  = 0x01U, //Занят
  RINGBUFFER_ERR_EMPTY = 0x02U, //Буфер пуст
  RINGBUFFER_ERR_FULL  = 0x03U, //Буфер заполнен
  RINGBUFFER_ERR_SYNC  = 0x04U  //Ошибка синхронизации чтения из буфера
} RingBuffer_StatusTypeDef;
#define RingBuffer_size 128 //Размер должен быть кратным степени двойки
#define RingBuffer_headsize 15 //Длина заголовка синхронизации

#define RingBuffer_tail_pp RingBuffer_tail = (RingBuffer_tail + 1) & (RingBuffer_size - 1)

extern volatile RingBuffer_StatusTypeDef RingBuffer_flag;
extern volatile uint16_t RingBuffer_head; //Номер записываемой ячейки приемного кольцевого буфера (Голова)
extern volatile uint16_t RingBuffer_tail; //Номер ячейки приемного кольцевого буфера из которой нужно забрать байт (Хвост)
extern volatile char RingBuffer[RingBuffer_size];
extern volatile uint16_t TxBuffer_Count;

static const char RingBuffer_header[RingBuffer_headsize] = {0x54,0x72,0x61,0x6e,0x73,0x70,0x6f,0x72,0x74,0x61,0x74,0x69,0x6f,0x6e,0x20};

/* BEGIN Prototypes */
void rs485_StartUp(void);
void rs485_IRQHandler(void);
RingBuffer_StatusTypeDef RingBuffer_trywrite(void);
RingBuffer_StatusTypeDef RingBuffer_tryread(void);
void rs485_transmit_on(void);
void rs485_transmit_off(void);
void rs485_transmiting(void);
void rs485_transmit_preparing(void);
uint16_t RingBuffer_getNewDataCount(void);
/* END Prototypes */

#endif /* RS485_H */
