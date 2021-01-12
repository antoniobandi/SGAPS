#include "main.h"
#include <stdbool.h>

#define	SEND_SIZE	64
#define	RESPONSE_SIZE	15
#define AMP_COEF	0.0004884005f
#define	GEN_S	true
#define SINE_S	true
#define SIG_S	true
#define FIL_S	false
#define LOWPASS_S	true
#define AMP_TEMP	2000
#define AMP_S	1.00
#define FREQ_S	500
#define FGFREQ_S	0.5
#define FREQ_MIN	100
#define AMP_MIN	0

extern bool gen;
extern bool sinewave;
extern bool signal_q;
extern bool filtered;
extern bool lowpass;
extern long int amplitude_temp;
extern float amplitude;
extern long int frequency;
extern double cutOffFreq;

char temp[2];

void matlab(void);
