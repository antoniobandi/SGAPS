#include "fft.h"
#include "usart.h"
#include "signal_matlab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <complex.h>

bool gen = GEN_S;
bool sinewave = SINE_S;
bool signal_pc = PC_S;
bool signal_led = LED_S;
bool lowpass = LOWPASS_S;
long int amplitude_temp = AMP_TEMP;
float amplitude = AMP_S;
long int frequency = FREQ_S;
double fg_frequency = FGFREQ_S;

char c;
char *ptr;
char temp[2] = {'-', '\0'};

float i = 0;



void matlab(void){

	if (gen){
		if (sinewave){
			if (signal_pc){
				for (int k = 0; k < SEND_SIZE; k++){
					USART1_SendValue((float) frequency);
				}
				USART1_SendChar('\r');
				USART1_SendChar('\n');
			}

			else{
				for (int k = 0; k < SEND_SIZE; k++){
					USART1_SendValue((float) sin(i));
					i+=0.2;
				}
				USART1_SendChar('\r');
				USART1_SendChar('\n');
			}
		}

		else{
			if (signal_pc){
				for (int k = 0; k < SEND_SIZE; k++){
					USART1_SendValue(amplitude);
				}
				USART1_SendChar('\r');
				USART1_SendChar('\n');
			}

			else{
				i = 0;
				float complex vector[SEND_SIZE];
				for (int k = 0; k < SEND_SIZE; k++){
					vector[k] = (float) sin(i);
					i+=0.2;
				}
				fft(vector,SEND_SIZE);
				for (int k = 0; k < SEND_SIZE; k++){
					USART1_SendValue((float) cabsf(vector[k]));
				}
				USART1_SendChar('\r');
				USART1_SendChar('\n');
			}
		}
	}

	else{
		for (int k = 0; k < SEND_SIZE; k++){
			USART1_SendValue((float) 0);
		}
		USART1_SendChar('\r');
		USART1_SendChar('\n');
	}

	char response[RESPONSE_SIZE];
	response[0] = '-';
	response[1] = '\0';
	
	while (USART1_Dequeue(&c)){
		temp[0] = c;
		strncat(response,temp,1);
	}

	switch (response[1]){
		
		case '\0':
			break;

		case 'G' :
			if (strncmp(":on",strchr(response, ':'),3) == 0 ){
				gen = true;
			}
			else if (strncmp(":off", strchr(response,':'),4) == 0){
				gen = false;
			}
			break;

		case 'W':
			if (strncmp(":Sin",strchr(response, ':'),4) == 0 ){
				sinewave = true;
			}
			else if (strncmp(":Pra", strchr(response,':'),4) == 0){
				sinewave = false;
			}
			break;

		case 'F':
			frequency = strtol(strchr(response,'0'),&ptr,10);
			if (frequency <= 0) frequency = FREQ_MIN;
			break;

		case 'A':
			amplitude_temp = strtol(strchr(response,'0'),&ptr,10);
			amplitude = amplitude_temp * AMP_COEF;
			if (amplitude < 0) amplitude = AMP_MIN;
			break;

		case 'P':
			if (strncmp(":low",strchr(response, ':'),4) == 0 ){
				lowpass = true;
			}
			else if (strncmp(":high", strchr(response,':'),5) == 0){
				lowpass = false;
			}
			break;

		case 'S':
			if (strncmp(":Sig",strchr(response, ':'),4) == 0 ){
				signal_pc = true;
			}
			else if (strncmp(":Spec", strchr(response,':'),5) == 0){
				signal_pc = false;
			}
			break;

		case 'L':
			if (strncmp(":Sig",strchr(response, ':'),4) == 0 ){
				signal_led = true;
			}
			else if (strncmp(":Spec", strchr(response,':'),5) == 0){
				signal_led = false;
			}
			break;

		case 'Q':
			gen = GEN_S;
			sinewave = SINE_S;
			signal_pc = PC_S;
			signal_led = LED_S;
			lowpass = LOWPASS_S;
			amplitude_temp = AMP_TEMP;
			amplitude = AMP_S;
			frequency = FREQ_S;
			fg_frequency = FGFREQ_S;
			break;

		case 'U':
			if (strncmp(":0.1",strchr(response, ':'),4) == 0 ){
				fg_frequency = 0.1;
			}
			else if (strncmp(":0.2", strchr(response,':'),4) == 0){
				fg_frequency = 0.2;
			}
			else if (strncmp(":0.3", strchr(response,':'),4) == 0){
				fg_frequency = 0.3;
			}
			else if (strncmp(":0.4", strchr(response,':'),4) == 0){
				fg_frequency = 0.4;
			}
			else if (strncmp(":0.5", strchr(response,':'),4) == 0){
				fg_frequency = 0.5;
			}
			else if (strncmp(":0.6", strchr(response,':'),4) == 0){
				fg_frequency = 0.6;
			}
			else if (strncmp(":0.7", strchr(response,':'),4) == 0){
				fg_frequency = 0.7;
			}
			else if (strncmp(":0.8", strchr(response,':'),4) == 0){
				fg_frequency = 0.8;
			}
			else if (strncmp(":0.9", strchr(response,':'),4) == 0){
				fg_frequency = 0.9;
			}
			break;

		default:
			break;
	}
}
