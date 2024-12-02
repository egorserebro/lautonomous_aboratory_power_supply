/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "dma.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */
	uint16_t OUT_DMA_1[1536] = {0};
	uint16_t OUT_DMA_2[1536] = {0};	
	
	uint8_t DMA_SPI2_TX[100] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	uint8_t DMA_SPI2_RX[100] = {0};
/* USER CODE END 1 */

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/* USER CODE BEGIN 2 */
	
void MX_DMA_User_Init(void){
	//SPI 1 TX
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_ClearFlag_TC1(DMA1);
	LL_SPI_EnableDMAReq_TX(SPI1);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)OUT_DMA_1, LL_SPI_DMA_GetRegAddr(SPI1), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1));
	
	//SPI 2 TX
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_ClearFlag_TC2(DMA1);
	LL_SPI_EnableDMAReq_TX(SPI2);
	//LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2, (uint32_t)DMA_SPI2_TX, LL_SPI_DMA_GetRegAddr(SPI2), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2));
	
	//SPI 2 RX
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
  LL_DMA_ClearFlag_TC3(DMA1);
	LL_SPI_EnableDMAReq_RX(SPI2);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_3);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_3, LL_SPI_DMA_GetRegAddr(SPI2),(uint32_t)DMA_SPI2_RX,  LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3));
	
	
	
	
	
//	//USART1 TX
//	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
//  LL_DMA_ClearFlag_TC3(DMA1);
//	LL_USART_EnableDMAReq_TX(USART1);
//	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
//	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_3, (uint32_t)DMA_USART1_TX, LL_USART_DMA_GetRegAddr(USART1,LL_DMA_DIRECTION_MEMORY_TO_PERIPH), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//	
//	//USART1 RX
//	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
//  LL_DMA_ClearFlag_TC2(DMA1);
//	LL_USART_EnableDMAReq_TX(USART1);
//	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);

//  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2,  LL_USART_DMA_GetRegAddr(USART1,LL_DMA_DIRECTION_PERIPH_TO_MEMORY),(uint32_t) DMA_USART1_RX, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	
	
	
}
/* USER CODE END 2 */

