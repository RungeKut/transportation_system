#include "st7735.h"
#include "main.h"

//Разрешение 128x160p 
//Разрешаем работу DWT(Data Watchpoint) счетчика для формирования
//наносекундных задержек, и рассчитываем количество тиков
#define    DWT_CYCCNT    *(volatile uint32_t *)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t *)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t *)0xE000EDFC
uint32_t DWT_Tick = 0;
volatile uint8_t digits[3] = {0,0,0};
uint8_t *disp_w[10] = {&width_dig0, &width_dig1, &width_dig2, &width_dig3, &width_dig4, &width_dig5, &width_dig6, &width_dig7, &width_dig8, &width_dig9};
uint8_t *disp_h[10] = {&height_dig0, &height_dig1, &height_dig2, &height_dig3, &height_dig4, &height_dig5, &height_dig6, &height_dig7, &height_dig8, &height_dig9};
uint8_t *disp_p[10] = {(uint8_t *)dig0, (uint8_t *)dig1, (uint8_t *)dig2, (uint8_t *)dig3, (uint8_t *)dig4, (uint8_t *)dig5, (uint8_t *)dig6, (uint8_t *)dig7, (uint8_t *)dig8, (uint8_t *)dig9};
volatile Display_StatusTypeDef Display_Status = CLEAR;
volatile uint8_t Charger_Status = 0;
volatile uint8_t DisplayOutLowBattery_Status = 0;
uint8_t temp_bat = 250;
uint8_t temp_spd = 250;
uint8_t temp_wgt = 250;
  
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void st7735_StartUp(void)//                                                  ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/  
{
	LL_SPI_Enable(SPI1);
	LL_SPI_TransmitData16(SPI1, 0x01FF);
	lcd_st7735s_init();
	delaySetNs(1000);
	LL_GPIO_SetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
	lcd_st7735s_fillrect(0,0,127,159,0xFFFF);
	lcd_st7735s_img8(100,117, width_bat_img, height_bat_img, bat_img);
	lcd_st7735s_img8(70,57, width_speed_img, height_speed_img, speed_img);
	lcd_st7735s_img8(70,7, width_weight_img, height_weight_img, weight_img); 
//  lcd_st7735s_img(104,120, width_charge_img, height_charge_img, charge_img);
}
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓*/
void lcd_reinit(void)//                                                      ┃
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/  
{
  lcd_st7735s_init();
	delaySetNs(1000);
	LL_GPIO_SetOutputPin(Seg_c_Disp_BackLight_GPIO_Port, Seg_c_Disp_BackLight_Pin);
	lcd_st7735s_fillrect(0,0,127,159,0xFFFF);
	lcd_st7735s_img8(100,117, width_bat_img, height_bat_img, bat_img);
	lcd_st7735s_img8(70,57, width_speed_img, height_speed_img, speed_img);
	lcd_st7735s_img8(70,7, width_weight_img, height_weight_img, weight_img); 
}
void delaySetNs(uint32_t ns)
{
	SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;
//	DWT_Tick = (uint32_t) ns * 4 / 125; //32MHz
	DWT_Tick = (uint32_t) ns * 25 / 347; //72MHz
}

//Сама функция задержки
inline void delayNs(void)
{
	DWT->CYCCNT = 0; 
	while(DWT->CYCCNT < DWT_Tick) {}
}

//Установка низкого уровня ChipSelect
void lcd_st7735s_CS_0(void)
{
	LL_GPIO_ResetOutputPin(Seg_2_Disp_CS_GPIO_Port, Seg_2_Disp_CS_Pin);
  delayNs();
}

//Установка высокого уровня ChipSelect
void lcd_st7735s_CS_1(void)
{
	LL_GPIO_SetOutputPin(Seg_2_Disp_CS_GPIO_Port, Seg_2_Disp_CS_Pin);
}

