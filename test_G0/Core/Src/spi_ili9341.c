#include "main.h"
#include "spi_ili9341.h"
#include "stm32g0xx_it.h"
#include "gpio.h"
#include "dma.h"
#include <stddef.h>

#include "font24.c"
#include "font20.c"
#include "font16.c"
#include "font12.c"
#include "font8.c"

uint16_t TFT9341_WIDTH;
uint16_t TFT9341_HEIGHT;


typedef struct
{
	uint16_t TextColor;
	uint16_t BackColor;
	sFONT *pFont;
}LCD_DrawPropTypeDef;
LCD_DrawPropTypeDef lcdprop;
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
//-------------------------------------------------------------------
void TFT9341_DrawPixel(int x, int y, uint16_t color)
{
	if((x<0)||(y<0)||(x>=TFT9341_WIDTH)||(y>=TFT9341_HEIGHT)) return;
	TFT9341_SetAddrWindow(x,y,x,y);
	TFT9341_SendCommand(0x2C);
	TFT9341_SendData(color>>8);
	TFT9341_SendData(color & 0xFF);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void TFT9341_DrawChar(uint16_t x, uint16_t y, uint8_t c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t offset;
  uint8_t *c_t;
  uint8_t *pchar;
  uint32_t line=0;
  height = lcdprop.pFont->Height;
  width  = lcdprop.pFont->Width;
  offset = 8 *((width + 7)/8) -  width ;
  c_t = (uint8_t*) &(lcdprop.pFont->table[(c-' ') * lcdprop.pFont->Height * ((lcdprop.pFont->Width + 7) / 8)]);
  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)c_t + (width + 7)/8 * i);
    switch(((width + 7)/8))
    {
      case 1:
          line =  pchar[0];      
          break;
      case 2:
          line =  (pchar[0]<< 8) | pchar[1];
          break;
      case 3:
      default:
        line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];      
        break;
    }
    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1))) 
      {
        TFT9341_DrawPixel((x + j), y, lcdprop.TextColor);
      }
      else
      {
        TFT9341_DrawPixel((x + j), y, lcdprop.BackColor);
      } 
    }
    y++;      
  }
}
void TFT9341_String(uint16_t x,uint16_t y, char *str)
{
  while(*str)
  {
    TFT9341_DrawChar(x,y,str[0]);
    x+=lcdprop.pFont->Width;
    (void)*str++;
  }
}
//---------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void TFT9341_SetTextColor(uint16_t color)
{
  lcdprop.TextColor=color;
}
//------------------------------------
void TFT9341_SetBackColor(uint16_t color)
{
  lcdprop.BackColor=color;
}
//------------------------------------
void TFT9341_SetFont(sFONT *pFonts)
{
  lcdprop.pFont=pFonts;
}
//---------------------------------------------------------------
//-------------------------------------------------------------------
void TFT9341_FontsIni(void)
{
  Font8.Height = 8;
  Font8.Width = 5;
  Font12.Height = 12;
  Font12.Width = 7;
  Font16.Height = 16;
  Font16.Width = 11;
  Font20.Height = 20;
  Font20.Width = 14;
  Font24.Height = 24;
  Font24.Width = 17;
  lcdprop.BackColor=TFT9341_BLACK;
  lcdprop.TextColor=TFT9341_GREEN;
  lcdprop.pFont=&Font16;
}
//---------------------------------------------------------------


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
				while(!(SPI1->SR & SPI_SR_TXE));
				LL_SPI_TransmitData8(SPI1, data[0]);
				while(!(SPI1->SR & SPI_SR_TXE));	
				LL_SPI_TransmitData8(SPI1, data[1]);
				
  }
}


