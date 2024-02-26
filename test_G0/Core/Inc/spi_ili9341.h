#include "stm32g0xx.h"

//#ifndef __ILI9341_H__
//#define __ILI9341_H__

#include "fonts.h"
#include <stdbool.h>

#define ILI9341_MADCTL_MY  0x80 	//MY – то же самое только для оси У
#define ILI9341_MADCTL_MX  0x40 //пределяет с какой стороны дисплея Х будет равен нулю, с правой или с левой, соответственно с другой стороны дисплея Х будет максимальным
#define ILI9341_MADCTL_MV  0x20	//MV – меняет оси X и Y местами
#define ILI9341_MADCTL_ML  0x10	//ML – определяет порядок обновления выделенной области по вертикали
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04	//MH – определяет порядок обновления выделенной области по горизонтали


// default orientation
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)



// Color definitions
#define	ILI9341_BLACK   0x0000
#define	ILI9341_BLUE    0x001F
#define	ILI9341_RED     0xF800
#define	ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_WHITE   0xFFFF
#define ILI9341_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))




void TFT9341_LED_ON(void);


void TFT9341_ini(uint16_t w_size, uint16_t h_size);
void TFT9341_reset(void);
void TFT9341_SendCommand(uint8_t cmd);
void TFT9341_SendData(uint8_t dt);
void TFT9341_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT9341_FillScreen(uint16_t color);


void TFT9341_DrawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color);
void TFT9341_SetTextColor(uint16_t color);
void TFT9341_SetBackColor(uint16_t color);
void TFT9341_SetFont(sFONT *pFonts);
void TFT9341_DrawChar(uint16_t x, uint16_t y, uint8_t c);
void TFT9341_String(uint16_t x,uint16_t y, char *str);
void TFT9341_SetRotation(uint8_t r);
void TFT9341_FillRect_DMA(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT9341_FillRect_T(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t mass);
void massiv_code(uint16_t x1, uint16_t y1, uint8_t number);


#define swap(a,b) {int16_t t=a;a=b;b=t;}

#define TFT9341_MADCTL_MY  0x80
#define TFT9341_MADCTL_MX  0x40
#define TFT9341_MADCTL_MV  0x20
#define TFT9341_MADCTL_ML  0x10
#define TFT9341_MADCTL_RGB 0x00
#define TFT9341_MADCTL_BGR 0x08
#define TFT9341_MADCTL_MH  0x04
#define TFT9341_ROTATION (TFT9341_MADCTL_MX | TFT9341_MADCTL_BGR)
#define	TFT9341_BLACK   0x0000
#define	TFT9341_BLUE    0x001F
#define	TFT9341_RED     0xF800
#define	TFT9341_GREEN   0x07E0
#define TFT9341_CYAN    0x07FF
#define TFT9341_MAGENTA 0xF81F
#define TFT9341_YELLOW  0xFFE0
#define TFT9341_WHITE   0xFFFF



//-------------------------------------------------------------------
#define RESET_ACTIVE() LL_GPIO_ResetOutputPin(RST_LCD_GPIO_Port, RST_LCD_Pin);//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET)
#define RESET_IDLE() LL_GPIO_SetOutputPin(RST_LCD_GPIO_Port, RST_LCD_Pin);//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET)
#define CS_ACTIVE() LL_GPIO_ResetOutputPin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin )//CS_ACTIVE() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
#define CS_IDLE() LL_GPIO_SetOutputPin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin )// HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
#define DC_COMMAND() LL_GPIO_ResetOutputPin(RS_LCD_GPIO_Port, RS_LCD_Pin)//DC_COMMAND() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET)
#define DC_DATA() LL_GPIO_SetOutputPin(RS_LCD_GPIO_Port, RS_LCD_Pin)//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET)
//-------------------------------------------------------------------


//#endif // __ILI9341_H__
