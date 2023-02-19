/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#define SPD_UP_Pin GPIO_PIN_6
#define SPD_UP_GPIO_Port GPIOC
#define SPD_UP_EXTI_IRQn EXTI9_5_IRQn
#define SPD_DOWN_Pin GPIO_PIN_8
#define SPD_DOWN_GPIO_Port GPIOC
#define SPD_DOWN_EXTI_IRQn EXTI9_5_IRQn
#define SCH_PREV_Pin GPIO_PIN_9
#define SCH_PREV_GPIO_Port GPIOC
#define SCH_PREV_EXTI_IRQn EXTI9_5_IRQn
#define OPR_TOGGLE_Pin GPIO_PIN_15
#define OPR_TOGGLE_GPIO_Port GPIOA
#define OPR_TOGGLE_EXTI_IRQn EXTI15_10_IRQn
#define SCH_NEXT_Pin GPIO_PIN_11
#define SCH_NEXT_GPIO_Port GPIOC
#define SCH_NEXT_EXTI_IRQn EXTI15_10_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
