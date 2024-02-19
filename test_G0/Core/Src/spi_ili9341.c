#include "main.h"
#include "spi_ili9341.h"
#include "gpio.h"
#include <stddef.h>

uint16_t TFT9341_WIDTH;
uint16_t TFT9341_HEIGHT;


uint16_t TFT9341_WIDTH;
uint16_t TFT9341_HEIGHT;
//-------------------------------------------------------------------


static void TFT9341_WriteData(uint8_t* buff, size_t buff_size) {
	DC_DATA();
	while(buff_size > 0) {
		uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
			for(int16_t i = 0;i<buff_size;i++){
				LL_SPI_TransmitData8(SPI1, buff[i]);
				while (SPI1->SR & SPI_SR_BSY);
			}
		buff += chunk_size;
		buff_size -= chunk_size;
	}
}

//-------------------------------------------------------------------
static void TFT9341_SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  // column address set
  TFT9341_SendCommand(0x2A); // CASET
  {
    uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
    TFT9341_WriteData(data, sizeof(data));
  }

  // row address set
  TFT9341_SendCommand(0x2B); // RASET
  {
    uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
    TFT9341_WriteData(data, sizeof(data));
  }

  // write to RAM
  TFT9341_SendCommand(0x2C); // RAMWR
}//-------------------------------------------------------------------
void TFT9341_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  if((x1 >= TFT9341_WIDTH) || (y1 >= TFT9341_HEIGHT) || (x2 >= TFT9341_WIDTH) || (y2 >= TFT9341_HEIGHT)) return;
	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
  TFT9341_SetAddrWindow(x1, y1, x2, y2);
  uint8_t data[] = { color >> 8, color & 0xFF };
  DC_DATA();
  for(uint32_t i = 0; i < (x2-x1+1)*(y2-y1+1); i++)
  {
				//HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
				LL_SPI_TransmitData8(SPI1, data[0]);
				while (SPI1->SR & SPI_SR_BSY);
				LL_SPI_TransmitData8(SPI1, data[1]);
				while (SPI1->SR & SPI_SR_BSY);		
  }
}
//-------------------------------------------------------------------
void TFT9341_FillScreen(uint16_t color)
{
  TFT9341_FillRect(0, 0, TFT9341_WIDTH-1, TFT9341_HEIGHT-1, color);
}

