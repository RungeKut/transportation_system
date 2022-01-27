
#ifndef ST7735_H
#define ST7735_H
#include "main.h"

//-----------------------------------------------------------------------------/
// Function COMMAND DISPLAY 
//-----------------------------------------------------------------------------/
#define ST7735S_NOP				0x00	//p.107	
#define ST7735S_SWRESET		0x01	//Software Reset p.108
#define ST7735S_RDDID			0x04	//Read Display ID p.109
#define ST7735S_RDDST			0x09	//Read Display Status p.110
#define ST7735S_RDDPM			0x0A	//Read Display Power Mode p.112
#define ST7735S_RDDMADCTL	0x0B	//Read Display MADCTL p.113
#define ST7735S_RDDCOLMOD	0x0C	//Read Display Pixel Format p.114
#define ST7735S_RDDIM			0x0D	//Read Display Image Mode p.115
#define ST7735S_RDDSM			0x0E	//Read Display Signal Mode p.116
#define ST7735S_RDDSDR		0x0F	//Read Display Self-Diagnostic Result p.118
#define ST7735S_SLPIN			0x10	//Sleep In p.119
#define ST7735S_SLPOUT		0x11	//Sleep Out p.120
#define ST7735S_PTLON			0x12	//Partial Display Mode On p.121
#define ST7735S_NORON			0x13	//Normal Display Mode On p.122
#define ST7735S_INVOFF		0x20	//Display Inversion Off p.123
#define ST7735S_INVON			0x21	//Display Inversion On p.124
#define ST7735S_GAMSET		0x26	//Gamma Set p.125
#define ST7735S_DISPOFF		0x28	//Display Off p.126
#define ST7735S_DISPON		0x29	//Display On p.127
#define ST7735S_CASET			0x2A	//Column Address Set p.128
#define ST7735S_RASET			0x2B	//Row Address Set p.130
#define ST7735S_RAMWR			0x2C	//Memory Write p.132
#define ST7735S_RGBSET		0x2D	//Color Setting for 4K, 65K and 262K p.133
#define ST7735S_RAMRD			0x2E	//Memory Read p.134
#define ST7735S_PTLAR			0x30	//Partial Area p.135
#define ST7735S_SCRLAR		0x33	//Scroll Area Set p.137
#define ST7735S_TEOFF			0x34	//Tearing Effect Line OFF p.139
#define ST7735S_TEON			0x35	//Tearing Effect Line ON p.140
#define ST7735S_MADCTL		0x36	//Memory Data Access Control p.142
#define ST7735S_VSCSAD		0x37	//Vertical Scroll Start Address of RAM p.145
#define ST7735S_IDMOFF		0x38	//Idle Mode Off p.147
#define ST7735S_IDMON			0x39	//Idle Mode On  p.148
#define ST7735S_COLMOD		0x3A	//Interface Pixel Format p.150
#define ST7735S_RDID1			0xDA	//Read ID1 Value p.151
#define ST7735S_RDID2			0xDB	//Read ID2 Value p.152
#define ST7735S_RDID3			0xDC	//Read ID3 Value p.154
#define ST7735S_FRMCTR1		0xB1	//Frame Rate Control	In normal mode/ Full colors) p.159
#define ST7735S_FRMCTR2		0xB2	//Frame Rate Control	In Idle mode/ 8-colors) p.160
#define ST7735S_FRMCTR3		0xB3	//Frame Rate Control	In Partial mode/ full colors) p.161
#define ST7735S_INVCTR		0xB4	//Display Inversion Control p.162
#define ST7735S_PWCTR1		0xC0	//Power Control 1 p.163
#define ST7735S_PWCTR2		0xC1	//Power Control 2 p.165
#define ST7735S_PWCTR3		0xC2	//Power Control 3	in Normal mode/ Full colors) p.167
#define ST7735S_PWCTR4		0xC3	//Power Control 4	in Idle mode/ 8-colors) p.169
#define ST7735S_PWCTR5		0xC4	//Power Control 5	in Partial mode/ full-colors) p.171
#define ST7735S_VMCTR1		0xC5	//VCOM Control 1 p.173
#define ST7735S_VMOFCTR		0xC7	//VCOM Offset Control p.175
#define ST7735S_WRID2			0xD1	//Write ID2 Value p.177
#define ST7735S_WRID3			0xD2	//Write ID3 Value p.178
#define ST7735S_NVFCTR1		0xD9	//NVM Control Status p.179
#define ST7735S_NVFCTR2		0xDE	//NVM Read Command p.180
#define ST7735S_NVFCTR3		0xDF	//NVM Write Command p.181
#define ST7735S_GMCTRP1		0xE0	//Gamma	‘+’polarity) Correction Characteristics Setting p.182
#define ST7735S_GMCTRN1		0xE1	//Gamma ‘-’polarity Correction Characteristics Setting p.184
#define ST7735S_GCV				0xFC	//Gate Pump Clock Frequency Variable p.186
//-----------------------------------------------------------------------------/
// Флаг работы с дисплеем
//-----------------------------------------------------------------------------/
#define DISPLAY_MENU       0x01  //На экране меню
#define DISPLAY_PREPERING  0x02  //На экране подготовка
#define DISPLAY_STOP       0x04  //На экране Авария
extern volatile uint8_t DISPLAY_FLAG;

