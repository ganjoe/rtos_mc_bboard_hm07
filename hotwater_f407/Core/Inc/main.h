/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "math.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
//const uint8_t* readbyte;
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
#define shunt_u_Pin GPIO_PIN_0
#define shunt_u_GPIO_Port GPIOA
#define shunt_v_Pin GPIO_PIN_1
#define shunt_v_GPIO_Port GPIOA
#define emk_u_Pin GPIO_PIN_2
#define emk_u_GPIO_Port GPIOA
#define emk_v_Pin GPIO_PIN_3
#define emk_v_GPIO_Port GPIOA
#define pwm_uh_Pin GPIO_PIN_6
#define pwm_uh_GPIO_Port GPIOA
#define pwm_vh_Pin GPIO_PIN_7
#define pwm_vh_GPIO_Port GPIOA
#define enable_u_Pin GPIO_PIN_10
#define enable_u_GPIO_Port GPIOE
#define enable_v_Pin GPIO_PIN_12
#define enable_v_GPIO_Port GPIOE
#define test_Pin GPIO_PIN_13
#define test_GPIO_Port GPIOB
#define drv_en_Pin GPIO_PIN_6
#define drv_en_GPIO_Port GPIOB
#define drv_cs_Pin GPIO_PIN_7
#define drv_cs_GPIO_Port GPIOB
#define drv_fault_Pin GPIO_PIN_8
#define drv_fault_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
