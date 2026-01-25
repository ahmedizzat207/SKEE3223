/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "ssd1306.h"
// #include "ssd1306_fonts.h"
#include <string.h>
// #include "stm32l4xx_hal.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int digit_tens;
int digit_ones;
int digit_tenths;

// Debounce variables for EXTI
volatile int exti_debounce_counter = 0;
volatile int exti_debounce_flag = 0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// void Update_Number(int command);
#define DECREMENT 0xFFFFFFFF
#define INCREMENT 0x00000001
#define EXTI_DEBOUNCE_DELAY 300

void GPIO_Init(void);
void DisplayNumber(void);
void SoftwareDelay(volatile int);
int ReadButtons(void);
extern void Update_Number(int val_r0, int *ptens, int *pones, int *ptenths);

const uint32_t segTable[10] = {
		0b0111111,   //0: A,B,C,D,E,F
		0b0000110,   //1: B,C
		0b1011011,   //2: A,B,G,E,D
		0b1001111,   //3: A,B,G,C,D
		0b1100110,   //4: F,G,B,C
		0b1101101,   //5: A,F,G,C,D
		0b1111101,   //6: A,F,G,E,D,C
		0b0000111,   //7: A,B,C
		0b1111111,   //8: A,B,C,D,E,F,G
		0b1101111    //9: A,B,C,D,F,G
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// void Update_Number(void)
// {
// 	if(!(GPIOA->IDR & SW1_Pin)) {  // increment 0.1
// 		digit_tenths++;
// 		if(digit_tenths >= 10) {
// 			digit_tenths = 0;
// 			digit_ones++;
// 			if(digit_ones >= 10) {
// 				digit_ones = 0;
// 				digit_tens++;
// 				if(digit_tens >= 10) digit_tens = 0;
// 			}
// 		}
//   }
//   SoftwareDelay(5000);
// 	// } else if(command == -1) {  // decrement 0.1
// 	// 	digit_tenths--;
// 	// 	if(digit_tenths < 0) {
// 	// 		digit_tenths = 9;
// 	// 		digit_ones--;
// 	// 		if(digit_ones < 0) {
// 	// 			digit_ones = 9;
// 	// 			digit_tens--;
// 	// 			if(digit_tens < 0) digit_tens = 9;
// 	// 		}
// 	// 	}
// 	// }
// }




// ------------------- DISPLAY FUNCTION -------------------
void SendSegments(uint8_t digit, int dot)
{
	uint32_t out = 0;

	if(digit & 0b0000001) out |= 0x01 << 2;
	if(digit & 0b0000010) out |= 0x01 << 1;
	if(digit & 0b0000100) out |= 0x01 << 10;
	if(digit & 0b0001000) out |= 0x01 << 9;
	if(digit & 0b0010000) out |= 0x01 << 8;
	if(digit & 0b0100000) out |= 0x01 << 3;
	if(digit & 0b1000000) out |= 0x01 << 4;
	if(dot)          out |= CH_DP_Pin;

	// Clear previous segments
	GPIOA->ODR &= ~(CH_A_Pin|CH_B_Pin|CH_C_Pin|CH_D_Pin|CH_E_Pin|CH_F_Pin|CH_G_Pin|CH_DP_Pin);
	GPIOA->ODR |= out;
}


void DisplayNumber(void)
{
	// ----- Tens -----
	SEL_C_GPIO_Port->ODR |=  SEL_C_Pin; // disable tenths
	SEL_B_GPIO_Port->ODR |=  SEL_B_Pin;  // disable ones
	SendSegments(segTable[digit_tens], 0);
	SEL_A_GPIO_Port->ODR &= ~SEL_A_Pin;  // enable tens
	SoftwareDelay(3000);
	SEL_A_GPIO_Port->ODR |= SEL_A_Pin;

	// ----- Ones -----
	SendSegments(segTable[digit_ones], 1); // decimal point
	SEL_B_GPIO_Port->ODR &= ~SEL_B_Pin;  // enable ones
	SoftwareDelay(3000);
	SEL_B_GPIO_Port->ODR |= SEL_B_Pin;

	// ----- Tenths -----
	SendSegments(segTable[digit_tenths], 0);
	SEL_C_GPIO_Port->ODR &= ~SEL_C_Pin; // enable tenths
	SoftwareDelay(3000);
	SEL_C_GPIO_Port->ODR |= SEL_C_Pin;
}

// ------------------- BUTTON READ -------------------
int ReadButtons(void)
{
	// Right PB (PA12) active LOW
	if(!(GPIOA->IDR & SW1_Pin)) return 0xFFFFFFFF;

	// Left PB (PB1) active HIGH
	// if(GPIOB->IDR & (1<<1)) return 0x00000001;

	return 0;
}
// ------------------- SOFTWARE DELAY -------------------
void SoftwareDelay(volatile int n)
{
	for(; n>0; n--);
}

void EXTI0_IRQHandler(void)
{
  // Debounce: Only process if debounce period has elapsed
  if(exti_debounce_counter == 0 && !exti_debounce_flag) {
    // Button press accepted after debounce
    Update_Number(DECREMENT, &digit_tens, &digit_ones, &digit_tenths);
    exti_debounce_flag = 1;           // Mark as debouncing
    exti_debounce_counter = EXTI_DEBOUNCE_DELAY;  // Set debounce timer
  }
  EXTI->PR |= 0x0001;  // Clear EXTI pending flag
}

void EXTI1_IRQHandler(void)
{
  // Debounce: Only process if debounce period has elapsed
  if(exti_debounce_counter == 0 && !exti_debounce_flag) {
    // Button press accepted after debounce
    Update_Number(INCREMENT, &digit_tens, &digit_ones, &digit_tenths);
    exti_debounce_flag = 1;           // Mark as debouncing
    exti_debounce_counter = EXTI_DEBOUNCE_DELAY;  // Set debounce timer
  }
  EXTI->PR |= 0x0002;  // Clear EXTI pending flag
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	digit_tens = 4;
	digit_ones = 7;
	digit_tenths = 6;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
//	ssd1306_Init();
//	ssd1306_Fill(Black);
//	ssd1306_SetCursor(0,0);
//	ssd1306_WriteString("SKEE3223 APPBOARD\n",Font_7x10,White);
//	ssd1306_SetCursor(0,16); ssd1306_WriteString("Group 23",Font_7x10,White);
//	ssd1306_SetCursor(0,25); ssd1306_WriteString("Aaya",Font_7x10,White);
//	ssd1306_SetCursor(0,37); ssd1306_WriteString("Eilaf",Font_7x10,White);
//	ssd1306_SetCursor(0,49); ssd1306_WriteString("Leena",Font_7x10,White);
//	ssd1306_UpdateScreen();

  GPIOB->MODER &= ~(0x03 << 2);
  GPIOB->PUPDR |= (0x01 << 2);
  RCC->APB2ENR = 0x4000;
  SYSCFG->EXTICR[0] |= (0x0001 << 4);
  EXTI->IMR |= 0X03;
  EXTI->FTSR |= 0x03;
  NVIC_EnableIRQ(EXTI0_IRQn);
  NVIC_EnableIRQ(EXTI1_IRQn);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		// Debounce counter decrement
		if(exti_debounce_counter > 0) {
			exti_debounce_counter--;
		} else if(exti_debounce_counter == 0 && exti_debounce_flag) {
			exti_debounce_flag = 0;  // Debounce period complete, ready for next press
		}
		DisplayNumber(); // multiplex display
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CH_B_Pin|CH_A_Pin|CH_F_Pin|CH_G_Pin
                          |CH_E_Pin|CH_D_Pin|CH_C_Pin|CH_DP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEL_A_Pin|SEL_B_Pin|SEL_C_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SW1_Pin */
  GPIO_InitStruct.Pin = SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CH_B_Pin CH_A_Pin CH_F_Pin CH_G_Pin
                           CH_E_Pin CH_D_Pin CH_C_Pin CH_DP_Pin */
  GPIO_InitStruct.Pin = CH_B_Pin|CH_A_Pin|CH_F_Pin|CH_G_Pin
                          |CH_E_Pin|CH_D_Pin|CH_C_Pin|CH_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SEL_A_Pin SEL_B_Pin SEL_C_Pin */
  GPIO_InitStruct.Pin = SEL_A_Pin|SEL_B_Pin|SEL_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
