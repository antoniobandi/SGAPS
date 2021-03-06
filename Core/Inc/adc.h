/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"
//#include "firCoeffs.h"
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc3;

/* USER CODE BEGIN Private defines */
#define FS_INT		4095.0f
#define	FS_INT_HALF	2047
#define FILTER_ORDER 	32
/* USER CODE END Private defines */

void MX_ADC3_Init(void);

/* USER CODE BEGIN Prototypes */
void cutOffSetup(void);
void copy_LP(volatile float* array1, float *array2);
void copy_HP(volatile float* array1, float *array2);

__IO uint16_t buffer[BUFFER_SIZE];			//Tu cu spremati digitalizirani signal iz ADC-a
__IO float array[BUFFER_SIZE];			//Tu cu spremati digitalizirani signal iz buffera spreman za filtriranje
__IO float filteredArray[BUFFER_SIZE];	//Tu cu spremati isfiltrirani signal
__IO float firCoef_HP[BUFFER_SIZE + 1];		//Polje za koeficijente visokopropusnog filtra
__IO float firCoef_LP[BUFFER_SIZE];			//Polje za koeficijente niskopropusnog filtra
//__IO _Bool filterType;						//Varijabla za odabir vrste filtra (1->HP, 0->LP)
//__IO double cutOffFreq;						//Varijabla u kojoj je spremljena granicna frekvencija
__IO int filteredArray_int[BUFFER_SIZE];
__IO int arrayInt[BUFFER_SIZE];
__IO float arrayFormer[BUFFER_SIZE];
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
