/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "motor.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum{
	SPEED = 0xA1,
	DISTANCE =0xA2,
}msg_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RIGHT_MOTOR_TIMER 				&htim1
#define LEFT_MOTOR_TIMER 				&htim2
#define RIGHT_MOTOR_ENCODER_TIMER		&htim3
#define LEFT_MOTOR_ENCODER_TIMER		&htim4

#define RIGHT_MOTOR_KP					0.3
#define RIGHT_MOTOR_KI					0.05
#define RIGHT_MOTOR_KD					0.01
#define LEFT_MOTOR_KP					0.3
#define LEFT_MOTOR_KI					0.05
#define LEFT_MOTOR_KD					0.01
#define ANTI_WINDUP			        	100

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

serwo_str right_motor;
serwo_str left_motor;

uint32_t ADC2_measurement[3];
uint32_t ADC3_measurement[2];
float v_cell_1, v_cell_2, v_cell_3;
uint8_t UART_buffer[5];
int16_t speed_right, speed_left;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void parse(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_TIM15_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(RIGHT_MOTOR_TIMER);
  HAL_TIM_Base_Start(LEFT_MOTOR_TIMER);

  HAL_TIM_PWM_Start(LEFT_MOTOR_TIMER, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(LEFT_MOTOR_TIMER, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(LEFT_MOTOR_TIMER, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(LEFT_MOTOR_TIMER, TIM_CHANNEL_4);

  HAL_TIM_PWM_Start(RIGHT_MOTOR_TIMER, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(RIGHT_MOTOR_TIMER, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(RIGHT_MOTOR_TIMER, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(RIGHT_MOTOR_TIMER, TIM_CHANNEL_4);

  motor_str_init(&right_motor, RIGHT_MOTOR_TIMER, RIGHT_MOTOR_ENCODER_TIMER, 1);
  motor_str_init(&left_motor, LEFT_MOTOR_TIMER, LEFT_MOTOR_ENCODER_TIMER, -1);
  pid_init(&(right_motor.pid_controller), RIGHT_MOTOR_KP, RIGHT_MOTOR_KI, RIGHT_MOTOR_KD, ANTI_WINDUP);
  pid_init(&(left_motor.pid_controller), LEFT_MOTOR_KP, LEFT_MOTOR_KI, LEFT_MOTOR_KD, ANTI_WINDUP);
  filter_init(&(right_motor.filter));
  filter_init(&(left_motor.filter));

  HAL_TIM_Encoder_Start(RIGHT_MOTOR_ENCODER_TIMER, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(LEFT_MOTOR_ENCODER_TIMER, TIM_CHANNEL_ALL);

  //HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  //HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);

  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start(&htim15);

  HAL_ADC_Start_DMA(&hadc2, ADC2_measurement, 3);
  HAL_ADC_Start_DMA(&hadc3, ADC3_measurement, 2);

  HAL_UART_Receive_IT(&huart3, UART_buffer, 5);

  //left_motor_forward(98);
  //right_motor_forward(98);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM6)
	{
		v_cell_1 = 3.3f * ADC2_measurement[2] / 4096.0f;
		v_cell_2 = 3.3f * ADC3_measurement[0] / 4096.0f;
		v_cell_3 = 3.3f * ADC3_measurement[1] / 4096.0f;
	}

	if(htim -> Instance == TIM7)
	{
		motor_calculate_speed(&right_motor);
		motor_calculate_speed(&left_motor);
		motor_run_pid(&left_motor);
		motor_run_pid(&right_motor);
	}

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc -> Instance == ADC2)
	{
		filter_add_data(&(right_motor.filter), ADC2_measurement[1]);
		filter_add_data(&(left_motor.filter), ADC2_measurement[0]);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	parse();
	HAL_UART_Receive_IT(&huart3, UART_buffer, 5);
}

void parse(void)
{
	switch(UART_buffer[0])
	{
		case SPEED:
			speed_right = (int16_t)(UART_buffer[1]<<8 | UART_buffer[2]);
			speed_left = (int16_t)(UART_buffer[3]<<8 | UART_buffer[4]);
			motor_set_speed(&right_motor, speed_right);
			motor_set_speed(&left_motor, speed_left);
			break;
		case DISTANCE:
			break;
		default:
			break;
	}
}

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
