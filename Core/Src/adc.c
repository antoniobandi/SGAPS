/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "firCoeffs.h"
#include "signal_matlab.h"
#include <complex.h>
#include "fft.h"
#include "usart.h"
#include "lcd.h"


void cutOffSetup() {
		if(cutOffFreq == 0.1) {
	    	 copy_LP(firCoef_LP, firCoef_LP_01);
	    	 copy_HP(firCoef_HP, firCoef_HP_01);
	       } else if(cutOffFreq == 0.2) {
	         copy_LP(firCoef_LP, firCoef_LP_02);
	         copy_HP(firCoef_HP, firCoef_HP_02);
	       } else if(cutOffFreq == 0.3) {
	         copy_LP(firCoef_LP, firCoef_LP_03);
	         copy_HP(firCoef_HP, firCoef_HP_03);
	       } else if(cutOffFreq == 0.4) {
	         copy_LP(firCoef_LP, firCoef_LP_04);
	         copy_HP(firCoef_HP, firCoef_HP_04);
	       } else if(cutOffFreq == 0.5) {
	         copy_LP(firCoef_LP, firCoef_LP_05);
	         copy_HP(firCoef_HP, firCoef_HP_05);
	       } else if(cutOffFreq == 0.6) {
	         copy_LP(firCoef_LP, firCoef_LP_06);
	         copy_HP(firCoef_HP, firCoef_HP_06);
	       } else if(cutOffFreq == 0.7) {
	         copy_LP(firCoef_LP, firCoef_LP_07);
	         copy_HP(firCoef_HP, firCoef_HP_07);
	       } else if(cutOffFreq == 0.8) {
	         copy_LP(firCoef_LP, firCoef_LP_08);
	         copy_HP(firCoef_HP, firCoef_HP_08);
	       } else {
	         copy_LP(firCoef_LP, firCoef_LP_09);
	         copy_HP(firCoef_HP, firCoef_HP_09);
	       }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {

	HAL_ADC_Stop_DMA(&hadc3);

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = 1;

//	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, 1);
	for(int i = 0; i < BUFFER_SIZE; ++i) {
				array[i] = -amplitude/2 + (float)buffer[i]/FS_INT *(amplitude/2 + 1);
				arrayInt[i] = buffer[i];
			}

	float sum = 0;

	if(lowpass) {											//lowpass= 1 -> LP
		for(int n = 0; n < BUFFER_SIZE; ++n) {
			for(int k = 0; k <= FILTER_ORDER; ++k) {
				if(n - k >= 0)
					sum += firCoef_LP[k] * array[n-k];
				else
					sum += firCoef_LP[k] * arrayFormer[BUFFER_SIZE + n - k + 1];
			}
			filteredArray[n] = sum;
			sum = 0;
		}
	} else {											//!lowpass = 0 -> HP
		for(int n = 0; n < BUFFER_SIZE; ++n) {
			for(int k = 0; k <= FILTER_ORDER; ++k) {
				if(n - k >= 0)
					sum += firCoef_HP[k] * array[n-k];
				else
					sum += firCoef_HP[k] * arrayFormer[BUFFER_SIZE + n - k + 1];
			}
			filteredArray[n] = sum;
			sum = 0;
		}
	}

	for(int i = 0; i < BUFFER_SIZE; i++) {
		filteredArray_int[i] = filteredArray[i] * FS_INT_HALF + FS_INT_HALF;
		arrayFormer[i] = array[i];
	}

		if(!signal_q) {

			if(filtered){
				int v = 0;
				int spectrum_int[BUFFER_SIZE];
				for (int k = 0; k < 32; k++) spectrum_int[k] = 0;
				float complex vector[256];
				for (int k = 0; k < 256; k++){
					vector[k] = (float) filteredArray_int[k];
				}
				fft(vector, 256);
				for (int k = 32; k < 288; k++){
					if ((int) cabsf(vector[v]) > 4095) vector[v] = 4095;
					spectrum_int[k] = (int) cabsf(vector[v]);
					v++;
				}
				for (int k = 288; k < 320; k++) spectrum_int[k] = 0;
				SendToScreen(spectrum_int);

			} else {
				int v = 0;
				int spectrum_int[BUFFER_SIZE];
				for (int k = 0; k < 32; k++) spectrum_int[k] = 0;
				float complex vector[256];
				for (int k = 0; k < 256; k++){
					vector[k] = (float) arrayInt[k];
				}
				fft(vector, 256);
				for (int k = 32; k < 288; k++){
					if ((int) cabsf(vector[v]) > 4095) vector[v] = 4095;
					spectrum_int[k] = (int) cabsf(vector[v]);
					v++;
				}
				for (int k = 288; k < 320; k++) spectrum_int[k] = 0;
				SendToScreen(spectrum_int);

			}
		} else {
			if(filtered){
				SendToScreen(filteredArray_int);

			} else {
				SendToScreen(arrayInt);

			}
		}

	matlab();

	switch (freq_scale){

		case 1:
			sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
			SetScreenTime(1);
			break;

		case 2:
			sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
			SetScreenTime(2);
			break;

		case 3:
			sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
			SetScreenTime(3);
			break;

		case 4:
			sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
			SetScreenTime(4);
			break;

		case 5:
			sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
			SetScreenTime(5);
			break;

		case 6:
			sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
			SetScreenTime(6);
			break;

		case 7:
			sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
			SetScreenTime(7);
			break;

		case 8:
			sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
			SetScreenTime(8);
			break;

		default:
			sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
			SetScreenTime(8);
			break;
	}

	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK){
		Error_Handler();
	}
//		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, 0);

		HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&buffer, BUFFER_SIZE);


}

void copy_LP(volatile float* array1, float *array2) {
	for (int i = 0; i < BUFFER_SIZE; i++)
		*(array1 + i) = *(array2 + i);
}

void copy_HP(volatile float* array1, float *array2) {
	for (int i = 0; i < BUFFER_SIZE + 1; i++)
		*(array1 + i) = *(array2 + i);
}
/* USER CODE END 0 */

ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc3;

/* ADC3 init function */
void MX_ADC3_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspInit 0 */

  /* USER CODE END ADC3_MspInit 0 */
    /* ADC3 clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**ADC3 GPIO Configuration
    PF6     ------> ADC3_IN4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* ADC3 DMA Init */
    /* ADC3 Init */
    hdma_adc3.Instance = DMA2_Stream0;
    hdma_adc3.Init.Channel = DMA_CHANNEL_2;
    hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc3.Init.Mode = DMA_CIRCULAR;
    hdma_adc3.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc3) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc3);

  /* USER CODE BEGIN ADC3_MspInit 1 */

  /* USER CODE END ADC3_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspDeInit 0 */

  /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC3_CLK_DISABLE();

    /**ADC3 GPIO Configuration
    PF6     ------> ADC3_IN4
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6);

    /* ADC3 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC3_MspDeInit 1 */

  /* USER CODE END ADC3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