void TFT9341_ini(uint16_t w_size, uint16_t h_size)
{
  uint8_t data[15];
	CS_ACTIVE();
	 TFT9341_reset();
  //Software Reset
  TFT9341_SendCommand(0x01);
  LL_mDelay(1000);
	
	 //Power Control A  
  data[0] = 0x39;
  data[1] = 0x2C;
  data[2] = 0x00;
  data[3] = 0x34;
  data[4] = 0x02;
  TFT9341_SendCommand(0xCB);
  TFT9341_WriteData(data, 5);
  //Power Control B
  data[0] = 0x00;
  data[1] = 0xC1;
  data[2] = 0x30;
  TFT9341_SendCommand(0xCF);
  TFT9341_WriteData(data, 3);
  //Driver timing control A
  data[0] = 0x85;
  data[1] = 0x00;
  data[2] = 0x78;
  TFT9341_SendCommand(0xE8);
  TFT9341_WriteData(data, 3);
  //Driver timing control B
  data[0] = 0x00;
  data[1] = 0x00;
  TFT9341_SendCommand(0xEA);
  TFT9341_WriteData(data, 2);
  //Power on Sequence control
  data[0] = 0x64;
  data[1] = 0x03;
  data[2] = 0x12;
  data[3] = 0x81;
  TFT9341_SendCommand(0xED);
  TFT9341_WriteData(data, 4);
  //Pump ratio control
  data[0] = 0x20;
  TFT9341_SendCommand(0xF7);
  TFT9341_WriteData(data, 1);
  //Power Control,VRH[5:0]
  data[0] = 0x10;
  TFT9341_SendCommand(0xC0);
  TFT9341_WriteData(data, 1);
  //Power Control,SAP[2:0];BT[3:0]
  data[0] = 0x10;
  TFT9341_SendCommand(0xC1);
  TFT9341_WriteData(data, 1);
  //VCOM Control 1
  data[0] = 0x3E;
  data[1] = 0x28;
  TFT9341_SendCommand(0xC5);
  TFT9341_WriteData(data, 2);
  //VCOM Control 2
  data[0] = 0x86;
  TFT9341_SendCommand(0xC7);
  TFT9341_WriteData(data, 1);
  //Memory Acsess Control
  data[0] = 0x48;
  TFT9341_SendCommand(0x36);
  TFT9341_WriteData(data, 1);
  //Pixel Format Set
  data[0] = 0x55;//16bit
  TFT9341_SendCommand(0x3A);
  TFT9341_WriteData(data, 1);
  //Frame Rratio Control, Standard RGB Color
  data[0] = 0x00;
  data[1] = 0x18;
  TFT9341_SendCommand(0xB1);
  TFT9341_WriteData(data, 2);
  //Display Function Control
  data[0] = 0x08;
  data[1] = 0x82;
  data[2] = 0x27;//320 строк
  TFT9341_SendCommand(0xB6);
  TFT9341_WriteData(data, 3);
  //Enable 3G (пока не знаю что это за режим)
  data[0] = 0x00;//не включаем
  TFT9341_SendCommand(0xF2);
  TFT9341_WriteData(data, 1);
  //Gamma set
  data[0] = 0x01;//Gamma Curve (G2.2) (Кривая цветовой гаммы)
  TFT9341_SendCommand(0x26);
  TFT9341_WriteData(data, 1);
  //Positive Gamma  Correction
  data[0] = 0x0F;
  data[1] = 0x31;
  data[2] = 0x2B;
  data[3] = 0x0C;
  data[4] = 0x0E;
  data[5] = 0x08;
  data[6] = 0x4E;
  data[7] = 0xF1;
  data[8] = 0x37;
  data[9] = 0x07;
  data[10] = 0x10;
  data[11] = 0x03;
  data[12] = 0x0E;
  data[13] = 0x09;
  data[14] = 0x00;
  TFT9341_SendCommand(0xE0);
  TFT9341_WriteData(data, 15);
  //Negative Gamma  Correction
  data[0] = 0x00;
  data[1] = 0x0E;
  data[2] = 0x14;
  data[3] = 0x03;
  data[4] = 0x11;
  data[5] = 0x07;
  data[6] = 0x31;
  data[7] = 0xC1;
  data[8] = 0x48;
  data[9] = 0x08;
  data[10] = 0x0F;
  data[11] = 0x0C;
  data[12] = 0x31;
  data[13] = 0x36;
  data[14] = 0x0F;
  TFT9341_SendCommand(0xE1);
  TFT9341_WriteData(data, 15);
  TFT9341_SendCommand(0x11);//Выйдем из спящего режима
	LL_mDelay(150);
	
	 //Display ON
  data[0] = TFT9341_ROTATION;
  TFT9341_SendCommand(0x29);
	
	TFT9341_WriteData(data, 1);
  TFT9341_WIDTH = w_size;
  TFT9341_HEIGHT = h_size;
}

void TFT9341_reset(void)
{
	RESET_ACTIVE();
	LL_mDelay(5);
	RESET_IDLE();
}
void TFT9341_SendCommand(uint8_t cmd)
{
	DC_COMMAND();
	LL_SPI_TransmitData8(SPI1, cmd);
	while (SPI1->SR & SPI_SR_BSY);
}

void TFT9341_SendData(uint8_t dt)
{
	DC_DATA();
	LL_SPI_TransmitData8(SPI1, dt);
	while (SPI1->SR & SPI_SR_BSY);
}













//static void ILI9341_Select() {
//    LL_GPIO_ResetOutputPin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin );//HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_RESET);
//}	


 void ILI9341_LED_ON(void) {
    LL_GPIO_SetOutputPin(LCD_LED_GPIO_Port,  LCD_LED_Pin);//HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_RESET);
}	

//void ILI9341_Unselect() {
// LL_GPIO_SetOutputPin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin );//;// HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_SET);
//}

