#include "Mod_bus.h"
#include "stm32g0xx.h"
#include "dma.h"

uint16_t REGS [20] = {0};
uint16_t MODBUS_crc16(volatile unsigned char *buffer,uint16_t n);







void Send_Messange(void){
	uint16_t crc16;
	
//	DMA_USART1_TX [0] = 10;
//	DMA_USART1_TX [1] = (volt_set >>8) &0xFF;
//	DMA_USART1_TX [2] = volt_set &0xFF;
//	DMA_USART1_TX [3] = (current_set >>8) &0xFF;
//	DMA_USART1_TX [4] = current_set &0xFF;
//	DMA_USART1_TX [5] = On_Off&0xFF;;
//	DMA_USART1_TX [6] = 0xFF;
//	DMA_USART1_TX [7] = 0xFF;
//	
//	crc16 = MODBUS_crc16(DMA_USART1_TX,8);
//	
//	DMA_USART1_TX [8] = (crc16 >>8) &0xFF;
//	DMA_USART1_TX [9] = crc16 &0xFF;
	

}

void Read_Messange(void){

}

uint16_t MODBUS_crc16(volatile unsigned char *buffer,uint16_t n){
	unsigned int y = 65535, z = 0,i,j;
		for(i =0;i<n-2;i++){
			z = *(buffer+i) ^ y;
			for(j = 0; j<8; j++){
				if(z & 1){
					z = (z>>1);
					z = z ^ 0xA001;}
				else z = (z>>1);}
				y=z;}
	return z;
}

