//Установка низкого уровня Reset
void lcd_st7735s_RES_0(void)
{
	LL_GPIO_ResetOutputPin(Seg_1_Disp_reset_GPIO_Port, Seg_1_Disp_reset_Pin);
}

//Установка высокого уровня Reset
void lcd_st7735s_RES_1(void)
{
	LL_GPIO_SetOutputPin(Seg_1_Disp_reset_GPIO_Port, Seg_1_Disp_reset_Pin);
}

// Отправка данных/команд на дисплей
void lcd_st7735s_send(uint16_t data) 
  {
		while(((SPI1->SR) & 0x00000002) == 0)
		{
		}
    LL_SPI_TransmitData16(SPI1, data);
  }

// Отправка команды на дисплей с ожиданием конца передачи
void lcd_st7735s_send_cmd(uint8_t cmd) 
  {
		__disable_irq();
		uint16_t cmd_send = 0x0000;
		cmd_send |= cmd;
		lcd_st7735s_send(cmd_send);
		__enable_irq();
  }

// Отправка данных на дисплей с ожиданием конца передачи
void lcd_st7735s_send_data(uint8_t data) 
  {
		__disable_irq();
		uint16_t data_send = 0x0100;
		data_send |= data;
    lcd_st7735s_send(data_send);
		__enable_irq();
  }

// Определение области экрана для заполнения
void lcd_st7735s_at(uint8_t startX, uint8_t startY, uint8_t stopX, uint8_t stopY) 
{
	lcd_st7735s_send_cmd(0x2A);  
	lcd_st7735s_send_data(0x00);
	lcd_st7735s_send_data(startX);
	lcd_st7735s_send_data(0x00);        
	lcd_st7735s_send_data(stopX);
        
	lcd_st7735s_send_cmd(0x2B);
	lcd_st7735s_send_data(0x00);
	lcd_st7735s_send_data(startY);
	lcd_st7735s_send_data(0x00);       
	lcd_st7735s_send_data(stopY);
}

// Инициализация
void lcd_st7735s_init(void) 
{
	// аппаратный сброс дисплея
//	lcd_st7735s_CS_1();		// CS=1
//	delayNs();						// пауза
//	lcd_st7735s_RES_0();	// RST=0
	delayNs();						// пауза
  lcd_st7735s_RES_1();	// RST=1
	LL_mDelay(1);						// пауза
	lcd_st7735s_CS_0();		// CS=0
	delayNs();
	
	// инициализация дисплея
	lcd_st7735s_send_cmd(ST7735S_SLPOUT);	// после сброса дисплей спит - даем команду проснуться
	LL_mDelay(1);														// пауза
	lcd_st7735s_send_cmd(ST7735S_COLMOD);	// режим цвета:
	lcd_st7735s_send_data(0x05);	//16 бит
	lcd_st7735s_send_cmd(ST7735S_MADCTL);	// направление вывода изображения:
	lcd_st7735s_send_data(0x1C);	// снизу вверх, справа на лево, порядок цветов RGB
//	lcd_st7735s_send_cmd(ST7735S_TEON);
	lcd_st7735s_send_cmd(ST7735S_DISPON);	// включаем изображение
//	lcd_st7735s_CS_1();		// CS=1
}

// Функция заполнения прямоугольной области экрана заданным цветом
void lcd_st7735s_fillrect(uint8_t startX, uint8_t startY, uint8_t stopX, uint8_t stopY, uint16_t color) 
{
    uint8_t y;
    uint8_t x;
//	lcd_st7735s_send_cmd(ST7735S_DISPOFF);
//	lcd_st7735s_CS_0();

    lcd_st7735s_at(startX, startY, stopX, stopY);

    lcd_st7735s_send_cmd(ST7735S_RAMWR);

    for (y=startY;y<stopY+1;y++)
	for (x=startX;x<stopX+1;x++) 
          {
            lcd_st7735s_send_data((uint8_t)((color & 0xFF00)>>8));
            lcd_st7735s_send_data((uint8_t) (color & 0x00FF));
          }
	/*		while(((SPI1->SR) & 0x00000002) == 0)
		{
		}*/
//					lcd_st7735s_send_cmd(ST7735S_DISPON);
//	lcd_st7735s_CS_1();
}

