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
#include "usart.h"
#include "gpio.h"

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
#define RXBUFFER_SIZE 100 // 接收数据缓存区大小 (单位为字节)

uint8_t tx_data[] = "Hello, World!\r\n";

uint8_t rx_buffer[RXBUFFER_SIZE]; // 接收数据缓冲区

/**
 * @brief  串口空闲中断回调函数
 * @param  huart: 串口句柄指针
 * @param  Size: 接收到的数据长度
 * @retval None
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART1) // 判断是否为USART1的回调
  {
    // 将收到的数据通过USART1发送回去
    // RS485 发送前需要拉高使能引脚 (DE/RE)
    HAL_GPIO_WritePin(MAX3485_EN_GPIO_Port, MAX3485_EN_Pin, GPIO_PIN_SET); 
    
    HAL_UART_Transmit(&huart1, rx_buffer, Size, HAL_MAX_DELAY); 
    
    // 发送完成后需要拉低使能引脚，恢复为接收模式
    HAL_GPIO_WritePin(MAX3485_EN_GPIO_Port, MAX3485_EN_Pin, GPIO_PIN_RESET); 

    // 重新开启空闲中断接收
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, rx_buffer, RXBUFFER_SIZE); 
  }
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // 开启USART1的空闲中断接收
  HAL_UARTEx_ReceiveToIdle_IT(&huart1, rx_buffer, RXBUFFER_SIZE); 

  // 延迟一会并清除状态标志，防止复位瞬间的电平抖动导致串口助手显示乱码 (如 '?')
  HAL_Delay(100);

  /* 
     __HAL_UART_CLEAR_FLAG 作用：清除串口的状态标志位。
     UART_FLAG_TC (Transmission Complete) 是发送完成标志。
     在单片机刚上电或复位时，硬件寄存器的初始状态或引脚电平抖动可能触发一个虚假的“发送完成”信号，
     导致串口助手将其误识别为一个杂散字符（即你看到的乱码 '?'）。
     在正式发送数据前手动清除该标志，可以确保发送缓冲区是干净的，从而消除这个乱码。
  */
  __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);

  // 初始发送Hello World
  // RS485 发送前需要拉高使能引脚
  HAL_GPIO_WritePin(MAX3485_EN_GPIO_Port, MAX3485_EN_Pin, GPIO_PIN_SET);
  HAL_UART_Transmit(&huart1, tx_data, sizeof(tx_data) - 1, HAL_MAX_DELAY); 
  // 发送完成后拉低，进入接收模式
  HAL_GPIO_WritePin(MAX3485_EN_GPIO_Port, MAX3485_EN_Pin, GPIO_PIN_RESET);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
