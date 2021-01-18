#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"
#include "defines.h"
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_lcd.h"
#include <math.h>

void StartScreen(void);
void ScreenIntro(void);
void LCDFunctionGeneratorON(void);
void LCDFunctionGeneratorOFF(void);
void SetSquareOutput(long int freq);
void SetSineOutput(long int freq);
void SendToScreen(int *signal);
void STS(int* signal);
void ChangeTitle(char title[]);
void ShowSignal(int* signal, char title[]);
void Window(void);

extern int width;

#endif /* INC_LCD_H_ */
