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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define CHECK_BIT(var, pos) ((var) & (1 << (pos)))
uint8_t ShiftRegisterValue = 0x1;
bool DirectionLeft = false;
bool ONCE_BLINK = true; 		//true인 경우 초당1번 점멸, false인 경우 초당 5번 점멸

void ShiftRegister(void)
{

	//오른쪽으로 점멸인데 가장 오른쪽 점멸된 경우
	if(ShiftRegisterValue == 0x1 && DirectionLeft == false)
	{
		//다시 왼쪽으로 점멸
		DirectionLeft = true;
	}
	//왼쪽으로 점멸인데 가장 왼쪽 점멸된 경우
	else if(ShiftRegisterValue == 0x4 && DirectionLeft == true)
	{
		//다시 오른쪽으로 점멸
		DirectionLeft = false;
	}

	if(DirectionLeft == true)
	{
		ShiftRegisterValue <<= 1;
	}
	else if(DirectionLeft == false)
	{
		ShiftRegisterValue >>= 1;
	}

}

void DisplayLED(int led_number)
{
	//shift register의 몇번째 bit를 확인할 것인지
	int check_bit = 0;
	//점멸할 LED의 PIN
	uint16_t GPIO_PIN_NUM;

	if(led_number == 0)
	{
		check_bit = 0;
		GPIO_PIN_NUM = GPIO_PIN_0;
	}
	else if(led_number == 1)
	{
		check_bit = 1;
		GPIO_PIN_NUM = GPIO_PIN_7;
	}
	else if(led_number == 2)
	{
		check_bit = 2;
		GPIO_PIN_NUM = GPIO_PIN_14;
	}


	// ShiftRegisterValue 비트 확인
	if (CHECK_BIT(ShiftRegisterValue, check_bit))
	{
	    //점멸 주기를 위한 Tick연산
		uint32_t LED_Toggle_Tick = HAL_GetTick();

		int number_of_blink = 0;    //현재 깜빡임 카운트
		int preiod_of_blink = 0;    //점멸 주기 세팅
		int limit_of_blink = 0;     //최대 점멸 횟수 - 초당 점멸 횟수 제어용

		if(ONCE_BLINK == true)
		{
			preiod_of_blink = 1000;
			limit_of_blink = 2;
		}
		else if(ONCE_BLINK == false)
		{
			preiod_of_blink = 100;
			limit_of_blink = 10;
		}

		while(true)
		{
			if(HAL_GetTick() - LED_Toggle_Tick >= preiod_of_blink)
			{
				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_NUM);
				LED_Toggle_Tick = HAL_GetTick();
				number_of_blink++;
			}

			if(number_of_blink >= limit_of_blink)
				break;
		}
	}
	else
	{
	    //점멸하지 않을 때 - 다른 LED 점멸 중일 때는 점멸하지 않음
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  bool pressed = false;    //버튼을 방금 눌렀는지 즉, 지금 방향을 바꿔야 하는지
  uint32_t debounce_Tick = HAL_GetTick();
  uint32_t LED_Tick = HAL_GetTick();
  while (1)
    {
      /* USER CODE END WHILE */

	  //눌리지 않은 경우
	  if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_RESET)
	  {
		  //디바운싱
		  //HAL_Delay(20);
		  if(HAL_GetTick() - debounce_Tick >= 20)
		  {
			  debounce_Tick = HAL_GetTick();
			  if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_RESET)
			  {
				  //안눌려있을 때 마다 방향을 바꾸는 것이 아닌 떼는 순간 한번만 방향을 바꿈
				  if(pressed == true)
				  {
					  pressed = false;
					  ONCE_BLINK = !ONCE_BLINK;
				  }
			  }
		  }

	  }
	  else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_SET)    //눌린 경우
	  {
		  //디바운싱
		  //HAL_Delay(20);
		  if(HAL_GetTick() - debounce_Tick >= 20)
		  {
			  debounce_Tick = HAL_GetTick();
			  if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_SET)
			  {
				  //눌려있을 때 마다 방향을 바꾸는 것이 아닌 누른 한번만 방향을 바꿈
				  if(pressed == false)
				  {
					  pressed = true;
					  ONCE_BLINK = !ONCE_BLINK;
				  }
			  }
		  }

	  }
	  //HAL_Delay(1000);
	  if(HAL_GetTick() - LED_Tick >= 1000)
	  {
		  LED_Tick = HAL_GetTick();
		  ShiftRegister();
		  DisplayLED(0);
		  DisplayLED(1);
		  DisplayLED(2);
	  }

      /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_14|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB14 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_14|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
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
