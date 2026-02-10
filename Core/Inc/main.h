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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {
  PAGE_MAIN = 0,
  PAGE_PARAMS,
  PAGE_PARAMS_READ,
  PAGE_PARAMS_EDIT,
  PAGE_PARAMS_EDIT_RESULT,
  PAGE_IMPORT,
  PAGE_IMPORT_DIALOG,
  PAGE_IMPORT_STATUS,
  PAGE_RSTENC_DIALOG,
  PAGE_RSTENC_STATUS,
} PageStatus;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern volatile uint8_t btn_state;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_CONFIRM_Pin GPIO_PIN_5
#define BTN_CONFIRM_GPIO_Port GPIOA
#define BTN_CONFIRM_EXTI_IRQn EXTI9_5_IRQn
#define BTN_SELECT_Pin GPIO_PIN_6
#define BTN_SELECT_GPIO_Port GPIOA
#define BTN_SELECT_EXTI_IRQn EXTI9_5_IRQn
#define BTN_DOWN_Pin GPIO_PIN_7
#define BTN_DOWN_GPIO_Port GPIOA
#define BTN_DOWN_EXTI_IRQn EXTI9_5_IRQn
#define BTN_UP_Pin GPIO_PIN_0
#define BTN_UP_GPIO_Port GPIOB
#define BTN_UP_EXTI_IRQn EXTI0_IRQn
#define BTN_BACK_Pin GPIO_PIN_1
#define BTN_BACK_GPIO_Port GPIOB
#define BTN_BACK_EXTI_IRQn EXTI1_IRQn

/* USER CODE BEGIN Private defines */
// 最低位:0 解锁 1锁定, 其他位:0 未按下 1按下, 从右到左依次为 BACK UP DOWN SELECT CONFIRM
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
