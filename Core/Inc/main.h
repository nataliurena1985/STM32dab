/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


extern uint32_t USBRX;
extern uint8_t  BufferRX[];
extern uint32_t FV ;
extern uint32_t BT;
extern uint32_t Disparo;
extern uint32_t NivelDisparoL;
extern uint32_t NivelDisparoH;
extern uint32_t Flanco;
extern uint32_t Demora;
extern uint32_t NroTrama;
extern uint32_t AcopAC;

extern uint8_t BufferRX[100];

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;
extern DMA_HandleTypeDef hdma_adc4;

//extern CAN_HandleTypeDef hcan;

extern DAC_HandleTypeDef hdac1;
extern DMA_HandleTypeDef hdma_dac1_ch1;

extern SPI_HandleTypeDef hspi3;
extern DMA_HandleTypeDef hdma_spi3_tx;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart2;

extern int ERROR_SD;

extern const float  SPWM[250];
extern uint32_t SPWM1[250];
extern uint32_t SPWM2[250];

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPWM_SemicicloPositivo_Pin GPIO_PIN_0
#define SPWM_SemicicloPositivo_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define SD_CS_Pin GPIO_PIN_3
#define SD_CS_GPIO_Port GPIOC
#define SD_SPI_HANDLE hspi3
#define SD_CS_OFF 		HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin,GPIO_PIN_SET)
#define SD_CS_ON 		HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin,GPIO_PIN_RESET)



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
