/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   此文件提供了所有使用的 GPIO 引脚的配置代码。
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) 2026 STMicroelectronics。
  * 保留所有权利。
  *
  * 本软件根据可在本软件组件根目录中的 LICENSE 文件中找到的条款进行许可。
  * 如果没有 LICENSE 文件，则按“原样”提供本软件。
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* 配置 GPIO                                                                  */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** 配置引脚为
        * 模拟输入
        * 输入
        * 输出
        * 事件输出
        * 外部中断
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO 端口时钟使能 */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* 配置 GPIO 引脚输出电平 */
  HAL_GPIO_WritePin(MAX3485_EN_GPIO_Port, MAX3485_EN_Pin, GPIO_PIN_RESET);

  /* 配置 GPIO 引脚 : MAX3485_EN_Pin */
  GPIO_InitStruct.Pin = MAX3485_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MAX3485_EN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