//-----------------------------------------------------------------------------/
// Дополнительные псевдонимы
//-----------------------------------------------------------------------------/
extern volatile uint8_t digits[];
extern uint8_t *disp_w[];
extern uint8_t *disp_h[];
extern uint8_t *disp_p[];

// Длительность паузы в миллисекундах
#define LCD_DELAY         1

//-----------------------------------------------------------------------------/
// Макроопределения
//-----------------------------------------------------------------------------/

// Перевод 32 битного цвета в 16 битный
#define RGB16(color) (uint16_t)(((color&0xF80000)>>8)|((color&0x00FC00)>>5)|((color&0x0000F8)>>3))

//-----------------------------------------------------------------------------/
// Объявления функций управления
//-----------------------------------------------------------------------------/
void st7735_StartUp(void);

//Разрешаем работу DWT(Data Watchpoint) счетчика
void delaySetNs(uint32_t ns);

//Сама функция задержки
void delayNs(void);

//Установка низкого уровня ChipSelect
void lcd_st7735s_CS_0(void);

//Установка высокого уровня ChipSelect
void lcd_st7735s_CS_1(void);

//Установка низкого уровня Reset
void lcd_st7735s_RES_0(void);

//Установка высокого уровня Reset
void lcd_st7735s_RES_1(void);
	
// Aeyrwbz djpdhfoftn vjlekm xbckf
uint8_t abs(int x);

// Инициализация дисплея
void lcd_st7735s_init(void); 

// Отправка данных/команд на дисплей
void lcd_st7735s_send(uint16_t data);

// Отправка команды на дисплей
void lcd_st7735s_send_cmd(uint8_t cmd);

// Отправка данных на дисплей с ожиданием конца передачи
void lcd_st7735s_send_data(uint8_t data);

// Заполнение прямоугольной области экрана
void lcd_st7735s_fillrect(uint8_t startX, uint8_t startY, uint8_t stopX, uint8_t stopY, uint16_t color);

// Вывод пиксела
void lcd_st7735s_putpix(uint8_t x, uint8_t y, uint16_t Color);

// Вывод линии
void lcd_st7735s_line(int x0, int y0, int x1, int y1, uint16_t color);

// Рисование прямоугольника (не заполненного)
void lcd_st7735s_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);

// Вывод символа в цвете по координатам
void lcd_st7735s_putchar(uint8_t x, uint8_t y, uint8_t chr, uint16_t charColor, uint16_t bkgColor);

// Вывод строки в цвете по координатам
void lcd_st7735s_putstr(uint8_t x, uint8_t y, const uint8_t str[], uint16_t charColor, uint16_t bkgColor);

// Вывод строки в цвете по координатам строки
void lcd_st7735s_putstr_xy(uint8_t x, uint8_t y, const uint8_t str[], uint16_t charColor, uint16_t bkgColor);

//Форматирование строки.
void lcd_format_int(uint8_t *str, int dig, uint8_t rd);

// Вывод картинки из массива
void lcd_st7735s_img(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const uint16_t *pix);
void lcd_st7735s_img8(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const uint8_t *pix);

// Конвертация числа в массив номеров картинок (шрифтов)
void Convert2Array(uint8_t);

// Верстка дисплея из картинок
void DisplayOut(uint8_t, uint8_t, uint8_t);

#endif /* ST7735_H */
