//sine8_buf.c sine generation with output stored in buffer
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#include <math.h>

typedef struct COMPLEX_TAG { short re, im; } COMPLEX;

#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#define BUFFERLENGTH 8192

#define FRAMESIZE 1024
#define HOPSIZE FRAMESIZE << 2
#define PI 3.14
#define TWOPI 2*PI
#define NUMBEROFFRAMES 28
#define SQRT2 sqrt(2)
#define SQRT4 sqrt(4)



float hann[FRAMESIZE];
COMPLEX windowedFrame[FRAMESIZE];

COMPLEX firstBuff[BUFFERLENGTH];
COMPLEX secondBuff[BUFFERLENGTH];

COMPLEX frames[NUMBEROFFRAMES][FRAMESIZE];
COMPLEX outputFrame[NUMBEROFFRAMES][FRAMESIZE];
COMPLEX frameFFT[FRAMESIZE];
short prevPhi[FRAMESIZE];
short phiCulm[FRAMESIZE];
short deltaPhiPrime[FRAMESIZE];
short deltaPhi = 0;

void initHann() {
	int i;
	// Fyll hann med ett hann-fönster
	for(i = 0; i < FRAMESIZE; i++) {
		hann[i] = 0.5 * (1 - cos((TWOPI*i)/(FRAMESIZE-1)));
	}
}

void pitchshift(short *input, short *output, short steps) {
	int i,j;
	int start;
	// Dela upp buffern i frames
	start = 0;
	for(i = 0; i < NUMBEROFFRAMES; i++) {
		for(j = 0; j < FRAMESIZE; j++) {
			frames[i][j] = input[start+j];
		}
		start = start + HOPSIZE;
	}

	// Eliminera fasskillnad
	for(i = 0; i < NUMBEROFFRAMES; i++) {
		for(j = 0; j < FRAMESIZE; j++) {
			windowedFrame[j] = frames[i][j] * hann[j] / SQRT2;
		}
		// Fyll frameFFT
		dsp_fft();

		// FAS-skillnad mellan nuvarande och förra fönstret
		for(j = 0; j < FRAMESIZE; j++) {
			deltaPhi[j] = frameFFT[j]->im - prevPhi[j];
			prevPhi[j] = frameFFT[j]->im;
		}

		// Sätter fasskillnad mellan -pi och pi
		for(j = 0; j < FRAMESIZE; j++) {
			deltaPhi[j] = fmod(deltaPhi[j]-HOPSIZE*TWOPI*j/FRAMESIZE, TWOPI) - PI;
		}
		
		for(j = 0; j < FRAMESIZE; j++) {
			trueFREQ[j] = TWOPI*j/FRAMESIZE + deltaPhi/HOPSIZE;
		}

		for(j = 0; j < FRAMESIZE; j++) {
			phiCulm[j] += HOPSIZEOUT*trueFREQ[j];
		}
		
		for(j = 0; j < FRAMESIZE; j++) {
			frameFFT[j].im = phiCulm[j];
		}

		dsp_ifft();

		for(j = 0; j < FRAMESIZE; j++) {
			outputFrame[i][j] = ifftFrame*hann[j]/SQRT2;
		}
	}

	// Strecha signalen
	yTimeStretch[NUMBEROFFRAMES*HOPSIZEOUT-HOPSIZEOUT+FRAMESIZE];
	timeIndex = 0;

	for(i = 0; i < NUMBEROFFRAMES; i++) {
		for(j = 0; j < FRAMESIZE; j++) {
			yTimeStrech[timeIndex+j] += outputFrame[i][j];
		}		
		timeIndex += HOPSIZE;
	}

	// OMSAMPLA SIGNALEN
	y[BUFFERSIZE];
	
	// linjär interpolation
	for(i = 0; i < NUMBEROFFRAMES*HOPSIZEOUT-HOPSIZEOUT+FRAMESIZE; i++) {
		
	}

	// KLART! 
}

interrupt void c_int11()                    //interrupt service routine
{
  short out_sample = 0;
  output_left_sample(out_sample);	       //output sample value
  return;					               //return from interrupt
}

void main()
{
  comm_intr();	//initialise DSK
  while(1);     //infinite loop
}
 