// вывод пиксела
void lcd_st7735s_putpix(uint8_t x, uint8_t y, uint16_t color) 
{
//	lcd_st7735s_CS_0();
    
//    lcd_st7735s_at(x, y, x, y);
//    lcd_st7735s_send_cmd(0x2C);
    lcd_st7735s_send_data((uint8_t)((color & 0xFF00)>>8));
    lcd_st7735s_send_data((uint8_t) (color & 0x00FF));
    
//	lcd_st7735s_CS_1();
}
        
//-----------------------------------------------------------------------------/
// Алгоритм Брезенхема
//-----------------------------------------------------------------------------]

uint8_t abs(int x)
{
        if (x < 0)
               return (uint8_t)(-x);
        else
               return (uint8_t)(x);
}

// Функция отрисовки линии
void lcd_st7735s_line(int x0, int y0, int x1, int y1, uint16_t color)
{
    uint8_t steep = 0;
    
    if((abs(y1 - y0)) > (abs(x1 - x0)))
    {
      steep = 1; // Проверяем рост отрезка по оси икс и по оси игрек
    } 
    else
    {
      steep = 0;
    }
    uint8_t temp= 0;
    // Отражаем линию по диагонали, если угол наклона слишком большой
    if (steep)
    {
      temp = x0;
      x0 = y0;
      y0 = temp;
      
      temp = x1;
      x1 = y1;
      y1 = temp;
    }
    // Если линия растёт не слева направо, то меняем начало и конец отрезка местами
    if (x0 > x1)
    {
      temp = x0;
      x0 = x1;
      y0 = temp;
      
      temp = y0;
      x1 = y1;
      y1 = temp;
    }
    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int error = dx / 2; // Здесь используется оптимизация с умножением на dx, чтобы избавиться от лишних дробей
    int ystep = (y0 < y1) ? 1 : -1; // Выбираем направление роста координаты y
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        lcd_st7735s_putpix(steep ? y : x, steep ? x : y, color); // Не забываем вернуть координаты на место
        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}
  
//-----------------------------------------------------------------------------]
  
 // Рисование прямоугольника (не заполненного)
void lcd_st7735s_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color) 
{
	lcd_st7735s_fillrect(x1,y1, x2,y1, color);
	lcd_st7735s_fillrect(x1,y2, x2,y2, color);
	lcd_st7735s_fillrect(x1,y1, x1,y2, color);
	lcd_st7735s_fillrect(x2,y1, x2,y2, color);
}



// Форматирование строки
// uint8_t *str массив для строки
// int dig число для вывода
// uint8_t rd количество разрядов в числе. 
// (массив должен иметь на 2 элемента больше чем количество разрядов в числе)

