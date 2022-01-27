#include "crc_work.h"
#include "main.h"

uint32_t CRC_out = 0;   //Рассчитанное аппаратно CRC
uint8_t CRC8[CRC_SIZE]; //Сконвертированное CRC в массив

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
uint32_t crc32_zlib(char *data, uint32_t cnt)//                              ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
  //53 79 73 74 65 6D 20 00 00 08 80 64 46 28 00 00 20 54 72 61 6E 73 70 6F 72 74 61 74
  uint32_t i;
  LL_CRC_ResetCRCCalculationUnit(CRC);
  for (i = 0; i < cnt; i++)
  {
	  CRC->DR = rx_str[i];
  }
  uint32_t result = (CRC->DR);
  return ~result;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void crc_after_recive(void)//                                                ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
CRC_out = crc32_zlib((char *)rx_str, PACK_SIZE);
//  *(uint32_t*)&CRC8 = CRC_out;
  CRC8[3] = CRC_out;
  CRC8[2] = CRC_out >>  8;
  CRC8[1] = CRC_out >> 16;
  CRC8[0] = CRC_out >> 24;
  if ((CRC8[0] == rx_str[bufferSize - 4]) &&
      (CRC8[1] == rx_str[bufferSize - 3]) &&
      (CRC8[2] == rx_str[bufferSize - 2]) &&
      (CRC8[3] == rx_str[bufferSize - 1]))
  {
    
  }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void crc_before_transmit(void)//                                             ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  CRC_out = crc32_zlib((char *)tx_str, PACK_SIZE);
  tx_str[bufferSize - 4] = CRC_out >> 24;
  tx_str[bufferSize - 3] = CRC_out >> 16;
  tx_str[bufferSize - 2] = CRC_out >>  8;
  tx_str[bufferSize - 1] = CRC_out;
}
