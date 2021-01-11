/**
  ******************************************************************************
  * @file    ltdc.c
  * @brief   This file provides code for the configuration
  *          of the LTDC instances.
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
#include "ltdc.h"

/* USER CODE BEGIN 0 */


#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int width;


void StartScreen(void){
	TM_LCD_Init();
	TM_LCD_SetOrientation(2);
	TM_LCD_SetLayer1();
	TM_LCD_Fill(LCD_COLOR_WHITE);


	ScreenIntro();

	/*	prozor za osciloskop	*/

	Window();

	/*	generator; ime i ON/OFF */

	TM_LCD_SetXY(0, TM_LCD_GetHeight()-22);
	TM_LCD_SetFont(&TM_Font_7x10);
	char s[] = "Function generator";
	TM_LCD_Puts(&s[0]);
	char on[] = "ON";
	TM_LCD_SetXY(8, TM_LCD_GetHeight()-11);
	TM_LCD_Puts(&on[0]);
	char off[] = "OFF";
	TM_LCD_SetXY(28, TM_LCD_GetHeight()-11);
	TM_LCD_Puts(&off[0]);

	/*	prozor za generator	*/

	for(int i=135; i<160;i++){
		TM_LCD_DrawPixel(i, TM_LCD_GetHeight()-22, LCD_COLOR_BLACK);
	}
	for(int i=135; i<161;i++){
			TM_LCD_DrawPixel(i, TM_LCD_GetHeight()-5, LCD_COLOR_BLACK);
		}
	for(int i=TM_LCD_GetHeight()-22; i<TM_LCD_GetHeight()-5;i++){
			TM_LCD_DrawPixel(135, i, LCD_COLOR_BLACK);
		}
	for(int i=TM_LCD_GetHeight()-22; i<TM_LCD_GetHeight()-5;i++){
			TM_LCD_DrawPixel(160, i, LCD_COLOR_BLACK);
		}
	char yaxis[] = "0.15 V/div";
	TM_LCD_SetXY(210,TM_LCD_GetHeight()*3/4+5);
	TM_LCD_Puts(&yaxis[0]);
}

void ScreenIntro(void){
	char begin[] = "Sustav za generiranje,";
	char end[] = "akviziciju i prikaz signala";
	TM_LCD_SetXY(35, TM_LCD_GetHeight()/2-22);
	TM_LCD_Puts(&begin[0]);
	TM_LCD_SetXY(10, TM_LCD_GetHeight()/2);
	TM_LCD_Puts(&end[0]);
	char fer[] = "FER, 2020./2021.";
	TM_LCD_SetFont(&TM_Font_7x10);
	TM_LCD_SetXY(8, TM_LCD_GetHeight()-15);
	TM_LCD_Puts(&fer[0]);
	for(int i=0; i<20000000;i++){}
	char next1[] = "Press user button";
	char next2[] = "to continue";
	TM_LCD_SetXY(200, TM_LCD_GetHeight()-20);
	TM_LCD_Puts(&next1[0]);
	TM_LCD_SetXY(200, TM_LCD_GetHeight()-10);
	TM_LCD_Puts(&next2[0]);
	while(1){
		if(TM_DISCO_ButtonPressed() > 0) {
			TM_LCD_Fill(LCD_COLOR_WHITE);
			break;
		}
	}

}

void LCDFunctionGeneratorON(void){
	for(int i=7;i<23;i++){
		for(int j=TM_LCD_GetHeight()-13;j<TM_LCD_GetHeight()-2;j++){
			if(TM_LCD_GetPixel(i, j) == LCD_COLOR_WHITE){
				TM_LCD_DrawPixel(i, j, LCD_COLOR_GREEN2);
			}
		}
	}
	for(int i=27;i<49;i++){
		for(int j=TM_LCD_GetHeight()-13;j<TM_LCD_GetHeight()-2;j++){
			if(TM_LCD_GetPixel(i, j) == LCD_COLOR_RED2){
				TM_LCD_DrawPixel(i, j, LCD_COLOR_WHITE);
			}
		}
	}
}