void lcd_format_int(uint8_t *str, int dig, uint8_t rd)
{
  if(rd <= 0) return ;  // Если число разрядов меньше или равно нулю. (ошибка)

  volatile uint16_t temp = 0;
  
  if(dig < 0)           // Если число отрицательное
    {
      *str = 0x2D;      // Записать в 0 ячейку массива знак минус
      str++;            // Передвинуться на следущую ячейку массива
      dig = (dig * (-1));        // Взять число по модулю
    }
  else
    {
      *str = 0x20;
      str++;
    }
  
    if(rd == 1)           // Если один разряд
    {
      if(dig >= 10) 
        {
          str--; 
          *str = 'F'; // Переполнение
          for(uint8_t i=0; i<rd; i++)
            {
              str++;
              *str = 0x20;
            }
          return ;
        }
      *str = dig + 0x30;// Записали символ в массив
      str++;
      *str = 0x00;      // Записали пустой символ как конец строки
      return ;
    }
  
 
  switch(rd)            // Создали значение для вычисления разрядов
    {
      case 2:
        if(dig >= 100) 
        {
          str--; 
          *str = 'F'; // Переполнение
          for(uint8_t i=0; i<rd; i++)
            {
              str++;
              *str = 0x20;
            }
          return ;
        }
        temp = 10;
        break;
      case 3:
        if(dig >= 1000) 
        {
          str--; 
          *str = 'F'; // Переполнение
          for(uint8_t i=0; i<rd; i++)
            {
              str++;
              *str = 0x20;
            }
          return ;
        }
        temp = 100;
        break;
      case 4:
        if(dig >= 10000) 
        {
          str--; 
          *str = 'F'; // Переполнение
          for(uint8_t i=0; i<rd; i++)
            {
              str++;
              *str = 0x20;
            }
          return ;
        }
        temp = 1000;
        break;
      case 5:
        if(dig >= 100000) 
        {
          str--; 
          *str = 'F'; // Переполнение
          for(uint8_t i=0; i<rd; i++)
            {
              str++;
              *str = 0x20;
            }
          return ;
        }
        temp = 10000;
        break;
    }
  
  for(uint8_t i=0; i<rd; i++) // Количество итераций зависит от количества разрядов
    {
      *str = 0x30;
      while(dig >= temp)
        {
          *str = (*str + 1);
          dig -= temp;
        }
      str++;
      temp /= 10;
    }
  *str = 0x00;
}

