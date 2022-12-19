/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN Variables */
static uint8_t LED_Blink_Counter = 0;
const static uint8_t LED_Blink_Counter_LIMIT = 0b00000011;
const static uint8_t LED_Blink_Counter_MASK0 = 0b00000001;
const static uint8_t LED_Blink_Counter_MASK1 = 0b00000010;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern void LED_Blink_Task(void *argument);
/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
 * @brief RTOS Thread for LED Blink
 *
 * @param None
 * @retval None
 */
extern void LED_Blink_Task(void *argument)
{
  for (;;)
  {
    LED_Blink_Counter++;
    if (LED_Blink_Counter > LED_Blink_Counter_LIMIT)
    {
      LED_Blink_Counter = 0;
    }
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, LED_Blink_Counter & LED_Blink_Counter_MASK0);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, LED_Blink_Counter & LED_Blink_Counter_MASK1);
    osDelay(500);
  }
  osThreadTerminate();
}

/* USER CODE END Application */
