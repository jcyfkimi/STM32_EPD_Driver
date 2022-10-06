/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#define FLASH_NSS_Pin GPIO_PIN_4
#define FLASH_NSS_GPIO_Port GPIOA
#define FLASH_SCK_Pin GPIO_PIN_5
#define FLASH_SCK_GPIO_Port GPIOA
#define FLASH_MISO_Pin GPIO_PIN_6
#define FLASH_MISO_GPIO_Port GPIOA
#define FLASH_MOSI_Pin GPIO_PIN_7
#define FLASH_MOSI_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_12
#define LED4_GPIO_Port GPIOE
#define EPD_BUSY_Pin GPIO_PIN_13
#define EPD_BUSY_GPIO_Port GPIOE
#define EPD_RES_Pin GPIO_PIN_14
#define EPD_RES_GPIO_Port GPIOE
#define EPD_DC_Pin GPIO_PIN_15
#define EPD_DC_GPIO_Port GPIOE
#define EPD_CS_Pin GPIO_PIN_8
#define EPD_CS_GPIO_Port GPIOD
#define EPD_SCK_Pin GPIO_PIN_9
#define EPD_SCK_GPIO_Port GPIOD
#define EPD_SDO_Pin GPIO_PIN_10
#define EPD_SDO_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
