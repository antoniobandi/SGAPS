
#include "lcd.h"
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
	TM_DMA2DRAPHIC_Poly_t polje = {0};
	for(int i=0;i<BUFFER_SIZE;i++){
		int amplitude = -0.04297924298 * signal[i] + 180;
		polje.X = width;
		polje.Y = amplitude;
		if(width == 318){
			width = 1;
			Window();
		}
		width++;
	}
	TM_DMA2DGRAPHIC_DrawPolyLine(&polje, BUFFER_SIZE, LCD_COLOR_YELLOW);
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
	sprintf(f,"%.ld",freq);
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
	sprintf(f,"%.ld",freq);
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
	TM_LCD_DrawFilledRectangle(1, 185, 109, 20, LCD_COLOR_WHITE);

	TM_LCD_SetFont(&TM_Font_11x18);
	TM_LCD_SetXY(1,240*3/4+5);
	TM_LCD_Puts(&title[0]);
	TM_LCD_SetFont(&TM_Font_7x10);
	SendToScreen(signal);
}

void Window(void){
	TM_LCD_DrawFilledRectangle(1, 1, 318, 181, LCD_COLOR_BLACK);

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



























