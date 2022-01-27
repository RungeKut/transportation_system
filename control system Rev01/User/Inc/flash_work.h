#ifndef FLASH_WORK_H
#define FLASH_WORK_H

#include "main.h"

#define EnableWrite_FLASH 0x01 //Разрешение записи после стирания страницы
#define SuccessRead_FLASH 0x02 //Переменная из страницы прочитана
extern volatile HAL_StatusTypeDef flash_ok;
extern volatile uint8_t GLOBAL_FLASH_FLAG;
extern volatile uint16_t NbDataOnPage;

/* BEGIN Prototypes */
uint32_t FloatToUint(float);
float UintToFloat(uint32_t);
uint32_t flashPage_addr(uint16_t);
void flashPage_erase(uint16_t, uint8_t);
void flashPage_write32(uint32_t, uint16_t);
uint16_t flashPage_Init(uint16_t);
uint32_t flashPage_read32(uint16_t, uint16_t);
void flash_StartUp(void);
/* END Prototypes */

#endif /* FLASH_WORK_H */
