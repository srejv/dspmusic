#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_48KHZ;	//set sampling rate
#include <math.h>
#include <dsp_fft.h>
#include "twiddlefactors16x16n8192.c"

typedef struct COMPLEX_TAG { short re, im; } COMPLEX;

#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#define N 8192

short buffer[N];
short inputBuffer[N];
short fftOutputBuffer[N];
short done = 0;

short index = 0;
char buffFullFlag = 0;


interrupt void c_int11() {
	short input = input_left_sample();
	buffer[index] = input;
	buffer[index] = 0;

	if(++index >= N) { 
		buffFullFlag = 1;
		index = 0; 
	} else {
		index = index + 2;
	}
	return;
}

void main() {
	int i;
	comm_intr();
	while(1) {
		while(!buffFullFlag) ; // Wait for buffFullFlag
		buffFullFlag = 0;
		done = 0;
		for(i = 0; i < N; i++) {
			inputBuffer[i] = buffer[i];
		}

		DSP_fft16x32(w, N, inputBuffer, fftOutputBuffer);
		done = 1;
		DSP_ifft16x32(w, N, fftOutputBuffer, inputBuffer);
	}
}
