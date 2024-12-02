/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "ucpd.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "spi_ili9341.h"
#include "Mod_bus.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

	extern uint16_t TFT9341_WIDTH;
	extern uint16_t TFT9341_HEIGHT;
	
	struct Display
{
    uint16_t volt_set_current;
		uint16_t current_set_current;
    uint16_t volt_set_izmerenn;	
		uint16_t current_set_izmerenn;	
		uint8_t  set_d_v[4];
		uint8_t  set_d_c[4];
		uint8_t  izmerenn_d_v[4];
		uint8_t  izmerenn_d_c[4];	
};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint16_t d_v[4] = {0} ,d_c[4] = {0},d_v_izm[4] = {0} ,d_c_izm[4] = {0};	
struct  Display Display_current;
Display_current.set_d_v[0] = 10;
Display_current.set_d_v[1] = 10;
Display_current.set_d_v[2] = 10;
Display_current.set_d_v[3] = 10;
Display_current.set_d_c[0] = 10;
Display_current.set_d_c[1] = 10;
Display_current.set_d_c[2] = 10;
Display_current.set_d_c[3] = 10;

Display_current.izmerenn_d_v[0] = 10;
Display_current.izmerenn_d_v[1] = 10;
Display_current.izmerenn_d_v[2] = 10;
Display_current.izmerenn_d_v[3] = 10;
Display_current.izmerenn_d_c[0] = 10;
Display_current.izmerenn_d_c[1] = 10;
Display_current.izmerenn_d_c[2] = 10;
Display_current.izmerenn_d_c[3] = 10;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  LL_SYSCFG_DisableDBATT(LL_SYSCFG_UCPD2_STROBE);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	NVIC->ICPR[0U] = 0;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_UCPD1_Init();
  MX_UCPD2_Init();
  MX_TIM2_Init();
  MX_TIM14_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
	LL_TIM_EnableCounter(TIM2);
	LL_SPI_Enable(SPI1);
	LL_SPI_Enable(SPI2);
	MX_DMA_User_Init();
	TFT9341_LED_ON();
	TFT9341_reset();
	TFT9341_ini(240, 320);
	TFT9341_FillScreen(TFT9341_BLACK);
	TFT9341_SetRotation(1);
	TFT9341_SetFont(&Font48);


	
	LL_GPIO_SetOutputPin(SPI2_NSS_GPIO_Port, SPI1_NSS_Pin );
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, 20);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, 19);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_2);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_3);
 
REGS[volt_set] = 123;
REGS[current_set] = 123;
//вывод точек
TFT9341_FillRect(90, 65, 95, 70, TFT9341_GREEN);
TFT9341_FillRect(234, 65, 239, 70, TFT9341_GREEN);
TFT9341_FillRect(90, 110, 95, 115, TFT9341_GREEN);
TFT9341_FillRect(234, 110, 239, 115, TFT9341_GREEN);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {

			LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_2);
			LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_3);
			LL_GPIO_ResetOutputPin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin );
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, 20);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, 19);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_2);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_3);
		
