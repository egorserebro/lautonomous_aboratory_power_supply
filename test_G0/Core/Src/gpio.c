/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);

  /**/
  LL_GPIO_ResetOutputPin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(RST_LCD_GPIO_Port, RST_LCD_Pin);

  /**/
  LL_GPIO_ResetOutputPin(RS_LCD_GPIO_Port, RS_LCD_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LCD_LED_GPIO_Port, LCD_LED_Pin);

  /**/
  GPIO_InitStruct.Pin = SPI1_NSS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SPI1_NSS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SPI2_NSS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SPI2_NSS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = RST_LCD_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(RST_LCD_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = RS_LCD_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(RS_LCD_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LCD_LED_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LCD_LED_GPIO_Port, &GPIO_InitStruct);

  /**/
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTB, LL_EXTI_CONFIG_LINE10);

  /**/
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTB, LL_EXTI_CONFIG_LINE8);

  /**/
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTB, LL_EXTI_CONFIG_LINE9);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_10;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_8;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_9;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(BUTTON1_GPIO_Port, BUTTON1_Pin, LL_GPIO_PULL_UP);

  /**/
  LL_GPIO_SetPinPull(BUTTON2_GPIO_Port, BUTTON2_Pin, LL_GPIO_PULL_UP);

  /**/
  LL_GPIO_SetPinPull(BUTTON3_GPIO_Port, BUTTON3_Pin, LL_GPIO_PULL_UP);

  /**/
  LL_GPIO_SetPinMode(BUTTON1_GPIO_Port, BUTTON1_Pin, LL_GPIO_MODE_INPUT);

  /**/
  LL_GPIO_SetPinMode(BUTTON2_GPIO_Port, BUTTON2_Pin, LL_GPIO_MODE_INPUT);

  /**/
  LL_GPIO_SetPinMode(BUTTON3_GPIO_Port, BUTTON3_Pin, LL_GPIO_MODE_INPUT);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI4_15_IRQn, 1);
  NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