void TFT9341_FillScreen(uint16_t color)
{
  TFT9341_FillRect(0, 0, TFT9341_WIDTH-1, TFT9341_HEIGHT-1, color);
}//-------------------------------------------------------------------

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
  data[2] = 0x27;//320 �����
  TFT9341_SendCommand(0xB6);
  TFT9341_WriteData(data, 3);
  //Enable 3G (���� �� ���� ��� ��� �� �����)
  data[0] = 0x00;//�� ��������
  TFT9341_SendCommand(0xF2);
  TFT9341_WriteData(data, 1);
  //Gamma set
  data[0] = 0x01;//Gamma Curve (G2.2) (������ �������� �����)
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
  TFT9341_SendCommand(0x11);//������ �� ������� ������
	LL_mDelay(150);
	
	 //Display ON
  data[0] = TFT9341_ROTATION;
  TFT9341_SendCommand(0x29);
	
	TFT9341_WriteData(data, 1);
  TFT9341_WIDTH = w_size;
  TFT9341_HEIGHT = h_size;
	
	 TFT9341_FontsIni();
}//-------------------------------------------------------------------

void TFT9341_reset(void)
{
	RESET_ACTIVE();
	LL_mDelay(5);
	RESET_IDLE();
}//-------------------------------------------------------------------
void TFT9341_SendCommand(uint8_t cmd)
{
	DC_COMMAND();
	LL_SPI_TransmitData8(SPI1, cmd);
	while (SPI1->SR & SPI_SR_BSY);
}//-------------------------------------------------------------------

void TFT9341_SendData(uint8_t dt)
{
	DC_DATA();
	LL_SPI_TransmitData8(SPI1, dt);
	while (SPI1->SR & SPI_SR_BSY);
}//-------------------------------------------------------------------



 void TFT9341_LED_ON(void) {
    LL_GPIO_SetOutputPin(LCD_LED_GPIO_Port,  LCD_LED_Pin);//HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_RESET);
}	//-------------------------------------------------------------------

//-------------------------------------------------------------------

//-------------------------------------------------------------------
void TFT9341_SetRotation(uint8_t r)
{
  TFT9341_SendCommand(0x36);
  switch(r)
  {
    case 0:
      TFT9341_SendData(0x48);
      TFT9341_WIDTH = 240;
      TFT9341_HEIGHT = 320;
      break;
    case 1:
      TFT9341_SendData(0x28);
      TFT9341_WIDTH = 320;
      TFT9341_HEIGHT = 240;
      break;
    case 2:
      TFT9341_SendData(0x88);
      TFT9341_WIDTH = 240;
      TFT9341_HEIGHT = 320;
      break;
    case 3:
      TFT9341_SendData(0xE8);
      TFT9341_WIDTH = 320;
      TFT9341_HEIGHT = 240;
      break;
  }
}
//--------------------------------------------------------------
void TFT9341_FillRect_T(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t mass)
{
 uint32_t i, n, cnt, buf_size;
	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
  TFT9341_SetAddrWindow(x1, y1, x2, y2);
  DC_DATA();
  n = (x2-x1+1)*(y2-y1+1)*2;
  if(n<=65535)
  {
    cnt = 1;
    buf_size = n;
  }
  else
  {
    cnt = n/2;
    buf_size = 2;
    for(i = 3; i < n/3; i++)
    {
      if(n%i == 0)
      {
        cnt = i;
        buf_size = n/i;
        break;
      }
    }
  }
		
//			LL_SPI_Disable(SPI1);
//	LL_SPI_Enable(SPI1);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, mass, LL_SPI_DMA_GetRegAddr(SPI1), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, buf_size);	//2880
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}
		