//static void ILI9341_Reset() {
//    LL_GPIO_ResetOutputPin(RST_LCD_GPIO_Port, RST_LCD_Pin);//HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_RESET);
//    LL_mDelay(5);
//    LL_GPIO_SetOutputPin(RST_LCD_GPIO_Port, RST_LCD_Pin);//HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_SET);
//}

//static void ILI9341_WriteCommand(uint8_t cmd) {
//    LL_GPIO_ResetOutputPin(RS_LCD_GPIO_Port, RS_LCD_Pin);// HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_RESET);	//DC -->A0 
//		LL_SPI_TransmitData8(SPI1, cmd);//HAL_SPI_Transmit(&ILI9341_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
//		while (SPI1->SR & SPI_SR_BSY);
//    
//}

//static void ILI9341_WriteData(uint8_t* buff, size_t buff_size) {
//     LL_GPIO_SetOutputPin(RS_LCD_GPIO_Port, RS_LCD_Pin);//HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);
//	
//		 while (SPI1->SR & SPI_SR_BSY);
//	
//		 for(int16_t i = 0;i<buff_size;i++){
//				LL_SPI_TransmitData8(SPI1, buff[i]);
//				while (SPI1->SR & SPI_SR_BSY);
//		 }
//	
//	
//    // split data in small chunks because HAL can't send more then 64K at once оганичение на передачу 64 кбайт
////    while(buff_size > 0) {
////        uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
////        HAL_SPI_Transmit(&ILI9341_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
////        buff += chunk_size;
////        buff_size -= chunk_size;
////    }
//}

//static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
//    // column address set
//    ILI9341_WriteCommand(0x2A); // CASET
//    {
//        uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // row address set
//    ILI9341_WriteCommand(0x2B); // RASET
//    {
//        uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // write to RAM
//    ILI9341_WriteCommand(0x2C); // RAMWR
//}

//void ILI9341_Init() {
//    ILI9341_Select();
//    ILI9341_Reset();

//    // command list is based on https://github.com/martnak/STM32-ILI9341

//    // SOFTWARE RESET
//    ILI9341_WriteCommand(0x01);
//    LL_mDelay(1000);
//        
//    // POWER CONTROL A
//    ILI9341_WriteCommand(0xCB);
//    {
//        uint8_t data[] = { 0x39, 0x2C, 0x00, 0x34, 0x02 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // POWER CONTROL B
//    ILI9341_WriteCommand(0xCF);
//    {
//        uint8_t data[] = { 0x00, 0xC1, 0x30 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // DRIVER TIMING CONTROL A
//    ILI9341_WriteCommand(0xE8);
//    {
//        uint8_t data[] = { 0x85, 0x00, 0x78 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // DRIVER TIMING CONTROL B
//    ILI9341_WriteCommand(0xEA);
//    {
//        uint8_t data[] = { 0x00, 0x00 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // POWER ON SEQUENCE CONTROL
//    ILI9341_WriteCommand(0xED);
//    {
//        uint8_t data[] = { 0x64, 0x03, 0x12, 0x81 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // PUMP RATIO CONTROL
//    ILI9341_WriteCommand(0xF7);
//    {
//        uint8_t data[] = { 0x20 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // POWER CONTROL,VRH[5:0]
//    ILI9341_WriteCommand(0xC0);
//    {
//        uint8_t data[] = { 0x23 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // POWER CONTROL,SAP[2:0];BT[3:0]
//    ILI9341_WriteCommand(0xC1);
//    {
//        uint8_t data[] = { 0x10 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // VCM CONTROL
//    ILI9341_WriteCommand(0xC5);
//    {
//        uint8_t data[] = { 0x3E, 0x28 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // VCM CONTROL 2
//    ILI9341_WriteCommand(0xC7);
//    {
//        uint8_t data[] = { 0x86 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // MEMORY ACCESS CONTROL
//    ILI9341_WriteCommand(0x36);
//    {
//        uint8_t data[] = { 0x48 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // PIXEL FORMAT
//    ILI9341_WriteCommand(0x3A);
//    {
//        uint8_t data[] = { 0x55 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // FRAME RATIO CONTROL, STANDARD RGB COLOR
//    ILI9341_WriteCommand(0xB1);
//    {
//        uint8_t data[] = { 0x00, 0x18 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // DISPLAY FUNCTION CONTROL
//    ILI9341_WriteCommand(0xB6);
//    {
//        uint8_t data[] = { 0x08, 0x82, 0x27 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // 3GAMMA FUNCTION DISABLE
//    ILI9341_WriteCommand(0xF2);
//    {
//        uint8_t data[] = { 0x00 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // GAMMA CURVE SELECTED
//    ILI9341_WriteCommand(0x26);
//    {
//        uint8_t data[] = { 0x01 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // POSITIVE GAMMA CORRECTION
//    ILI9341_WriteCommand(0xE0);
//    {
//        uint8_t data[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
//                           0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // NEGATIVE GAMMA CORRECTION
//    ILI9341_WriteCommand(0xE1);
//    {
//        uint8_t data[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
//                           0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    // EXIT SLEEP
//    ILI9341_WriteCommand(0x11);
//    LL_mDelay(120);

