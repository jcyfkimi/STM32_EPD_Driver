/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define EPD_MOSI_0	HAL_GPIO_WritePin(EPD_SDO_GPIO_Port, EPD_SDO_Pin, GPIO_PIN_RESET)
#define EPD_MOSI_1	HAL_GPIO_WritePin(EPD_SDO_GPIO_Port, EPD_SDO_Pin, GPIO_PIN_SET)

#define EPD_CLK_0	HAL_GPIO_WritePin(EPD_SCK_GPIO_Port, EPD_SCK_Pin, GPIO_PIN_RESET)
#define EPD_CLK_1	HAL_GPIO_WritePin(EPD_SCK_GPIO_Port, EPD_SCK_Pin, GPIO_PIN_SET)

#define EPD_CS_0	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET)
#define EPD_CS_1	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET)

#define EPD_DC_0	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET)
#define EPD_DC_1	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_SET)

#define EPD_RST_0	HAL_GPIO_WritePin(EPD_RES_GPIO_Port, EPD_RES_Pin, GPIO_PIN_RESET)
#define EPD_RST_1	HAL_GPIO_WritePin(EPD_RES_GPIO_Port, EPD_RES_Pin, GPIO_PIN_SET)

#define isEPD_BUSY 	HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin)

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