void massiv_code(uint16_t x1, uint16_t y1, uint8_t number){
uint16_t adr,t=0,a, byte_f48, sh =0;
static	uint16_t automat = 0;
	
	while(dma_end != 1){}
		dma_end = 0;
	switch (number)
  {
  	case 0:
			adr = 192;
			
  		break;
  	case 1:
			adr = 384;	
  		break;
  	case 2:
			adr = 576;				
  		break;
  	case 3:
			adr = 768;				
  		break;
  	case 4:
			adr = 960;	
  		break;
  	case 5:
			adr = 1152;	
  		break;
  	case 6:
			adr = 1344;	
  		break;	
  	case 7:
			adr = 1536;		
  		break;
  	case 8:
			adr = 1728;			
  		break;	
  	case 9:
			adr = 1920;		
  		break;
  }
	if(byte_f48 == 180)sh = 31;
	else if(byte_f48 == 135) sh = 23;
	else sh = 15;
	
	
	if(automat == 0){
		automat = 1;
				for(uint16_t i=0;i<192;i++){
						a = Font48.table[adr+i];
						if(a & 0x80){	OUT_DMA_1[t] = TFT9341_BLUE;
						}
						else OUT_DMA_1[t] = TFT9341_BLACK;

						if(a & 0x40){	OUT_DMA_1[t+1] = TFT9341_BLUE;
						}
						else OUT_DMA_1[t+1] =TFT9341_BLACK;					
						
						if(a & 0x20){	OUT_DMA_1[t+2] = TFT9341_BLUE;
						}
						else OUT_DMA_1[t+2] =TFT9341_BLACK;			
						if(a & 0x10){	OUT_DMA_1[t+3] = TFT9341_BLUE;
						}
						else OUT_DMA_1[t+3] =TFT9341_BLACK;				
						if(a & 0x8){	OUT_DMA_1[t+4] = TFT9341_BLUE;
						}
						else OUT_DMA_1[t+4] =TFT9341_BLACK;						
						if(a & 0x4){	OUT_DMA_1[t+5] = TFT9341_BLUE;
						}
						else OUT_DMA_1[t+5] =TFT9341_BLACK;			
						if(a & 0x2){	OUT_DMA_1[t+6] = TFT9341_BLUE;
						}
						else OUT_DMA_1[t+6] =TFT9341_BLACK;		
						if(a & 0x1){	OUT_DMA_1[t+7] = TFT9341_BLUE;
						}
						else OUT_DMA_1[t+7] =TFT9341_BLACK;		
						t=t+8;	
				}
				//TFT9341_FillRect_T(50, 50, 81, 94, TFT9341_RED);
				TFT9341_FillRect_T(x1,  y1,  x1+31, y1+47, (uint32_t)OUT_DMA_1);
				}
	else {
						for(uint16_t i=0;i<192;i++){
						a = Font48.table[adr+i];
						if(a & 0x80){	OUT_DMA_2[t] = TFT9341_BLUE;
						}
						else OUT_DMA_2[t] = TFT9341_BLACK;

						if(a & 0x40){	OUT_DMA_2[t+1] = TFT9341_BLUE;
						}
						else OUT_DMA_2[t+1] =TFT9341_BLACK;					
						
						if(a & 0x20){	OUT_DMA_2[t+2] = TFT9341_BLUE;
						}
						else OUT_DMA_2[t+2] =TFT9341_BLACK;			
						if(a & 0x10){	OUT_DMA_2[t+3] = TFT9341_BLUE;
						}
						else OUT_DMA_2[t+3] =TFT9341_BLACK;				
						if(a & 0x8){	OUT_DMA_2[t+4] = TFT9341_BLUE;
						}
						else OUT_DMA_2[t+4] =TFT9341_BLACK;						
						if(a & 0x4){	OUT_DMA_2[t+5] = TFT9341_BLUE;
						}
						else OUT_DMA_2[t+5] =TFT9341_BLACK;			
						if(a & 0x2){	OUT_DMA_2[t+6] = TFT9341_BLUE;
						}
						else OUT_DMA_2[t+6] =TFT9341_BLACK;		
						if(a & 0x1){	OUT_DMA_2[t+7] = TFT9341_BLUE;
						}
						else OUT_DMA_2[t+7] =TFT9341_BLACK;		
						t=t+8;	
				}
				//TFT9341_FillRect_T(50, 50, 81, 94, TFT9341_RED);
				TFT9341_FillRect_T(x1,  y1,  x1+31, y1+47, (uint32_t)OUT_DMA_2);
		
		
	automat = 0;
	}
//{
//	{5, 0}, 		// . 
//	{0, 0}, 		// / 
//	{27, 45}, 		// 0 
//	{16, 225}, 		// 1 
//	{28, 315}, 		// 2 
//	{24, 495}, 		// 3 
//	{28, 630}, 		// 4 
//	{25, 810}, 		// 5 
//	{27, 990}, 		// 6 
//	{28, 1170}, 		// 7 
//	{24, 1350}, 		// 8 
//	{26, 1485}, 		// 9 
//};


}
	