//    // TURN ON DISPLAY
//    ILI9341_WriteCommand(0x29);

//    // MADCTL
//    ILI9341_WriteCommand(0x36);
//    {
//        uint8_t data[] = { ILI9341_ROTATION };
//        ILI9341_WriteData(data, sizeof(data));
//    }

//    ILI9341_Unselect();
//}

//void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
//    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT))
//        return;

//    ILI9341_Select();

//    ILI9341_SetAddressWindow(x, y, x+1, y+1);
//    uint8_t data[] = { color >> 8, color & 0xFF };
//    ILI9341_WriteData(data, sizeof(data));

//    ILI9341_Unselect();
//}

//static void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
//    uint32_t i, b, j;

//    ILI9341_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);

//    for(i = 0; i < font.height; i++) {
//        b = font.data[(ch - 32) * font.height + i];
//        for(j = 0; j < font.width; j++) {
//            if((b << j) & 0x8000)  {
//                uint8_t data[] = { color >> 8, color & 0xFF };
//                ILI9341_WriteData(data, sizeof(data));
//            } else {
//                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
//                ILI9341_WriteData(data, sizeof(data));
//            }
//        }
//    }
//}

//void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
//    ILI9341_Select();

//    while(*str) {
//        if(x + font.width >= ILI9341_WIDTH) {
//            x = 0;
//            y += font.height;
//            if(y + font.height >= ILI9341_HEIGHT) {
//                break;
//            }

//            if(*str == ' ') {
//                // skip spaces in the beginning of the new line
//                str++;
//                continue;
//            }
//        }

//        ILI9341_WriteChar(x, y, *str, font, color, bgcolor);
//        x += font.width;
//        str++;
//    }

//    ILI9341_Unselect();
//}

//void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
//    // clipping
//    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
//    if((x + w - 1) >= ILI9341_WIDTH) w = ILI9341_WIDTH - x;
//    if((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

//    ILI9341_Select();
//    ILI9341_SetAddressWindow(x, y, x+w-1, y+h-1);

//    uint8_t data[] = { color >> 8, color & 0xFF };
//    LL_GPIO_SetOutputPin(RS_LCD_GPIO_Port, RS_LCD_Pin);//HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);
//    for(y = h; y > 0; y--) {
//        for(x = w; x > 0; x--) {
//            //HAL_SPI_Transmit(&ILI9341_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
//									for(uint16_t i =0;i <sizeof(data);i++){	
//										LL_SPI_TransmitData8(SPI1, data[i]);
//										while (SPI1->SR & SPI_SR_BSY);
//									}
//        }
//    }

//    ILI9341_Unselect();
//}

//void ILI9341_FillScreen(uint16_t color) {
//    ILI9341_FillRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
//}

//void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
//    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
//    if((x + w - 1) >= ILI9341_WIDTH) return;
//    if((y + h - 1) >= ILI9341_HEIGHT) return;

//    ILI9341_Select();
//    ILI9341_SetAddressWindow(x, y, x+w-1, y+h-1);
//    ILI9341_WriteData((uint8_t*)data, sizeof(uint16_t)*w*h);
//    ILI9341_Unselect();
//}

//void ILI9341_InvertColors(bool invert) {
//    ILI9341_Select();
//    ILI9341_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
//    ILI9341_Unselect();
//}