//		REGS[volt_set]++;
//		if(REGS[volt_set] == 4000)REGS[volt_set] = 0;
//		
//				REGS[current_set]++;
//		
//		if(REGS[current_set] == 4000)REGS[current_set] = 0;
//					REGS[current_set]++;
//		
//		if(REGS[volt_measured] == 4000)REGS[volt_measured] = 0;	
//			REGS[volt_measured]++;
//		
//			if(REGS[current_measured]== 4000)REGS[current_measured] = 0;	
//			REGS[current_measured]++;	
		
		
		REGS[volt_set] = TIM2->CNT;
	REGS[current_set] = (TIM2->CNT)>>1;
		
			REGS[volt_set] = TIM2->CNT;
	REGS[volt_measured] = ((TIM2->CNT)>>1) +10;	
		
		
		
		
		//вывод заданного напряжения
		if(Display_current.volt_set_current != REGS[volt_set]){
				uint16_t del = 0, volt_set_b =REGS[volt_set];
				
					d_v[0] = volt_set_b % 10;
					volt_set_b = volt_set_b/10;
					d_v[1] = volt_set_b % 10;
					volt_set_b = volt_set_b/10;
					d_v[2] = volt_set_b % 10;
						volt_set_b = volt_set_b/10;
					d_v[3] = volt_set_b;
					if(d_v[3] != Display_current.set_d_v[3])massiv_code(25,  20,  d_v[3]);//заполняем массив выводим цифру
					if(d_v[2] != Display_current.set_d_v[2])massiv_code(57,  20,  d_v[2]);//выводим цифру		
					if(d_v[1] != Display_current.set_d_v[1])massiv_code(100,  20,  d_v[1]);//выводим цифру						
					if(d_v[0] != Display_current.set_d_v[0])massiv_code(132,  20,  d_v[0]);//выводим цифру										
			
			Display_current.set_d_v[0] = d_v[0];
			Display_current.set_d_v[1] = d_v[1];		
			Display_current.set_d_v[2] = d_v[2];		
			Display_current.set_d_v[3] = d_v[3];			
			Display_current.current_set_current = REGS[volt_set];
		}
		
		//вывод заданного тока
		if(Display_current.current_set_current != REGS[current_set]){
				uint16_t del = 0, cur_set_b = REGS[current_set];
				
					d_c[0] = cur_set_b % 10;
					cur_set_b = cur_set_b/10;
					d_c[1] = cur_set_b % 10;
					cur_set_b = cur_set_b/10;
					d_c[2] = cur_set_b % 10;
					cur_set_b = cur_set_b/10;
					d_c[3] = cur_set_b;
					if(d_c[3] != Display_current.set_d_c[3])massiv_code(170,  20,  d_c[3]);//заполняем массив выводим цифру
					if(d_c[2] != Display_current.set_d_c[2])massiv_code(202 , 20,  d_c[2]);//выводим цифру		
					if(d_c[1] != Display_current.set_d_c[1])massiv_code(240,  20,  d_c[1]);//выводим цифру						
					if(d_c[0] != Display_current.set_d_c[0])massiv_code(272,  20,  d_c[0]);//выводим цифру										
			
			Display_current.set_d_c[0] = d_c[0];
			Display_current.set_d_c[1] = d_c[1];		
			Display_current.set_d_c[2] = d_c[2];		
			Display_current.set_d_c[3] = d_c[3];			
			Display_current.current_set_current = REGS[current_set];
		}		
		
		//вывод измеренного напряжения
		if(Display_current.volt_set_izmerenn != REGS[volt_measured]){
				uint16_t del = 0, volt_izm_b =REGS[volt_measured];
				
					d_v_izm[0] = volt_izm_b % 10;
					volt_izm_b = volt_izm_b/10;
					d_v_izm[1] = volt_izm_b % 10;
					volt_izm_b = volt_izm_b/10;
					d_v_izm[2] = volt_izm_b % 10;
					volt_izm_b = volt_izm_b/10;
					d_v_izm[3] = volt_izm_b;
					if(d_v_izm[3] != Display_current.izmerenn_d_v[3])massiv_code(25,  70,  d_v_izm[3]);//заполняем массив выводим цифру
					if(d_v_izm[2] != Display_current.izmerenn_d_v[2])massiv_code(57,  70,  d_v_izm[2]);//выводим цифру		
					if(d_v_izm[1] != Display_current.izmerenn_d_v[1])massiv_code(100, 70,  d_v_izm[1]);//выводим цифру						
					if(d_v_izm[0] != Display_current.izmerenn_d_v[0])massiv_code(132, 70,  d_v_izm[0]);//выводим цифру										
			
			Display_current.izmerenn_d_v[0] = d_v_izm[0];
			Display_current.izmerenn_d_v[1] = d_v_izm[1];		
			Display_current.izmerenn_d_v[2] = d_v_izm[2];		
			Display_current.izmerenn_d_v[3] = d_v_izm[3];			
			Display_current.volt_set_izmerenn = REGS[volt_measured];
		}		
		
		//вывод измеренного тока
		if(Display_current.current_set_izmerenn != REGS[current_measured]){
				uint16_t del = 0, cur_izm_b =REGS[current_measured];
					d_v_izm[0] = cur_izm_b % 10;
					cur_izm_b = cur_izm_b/10;
					d_v_izm[1] = cur_izm_b % 10;
					cur_izm_b = cur_izm_b/10;
					d_c_izm[2] = cur_izm_b % 10;
					cur_izm_b = cur_izm_b/10;
					d_c_izm[3] = cur_izm_b;
					if(d_v_izm[3] != Display_current.izmerenn_d_c[3])massiv_code(170,  70,  d_v_izm[3]);//заполняем массив выводим цифру
					if(d_v_izm[2] != Display_current.izmerenn_d_c[2])massiv_code(202,  70,  d_v_izm[2]);//выводим цифру		
					if(d_v_izm[1] != Display_current.izmerenn_d_c[1])massiv_code(240, 70,  d_v_izm[1]);//выводим цифру						
					if(d_v_izm[0] != Display_current.izmerenn_d_c[0])massiv_code(272, 70,  d_v_izm[0]);//выводим цифру										
			
			Display_current.izmerenn_d_c[0] = d_c_izm[0];
			Display_current.izmerenn_d_c[1] = d_c_izm[1];		
			Display_current.izmerenn_d_c[2] = d_c_izm[2];		
			Display_current.izmerenn_d_c[3] = d_c_izm[3];			
			Display_current.volt_set_izmerenn = REGS[current_measured];
		}		

//				TFT9341_FillRect_T(50, 50, 81, 94, TFT9341_RED);
//				LL_mDelay(50);
//				TFT9341_FillScreen(TFT9341_BLACK);
//				LL_mDelay(50);
//				TFT9341_FillRect_T(100, 50, 131, 94, TFT9341_RED);

	

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /* HSE configuration and activation */
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(64000000);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