void LCDFunctionGeneratorOFF(void){
	for(int i=27;i<49;i++){
		for(int j=TM_LCD_GetHeight()-13;j<TM_LCD_GetHeight()-2;j++){
			if(TM_LCD_GetPixel(i, j) == LCD_COLOR_WHITE){
				TM_LCD_DrawPixel(i, j, LCD_COLOR_RED2);
			}
		}
	}
	for(int i=7;i<23;i++){
		for(int j=TM_LCD_GetHeight()-13;j<TM_LCD_GetHeight()-2;j++){
			if(TM_LCD_GetPixel(i, j) == LCD_COLOR_GREEN2){
				TM_LCD_DrawPixel(i, j, LCD_COLOR_WHITE);
			}
		}
	}
	for(int i=137;i<159;i++){
		for(int j=TM_LCD_GetHeight()-18;j<TM_LCD_GetHeight()-7;j++){
			TM_LCD_DrawPixel(i, j, LCD_COLOR_WHITE);
		}
	}
	char gasi[] = "               ";
	TM_LCD_SetXY(165, TM_LCD_GetHeight()-18);
	TM_LCD_Puts(&gasi[0]);
}

void SendToScreen(int* signal){
	for(int i=0;i<BUFFER_SIZE;i++){
		int amplitude = -0.04297924298 * signal[i] + 180;
		TM_LCD_DrawPixel(width, amplitude, LCD_COLOR_YELLOW );
		width++;
		if(width == 318){
			width = 1;
			Window();
		}
	}
}

void SetSquareOutput(long int freq){
	LCDFunctionGeneratorON();
	double sinus[20];
	for(int i=0; i<20; i++){
		double a = (double) i;
		sinus[i] = sin(0.33*a);
	}

	for(int i=0;i<20;i++){
		int amplitude = (int)(-6* sinus[i] + 227);
		TM_LCD_DrawPixel(i+138, amplitude, LCD_COLOR_WHITE );
	}
	for(int i=147; i<159;i++){
		TM_LCD_DrawPixel(i, TM_LCD_GetHeight()-18, LCD_COLOR_RED2);
	}
	for(int i=137; i<147;i++){
		TM_LCD_DrawPixel(i, TM_LCD_GetHeight()-8, LCD_COLOR_RED2);
	}
	for(int i=TM_LCD_GetHeight()-18; i<TM_LCD_GetHeight()-8;i++){
		TM_LCD_DrawPixel(147, i, LCD_COLOR_RED2);
	}
	char f[10] = {0};
	sprintf(f,"%ld",freq);
	for(int i=165;i<320;i++){
		for(int j=TM_LCD_GetHeight()-18;j<TM_LCD_GetHeight();j++){
			TM_LCD_DrawPixel(i, j, LCD_COLOR_WHITE);
		}
	}
	TM_LCD_SetXY(165, TM_LCD_GetHeight()-18);
	TM_LCD_Puts(&f[0]);
	char f1[] = " Hz";
	TM_LCD_Puts(&f1[0]);

}

void SetSineOutput(long int freq){

	LCDFunctionGeneratorON();
	double sinus[20];
	for(int i=147; i<159;i++){
		TM_LCD_DrawPixel(i, TM_LCD_GetHeight()-18, LCD_COLOR_WHITE);
	}
	for(int i=137; i<147;i++){
		TM_LCD_DrawPixel(i, TM_LCD_GetHeight()-8, LCD_COLOR_WHITE);
	}
	for(int i=TM_LCD_GetHeight()-18; i<TM_LCD_GetHeight()-8;i++){
		TM_LCD_DrawPixel(147, i, LCD_COLOR_WHITE);
	}

	for(int i=0; i<20; i++){
		double a = (double) i;
		sinus[i] = sin(0.33*a);
	}

	for(int i=0;i<20;i++){
		int amplitude = (int)(-6* sinus[i] + 227);
		TM_LCD_DrawPixel(i+138, amplitude, LCD_COLOR_RED2);
	}
	char f[10] = {0};
	sprintf(f,"%ld",freq);
	for(int i=165;i<320;i++){
			for(int j=TM_LCD_GetHeight()-18;j<TM_LCD_GetHeight();j++){
				TM_LCD_DrawPixel(i, j, LCD_COLOR_WHITE);
			}
		}
	TM_LCD_SetXY(165, TM_LCD_GetHeight()-18);
	TM_LCD_Puts(&f[0]);
	char f1[] = " Hz";
	TM_LCD_Puts(&f1[0]);
}

