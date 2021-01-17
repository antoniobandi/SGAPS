#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include "cmplx.h"

typedef float cmplx_t[2];

void cmplx_div(cmplx_t a, cmplx_t b, cmplx_t c){
    c[0] = (a[0] * b[0] + a[1] * b[1]) / (b[0] * b[0] + b[1] * b[1]);
    c[1] = (a[1] * b[0] - a[0] * b[1]) / (b[0] * b[0] + b[1] * b[1]);
}

void cmplx_mul(cmplx_t a, cmplx_t b, cmplx_t c){
    c[0] = a[0] * b[0] - a[1] * b[1];
    c[1] = a[0] * b[1] + a[1] * b[0];
}

double cmplx_mag(cmplx_t a){
    double rj;
    rj = sqrt(a[0] * a[0] + a[1] * a[1]);
    return rj;
}

double cmplx_phs(cmplx_t a){
    double ph;
    ph = atan(a[1] / a[0]);
    return ph;
}

double cmplx_real(double mag, double phs){
    double real;
    real = mag * cos(phs);
    return real;
}

double cmplx_imag(double mag, double phs){
    double imag;
    imag = mag * sin(phs);
    return imag;
}

void cmplx_dft(cmplx_t *input, cmplx_t *output, int N){
    float *ptrin = input;
    float *ptrout = output;

    for (size_t k = 0; k < N; k++) {
		double sumreal = 0;
		double sumimag = 0;
		for (size_t t = 0; t < N; t++) {
			double angle = 2 * M_PI * t * k / N;
			sumreal +=  (*(ptrin + 2 * t)) * cos(angle) + (*(ptrin + 1 + t * 2)) * sin(angle);
			sumimag += -(*(ptrin + 2 * t)) * sin(angle) + (*(ptrin + 1 + t * 2)) * cos(angle);
		}
		*(ptrout + k * 2) = (float) sumreal;
		*(ptrout + 1 + k * 2) = (float) sumimag;
	}
}

void cmplx_idft(cmplx_t *input, cmplx_t *output, int N){
    float *ptrin = input;
    float *ptrout = output;

    for (size_t n = 0; n < N; n++) {
		double sumreal = 0;
		double sumimag = 0;
		for (size_t k = 0; k < N; k++) {
			double angle = 2 * M_PI * n * k / N;
			sumreal +=  (*(ptrin + 2 * k)) * cos(angle) - (*(ptrin + 1 + k * 2)) * sin(angle);
			sumimag +=  (*(ptrin + 2 * k)) * sin(angle) + (*(ptrin + 1 + k * 2)) * cos(angle);
		}
		*(ptrout + n * 2) = (float) sumreal/N;
		*(ptrout + 1 + n * 2) = (float) sumimag/N;
	}
}
