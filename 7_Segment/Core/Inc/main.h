/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW1_Pin GPIO_PIN_0
#define SW1_GPIO_Port GPIOA
#define CH_B_Pin GPIO_PIN_1
#define CH_B_GPIO_Port GPIOA
#define CH_A_Pin GPIO_PIN_2
#define CH_A_GPIO_Port GPIOA
#define CH_F_Pin GPIO_PIN_3
#define CH_F_GPIO_Port GPIOA
#define CH_G_Pin GPIO_PIN_4
#define CH_G_GPIO_Port GPIOA
#define CH_E_Pin GPIO_PIN_8
#define CH_E_GPIO_Port GPIOA
#define CH_D_Pin GPIO_PIN_9
#define CH_D_GPIO_Port GPIOA
#define CH_C_Pin GPIO_PIN_10
#define CH_C_GPIO_Port GPIOA
#define CH_DP_Pin GPIO_PIN_11
#define CH_DP_GPIO_Port GPIOA
#define SEL_A_Pin GPIO_PIN_5
#define SEL_A_GPIO_Port GPIOB
#define SEL_B_Pin GPIO_PIN_6
#define SEL_B_GPIO_Port GPIOB
#define SEL_C_Pin GPIO_PIN_7
#define SEL_C_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