void ShowSignal(int* signal, char title[]){
	for(int i=1; i<319;i++){
		for(int j=1;j<TM_LCD_GetHeight()*3/4;j++){
			if(TM_LCD_GetPixel(i, j) == LCD_COLOR_YELLOW){
				TM_LCD_DrawPixel(i, j, LCD_COLOR_BLACK);
			}
		}
	}
	for(int i=1;i<110;i++){
		for(int j=185;j<205;j++){
			TM_LCD_DrawPixel(i, j, LCD_COLOR_WHITE);
		}
	}
	TM_LCD_SetFont(&TM_Font_11x18);
	TM_LCD_SetXY(1,TM_LCD_GetHeight()*3/4+5);
	TM_LCD_Puts(&title[0]);
	TM_LCD_SetFont(&TM_Font_7x10);
	SendToScreen(signal);
}

void Window(void){
	for(int i=1; i<319;i++){
			for(int j=1;j<TM_LCD_GetHeight()*3/4+2;j++){
				TM_LCD_DrawPixel(i, j, LCD_COLOR_BLACK);
			}
		}
		for(int i=1; i<319;i++){
			for(int j=1;j<TM_LCD_GetHeight()*3/4;j++){
				if(j==91){
					if(i%3==0){
						TM_LCD_DrawPixel(i, j, LCD_COLOR_WHITE);
					}
				}
				if((i%16==0)&&(j%9==0))	TM_LCD_DrawPixel(i, j, LCD_COLOR_WHITE);
			}
		}
}


/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Init(void)
{
  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 7;
  hltdc.Init.VerticalSync = 3;
  hltdc.Init.AccumulatedHBP = 14;
  hltdc.Init.AccumulatedVBP = 5;
  hltdc.Init.AccumulatedActiveW = 654;
  hltdc.Init.AccumulatedActiveH = 485;
  hltdc.Init.TotalWidth = 660;
  hltdc.Init.TotalHeigh = 487;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 0;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 0;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 0;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 0;
  pLayerCfg.ImageHeight = 0;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 0;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 0;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg1.FBStartAdress = 0;
  pLayerCfg1.ImageWidth = 0;
  pLayerCfg1.ImageHeight = 0;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */
    /* LTDC clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**LTDC GPIO Configuration
    PF10     ------> LTDC_DE
    PA3     ------> LTDC_B5
    PA4     ------> LTDC_VSYNC
    PA6     ------> LTDC_G2
    PB0     ------> LTDC_R3
    PB1     ------> LTDC_R6
    PB10     ------> LTDC_G4
    PB11     ------> LTDC_G5
    PG6     ------> LTDC_R7
    PG7     ------> LTDC_CLK
    PC6     ------> LTDC_HSYNC
    PC7     ------> LTDC_G6
    PA11     ------> LTDC_R4
    PA12     ------> LTDC_R5
    PD3     ------> LTDC_G7
    PG10     ------> LTDC_G3
    PG11     ------> LTDC_B3
    PG12     ------> LTDC_B4
    PB8     ------> LTDC_B6
    PB9     ------> LTDC_B7
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_11
                          |GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* USER CODE BEGIN LTDC_MspInit 1 */

  /* USER CODE END LTDC_MspInit 1 */
  }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle)
{

  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /**LTDC GPIO Configuration
    PF10     ------> LTDC_DE
    PA3     ------> LTDC_B5
    PA4     ------> LTDC_VSYNC
    PA6     ------> LTDC_G2
    PB0     ------> LTDC_R3
    PB1     ------> LTDC_R6
    PB10     ------> LTDC_G4
    PB11     ------> LTDC_G5
    PG6     ------> LTDC_R7
    PG7     ------> LTDC_CLK
    PC6     ------> LTDC_HSYNC
    PC7     ------> LTDC_G6
    PA11     ------> LTDC_R4
    PA12     ------> LTDC_R5
    PD3     ------> LTDC_G7
    PG10     ------> LTDC_G3
    PG11     ------> LTDC_B3
    PG12     ------> LTDC_B4
    PB8     ------> LTDC_B6
    PB9     ------> LTDC_B7
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_11
                          |GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_8|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_3);

  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
