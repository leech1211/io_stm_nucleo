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
#include <stdbool.h>
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
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define SERVO_MIN_US 500U    // calibrate: pulse for 0 deg
#define SERVO_MAX_US 2500U   // calibrate: pulse for 180 deg
#define ON GPIO_PIN_SET
#define OFF GPIO_PIN_RESET


volatile uint32_t pressTime = 0;
volatile uint32_t releaseTime = 0;
volatile static uint32_t last_edge = 0;
volatile bool isPressed = false;
volatile bool BtnMode = true;
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	uint32_t now = HAL_GetTick();
	if(pin == GPIO_PIN_13)
	{
		if(now - last_edge < 30)
		{
			return;
		}
		last_edge = now;

		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_SET)
		{
			//버튼이 눌림
		    pressTime = HAL_GetTick();
		    isPressed = true;
		}
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_RESET)
		{
			releaseTime = HAL_GetTick();
			isPressed = false;
			BtnMode = !BtnMode;
		}
	}
}


int get_now_angle(GPIO_PinState A5, GPIO_PinState A6, GPIO_PinState A7, GPIO_PinState C8)
{
	if(A5 == ON && A6 == OFF && A7 == OFF && C8 == OFF)
	{
		return 1;
	}
	else if(A5 == ON && A6 == ON && A7 == OFF && C8 == OFF)
	{
		return 2;
	}
	else if(A5 == OFF && A6 == ON && A7 == OFF && C8 == OFF)
	{
		return 3;
	}
	else if(A5 == OFF && A6 == ON && A7 == ON && C8 == OFF)
	{
		return 4;
	}
	else if(A5 == OFF && A6 == OFF && A7 == ON && C8 == OFF)
	{
		return 5;
	}
	else if(A5 == OFF && A6 == OFF && A7 == ON && C8 == ON)
	{
		return 6;
	}
	else if(A5 == OFF && A6 == OFF && A7 == OFF && C8 == ON)
	{
		return 7;
	}
	else if(A5 == ON && A6 == OFF && A7 == OFF && C8 == ON)
	{
		return 8;
	}
}
void Rotate_clock_direction()
{
	GPIO_PinState A5 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);
	GPIO_PinState A6 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6);
	GPIO_PinState A7 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7);
	GPIO_PinState C8 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
	int now_angle = get_now_angle(A5,A6,A7,C8);

	switch(now_angle)
	{
	case 1:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 2:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 3:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,ON);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 4:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,ON);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 5:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,ON);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,ON);
		return;
	case 6:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,ON);
		return;
	case 7:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,ON);
		return;
	case 8:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;

	}




}

void Rotate_reverse_clock_direction()
{
	GPIO_PinState A5 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);
	GPIO_PinState A6 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6);
	GPIO_PinState A7 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7);
	GPIO_PinState C8 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
	int now_angle = get_now_angle(A5,A6,A7,C8);

	switch(now_angle)
	{
	case 1:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,ON);
		return;
	case 2:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 3:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 4:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 5:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,ON);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,ON);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 6:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,ON);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,OFF);
		return;
	case 7:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,ON);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,ON);
		return;
	case 8:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,ON);
		return;

	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
  uint32_t now_time = 0;
  uint32_t print_tick = 0;
  uint32_t us = 1500;
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,OFF);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,OFF);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,OFF);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,ON);
  while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  if(HAL_GetTick() - print_tick > 1000)
//	  {
//		  print_tick = HAL_GetTick();
//		  //printf("isPressed = %d",isPressed);
//		  //printf("double_press = %d \n",double_press);
//		  printf("us : %lu\n",us);
//	  }

	  if(isPressed == true)
	  {
		  if(BtnMode == true)
		  {
			  Rotate_clock_direction();
		  }
		  else
		  {
			  Rotate_reverse_clock_direction();
		  }
		  HAL_Delay(1);
	  }





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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM34;
  PeriphClkInit.Tim34ClockSelection = RCC_TIM34CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 71;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 19999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