// Вывод картинки из массива
void lcd_st7735s_img(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const uint16_t *pix)
{
  lcd_st7735s_at(x, y, w+x-1, h+y-1);
  lcd_st7735s_send_cmd(0x2C);
	pix = pix + w * h -1;
	uint16_t Npix = w * h;
  while(Npix > 0) // Крутимся пока не заполнятся все ряды
  {
    lcd_st7735s_putpix(0, 0, *pix);
    pix--;
    Npix--;
  } 
}
//Массив конвертации RGB(332) to RGB (565)
const uint16_t rgb8_to_rgb16[] = {
0x0000, 0x000A, 0x0014, 0x001F, 0x0120, 0x012A, 0x0134, 0x013F,
0x0240, 0x024A, 0x0254, 0x025F, 0x0360, 0x036A, 0x0374, 0x037F,
0x0480, 0x048A, 0x0494, 0x049F, 0x05A0, 0x05AA, 0x05B4, 0x05BF,
0x06C0, 0x06CA, 0x06D4, 0x06DF, 0x07E0, 0x07EA, 0x07F4, 0x07FF,
0x2000, 0x200A, 0x2014, 0x201F, 0x2120, 0x212A, 0x2134, 0x213F,
0x2240, 0x224A, 0x2254, 0x225F, 0x2360, 0x236A, 0x2374, 0x237F,
0x2480, 0x248A, 0x2494, 0x249F, 0x25A0, 0x25AA, 0x25B4, 0x25BF,
0x26C0, 0x26CA, 0x26D4, 0x26DF, 0x27E0, 0x27EA, 0x27F4, 0x27FF,
0x4000, 0x400A, 0x4014, 0x401F, 0x4120, 0x412A, 0x4134, 0x413F,
0x4240, 0x424A, 0x4254, 0x425F, 0x4360, 0x436A, 0x4374, 0x437F,
0x4480, 0x448A, 0x4494, 0x449F, 0x45A0, 0x45AA, 0x45B4, 0x45BF,
0x46C0, 0x46CA, 0x46D4, 0x46DF, 0x47E0, 0x47EA, 0x47F4, 0x47FF,
0x6800, 0x680A, 0x6814, 0x681F, 0x6920, 0x692A, 0x6934, 0x693F,
0x6A40, 0x6A4A, 0x6A54, 0x6A5F, 0x6B60, 0x6B6A, 0x6B74, 0x6B7F,
0x6C80, 0x6C8A, 0x6C94, 0x6C9F, 0x6DA0, 0x6DAA, 0x6DB4, 0x6DBF,
0x6EC0, 0x6ECA, 0x6ED4, 0x6EDF, 0x6FE0, 0x6FEA, 0x6FF4, 0x6FFF,
0x8800, 0x880A, 0x8814, 0x881F, 0x8920, 0x892A, 0x8934, 0x893F,
0x8A40, 0x8A4A, 0x8A54, 0x8A5F, 0x8B60, 0x8B6A, 0x8B74, 0x8B7F,
0x8C80, 0x8C8A, 0x8C94, 0x8C9F, 0x8DA0, 0x8DAA, 0x8DB4, 0x8DBF,
0x8EC0, 0x8ECA, 0x8ED4, 0x8EDF, 0x8FE0, 0x8FEA, 0x8FF4, 0x8FFF,
0xB000, 0xB00A, 0xB014, 0xB01F, 0xB120, 0xB12A, 0xB134, 0xB13F,
0xB240, 0xB24A, 0xB254, 0xB25F, 0xB360, 0xB36A, 0xB374, 0xB37F,
0xB480, 0xB48A, 0xB494, 0xB49F, 0xB5A0, 0xB5AA, 0xB5B4, 0xB5BF,
0xB6C0, 0xB6CA, 0xB6D4, 0xB6DF, 0xB7E0, 0xB7EA, 0xB7F4, 0xB7FF,
0xD000, 0xD00A, 0xD014, 0xD01F, 0xD120, 0xD12A, 0xD134, 0xD13F,
0xD240, 0xD24A, 0xD254, 0xD25F, 0xD360, 0xD36A, 0xD374, 0xD37F,
0xD480, 0xD48A, 0xD494, 0xD49F, 0xD5A0, 0xD5AA, 0xD5B4, 0xD5BF,
0xD6C0, 0xD6CA, 0xD6D4, 0xD6DF, 0xD7E0, 0xD7EA, 0xD7F4, 0xD7FF,
0xF800, 0xF80A, 0xF814, 0xF81F, 0xF920, 0xF92A, 0xF934, 0xF93F,
0xFA40, 0xFA4A, 0xFA54, 0xFA5F, 0xFB60, 0xFB6A, 0xFB74, 0xFB7F,
0xFC80, 0xFC8A, 0xFC94, 0xFC9F, 0xFDA0, 0xFDAA, 0xFDB4, 0xFDBF,
0xFEC0, 0xFECA, 0xFED4, 0xFEDF, 0xFFE0, 0xFFEA, 0xFFF4, 0xFFFF
};
void lcd_st7735s_img8(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const uint8_t *pix)
{
  lcd_st7735s_at(x, y, w+x-1, h+y-1);
  lcd_st7735s_send_cmd(0x2C);
	pix = pix + w * h -1;
	uint16_t Npix = w * h;
  while(Npix > 0) // Крутимся пока не заполнятся все ряды
  {
//RGB(232) to RGB (464)
    lcd_st7735s_putpix(0, 0, rgb8_to_rgb16[*pix]);
    pix--;
    Npix--;
  } 
}
/*----------------------------------------------------------------*/
void Convert2Array(uint8_t num) //Конвертирует десятичное число в массив по разрядам Ex. 257 -> {2,5,7}
/*----------------------------------------------------------------*/
{
	uint8_t digits_count=0;	
	uint8_t limit=3;
	digits[0]=0;
	digits[1]=0;		
	digits[2]=0;
	if (num <= 0) { return; }
	while (num > 0 && limit) 
	{
		digits[2 - digits_count++] = num % 10;
    num /= 10;
    limit--;
  }
	return;
};
/*----------------------------------------------------------------*/
void ResetTempNumbers(void) //Выводит меню
/*----------------------------------------------------------------*/
{
  temp_bat = 250;
  temp_spd = 250;
  temp_wgt = 250;
}
/*----------------------------------------------------------------*/
void DisplayOutMenu(void) //Выводит меню
/*----------------------------------------------------------------*/
{
  lcd_st7735s_CS_0();
  lcd_st7735s_fillrect(0,0,127,159,rgb8_to_rgb16[0xFF]);
  lcd_st7735s_img8(100,116, width_bat_img, height_bat_img, bat_img);
  lcd_st7735s_img8(70,57, width_speed_img, height_speed_img, speed_img);
  lcd_st7735s_img8(70,7, width_weight_img, height_weight_img, weight_img);
  DisplayOutLowBattery_Status = 0;
  lcd_st7735s_CS_1();
}
/*----------------------------------------------------------------*/
void DisplayOutSpeed(uint8_t spd) //Выводит скорость
/*----------------------------------------------------------------*/
{
  lcd_st7735s_CS_0();
  Convert2Array(spd);
  lcd_st7735s_img8(39,62, *disp_w[digits[1]], *disp_h[digits[1]], disp_p[digits[1]]);
  lcd_st7735s_img8(12,62, *disp_w[digits[2]], *disp_h[digits[2]], disp_p[digits[2]]);
  lcd_st7735s_CS_1();
}
/*----------------------------------------------------------------*/
void DisplayOutWeight(uint8_t wgt) //Выводит силу
/*----------------------------------------------------------------*/
{
  lcd_st7735s_CS_0();
  Convert2Array(wgt);
  lcd_st7735s_img8(39,7, *disp_w[digits[1]], *disp_h[digits[1]], disp_p[digits[1]]);
  lcd_st7735s_img8(12,7, *disp_w[digits[2]], *disp_h[digits[2]], disp_p[digits[2]]);
  lcd_st7735s_CS_1();
}
/*----------------------------------------------------------------*/
void DisplayOutAvailableChargeIcon(uint8_t bat) //Выводит иконку остаточный заряд батареи
/*----------------------------------------------------------------*/
{
  lcd_st7735s_CS_0();
  if (bat > 15) 
  {
    lcd_st7735s_fillrect(104,(120 + bat*24/100 ),113,143,0xFFFF);
    lcd_st7735s_fillrect(104,120,113,(119 + bat*24/100 ),0x65DC);
  }
  else if ((bat <= 15) && (bat >= 5))
  {
    lcd_st7735s_fillrect(104,(120 + bat*24/100 ),113,143,0xFFFF);
    lcd_st7735s_fillrect(104,120,113,(119 + bat*24/100 ),0xF800);
  }
  else
  {
    lcd_st7735s_fillrect(104,121,113,143,0xFFFF);
    lcd_st7735s_fillrect(104,120,113,120,0xF800);
  }
  DisplayOutAvailableChargeNumb(bat);
  lcd_st7735s_CS_1();
}
/*----------------------------------------------------------------*/
void DisplayOutAvailableChargeNumb(uint8_t bat) //Выводит иконку остаточный заряд батареи цифры
/*----------------------------------------------------------------*/
{
  Convert2Array(bat);
  lcd_st7735s_img8(12,117, *disp_w[digits[2]], *disp_h[digits[2]], disp_p[digits[2]]);
  if (bat > 9)
  {
    lcd_st7735s_img8(39,117, *disp_w[digits[1]], *disp_h[digits[1]], disp_p[digits[1]]);
    if (bat > 99)
    {
      lcd_st7735s_img8(66,117, *disp_w[digits[0]], *disp_h[digits[0]], disp_p[digits[0]]);
    }
    else
    {
      lcd_st7735s_fillrect(66,117,91,146,0xFFFF);
    }
  }
  else
  {
    lcd_st7735s_fillrect(39,117,91,176,0xFFFF);
  }
}
/*----------------------------------------------------------------*/
void DisplayOutChargingIcon(uint8_t bat) //Выводит иконку батарейки
/*----------------------------------------------------------------*/
{
  lcd_st7735s_CS_0();
  lcd_st7735s_img8(104,120, width_charge_img, height_charge_img, charge_img);
  Sound_Play(track_charging);
  lcd_st7735s_CS_1();
}
/*----------------------------------------------------------------*/
void DisplayOutPrepering(void) //Выводит экран подготовки
/*----------------------------------------------------------------*/
{
  lcd_st7735s_CS_1();
  lcd_st7735s_fillrect(0,0,127,159,rgb8_to_rgb16[0x0F]);
  lcd_st7735s_img8(5,69, width_preparing_img, height_preparing_img, preparing_img);
  lcd_st7735s_CS_1();
}
/*----------------------------------------------------------------*/
void DisplayOutStop(void) //Выводит Аварийный экран
/*----------------------------------------------------------------*/
{
  lcd_st7735s_CS_0();
  lcd_st7735s_img8(0,0, width_stop_img, height_stop_img, stop_img);
  lcd_st7735s_CS_1();
}
/*----------------------------------------------------------------*/
void DisplayOutLowBattery(void) //Выводит сообщение о низком заряде
/*----------------------------------------------------------------*/
{
  lcd_st7735s_CS_0();
  lcd_st7735s_fillrect(0,0,127,159,rgb8_to_rgb16[0xFF]);
  lcd_st7735s_img8(3,51, width_lowbattery_img, height_lowbattery_img, lowbattery_img);
  if (DisplayOutLowBattery_Status == 0)
  {
    DisplayOutLowBattery_Status = 1;
    Sound_Play(track_charge);
  }
  lcd_st7735s_CS_1();
}
/*----------------------------------------------------------------*/
void DisplayOut(uint8_t bat, uint8_t spd, uint8_t wgt)
/*----------------------------------------------------------------*/
{
  if ( GLOBAL_FLAG_RX & STOP_BUTTON_FLAG )
  {
    if ( Display_Status != STOP )
    {
      DisplayOutStop();
      Display_Status = STOP;
      ResetTempNumbers();
    }
  }
  else if ( GLOBAL_FLAG_RX & INITIALIZATION_FLAG )
  {
    if ( Display_Status != PREPERING )
    {
      DisplayOutPrepering();
      Display_Status = PREPERING;
      ResetTempNumbers();
    }
  }
  else if (( LL_TIM_GetCounter(TIM_Sleep) == 20 )  &&
           (!(GLOBAL_CHARGE_FLAG & CHARGING_FLAG)) &&
           ( bat <= 10 ))
  {
    if ( Display_Status != LOW_BAT )
    {
      DisplayOutLowBattery();
      Display_Status = LOW_BAT;
      ResetTempNumbers();
    }
  }
  else if (( Charger_Status == 0 )  &&
           ( GLOBAL_CHARGE_FLAG & CHARGING_FLAG ))
  {
    DisplayOutChargingIcon(bat);
    Charger_Status = 1;
  }
  else if (( Display_Status != MENU )  &&
           ( Display_Status != LOW_BAT ))
  {
    DisplayOutMenu();
    Display_Status = MENU;
  }
  else if (( bat != temp_bat )  &&
           (!(GLOBAL_CHARGE_FLAG & CHARGING_FLAG )))
  {
    DisplayOutAvailableChargeIcon(bat);
    Charger_Status = 0;
    temp_bat = bat;
  }
  else if ( spd != temp_spd )
  {
    DisplayOutSpeed(spd);
    temp_spd = spd;
  }
  else if ( wgt != temp_wgt )
  {
    DisplayOutWeight(wgt);
    temp_wgt = wgt;
  }
 /* else if (( NumFlagSleepDeny != 0 ) ||
          (GLOBAL_CHARGE_FLAG & CHARGING_FLAG))
  {
    Display_Status = CLEAR;
  }*/
}
