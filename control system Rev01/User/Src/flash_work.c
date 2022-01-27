#include "flash_work.h"
#include "main.h"

//Последняя страница flash контроллера по адресу 0x08000000 + 128 * 1023 = 0x0801FF80
#define START_ADDR_USER_PAGE ((uint32_t)0x0801F800)
//Одна страница 128 bytes. Если переменные 32bit то на странице уберется:
//128 * 8 / 32 = 32 шт.
//Одна 32bit переменная = 4 ячейки памяти по 8bit, следовательно адрес нужно сдвигать на 0x4
volatile uint16_t NbDataOnPage = 1; //Номер переменной на странице
volatile uint8_t GLOBAL_FLASH_FLAG = 0;

FLASH_EraseInitTypeDef EraseInitStruct;
volatile HAL_StatusTypeDef flash_ok;
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
uint32_t FloatToUint(float n)           //                                   ┃
{                                       //                                   ┃
   return (uint32_t)(*(uint32_t*)&n);   //                                   ┃
}                                       //                                   ┃
float UintToFloat(uint32_t n)           //                                   ┃
{                                       //                                   ┃
   return (float)(*(float*)&n);         //                                   ┃
}                                       //                                   ┃
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Найдем адрес страницы                                                      ┃
uint32_t flashPage_addr(uint16_t Page)// Номер страницы                      ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  return 0x08000000 + 128 * (Page - 1);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Стираем страницу   Номер и количество страниц                              ┃
void flashPage_erase(uint16_t Page, uint8_t NPage)//                         ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;  //указываем что будем стирать страницу
  EraseInitStruct.PageAddress = flashPage_addr(Page);   //указываем начальный адрес стираемой страницы
  EraseInitStruct.NbPages     = NPage;                  //количество стираемых страниц - одна

  uint32_t PageError = 0x00;
  HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
  NbDataOnPage = 1;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Пишем 32bit переменную на страницу. Их всего 512                           ┃
void flashPage_write32(uint32_t dat, uint16_t Page)//32 bit word             ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  NbDataOnPage++;
  if (NbDataOnPage > 32)
  {
    flashPage_erase(Page, 1);
  }
  flash_ok = HAL_ERROR;
  while(flash_ok != HAL_OK)
  {
    flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashPage_addr(Page)+((NbDataOnPage-1)*0x4), (uint32_t)dat);
  }
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Читаем один байт из flash памяти                                           ┃
 uint8_t read_byte(uint32_t addr)//                                          ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  return *(volatile uint8_t *)addr;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Ищем последнюю, записанную переменную на странице и определяем ее номер    ┃
uint16_t flashPage_Init(uint16_t Page)//                                     ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
// Адрес последней страницы 0x08000000+128*1023=0x0801FF80
// Адрес последней ячейки этой страницы 0x0801FF80 + 127
  uint32_t UPaddr = flashPage_addr(Page);
  uint32_t DWaddr = flashPage_addr(Page) + 127;
  uint8_t byte;
  uint16_t num = 0;
  for (uint32_t i = DWaddr; i >= UPaddr; i--)
  {
    byte = read_byte(i);
    if (byte != 0)
    {
      return 32 - (num / 4);
    }
    num++;
  }
  flashPage_erase(Page, 1);
  return 1;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//Читаем переменную со страницы                                              ┃
uint32_t flashPage_read32(uint16_t Page, uint16_t NbData)//                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  uint32_t addr = flashPage_addr(Page) + 4 * (NbData - 1);
  uint32_t data = 0;
  uint16_t byte;
  for (uint8_t i = 0; i < 4; i++)
  {
    byte = read_byte(addr + i);
    data |= byte << (8 * i);
  }
  return data;
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
//При включении                                                              ┃
void flash_StartUp(void)//                                                   ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/
{
  flash_ok = HAL_FLASH_Unlock();
  NbDataOnPage = flashPage_Init(1024);
  float temp;
  temp = UintToFloat(flashPage_read32(1024, NbDataOnPage));
  if (temp != 0.0)
  {
    RemainBatCapacity_mWh = temp;
  }
  GLOBAL_FLASH_FLAG |= EnableWrite_FLASH;
}
