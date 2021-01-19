/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 0*/
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE); //change
  /* USER CODE END USART1_Init 0*/
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
char RX_BUFFER[BUFSIZE];
int RX_BUFFER_HEAD, RX_BUFFER_TAIL;

void  USER_UART_IRQHandler(UART_HandleTypeDef *huart) {
	if( huart ->Instance  ==  USART1 ) {
		rx_data = __HAL_UART_FLUSH_DRREGISTER( huart );

		static  char  rx_head;
		rx_head = RX_BUFFER_HEAD + 1;
		if( rx_head  ==  BUFSIZE ) {
			rx_head = 0;
		}
		if( rx_head  !=  RX_BUFFER_TAIL ) {
			RX_BUFFER[RX_BUFFER_HEAD] = rx_data;
			RX_BUFFER_HEAD = rx_head;
		}
	}
}

void  USART1_SendChar(uint8_t c) {
	HAL_UART_Transmit (&huart1 , &c, sizeof(c), 10);
}

void USART1_SendFloat(float v) {
	array_cast = (uint8_t *) (&v);
	USART1_SendChar(array_cast[0]);
	USART1_SendChar(array_cast[1]);
	USART1_SendChar(array_cast[2]);
	USART1_SendChar(array_cast[3]);
}

void USART1_SendInt(int v) {
	array_cast = (uint8_t *) (&v);
	USART1_SendChar(array_cast[0]);
	USART1_SendChar(array_cast[1]);
}

int  USART1_Dequeue(char* c) {
	int  ret;
	ret = 0;
	*c = 0;
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	if (RX_BUFFER_HEAD  !=  RX_BUFFER_TAIL) {
		*c = RX_BUFFER[RX_BUFFER_TAIL ];
		RX_BUFFER_TAIL ++;
		if (RX_BUFFER_TAIL  ==  BUFSIZE) {
			RX_BUFFER_TAIL = 0;
		}
		ret = 1;
	}
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	return  ret;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
