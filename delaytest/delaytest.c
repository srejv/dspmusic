//delaytest.c Test the delay implementation
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#include <math.h>
#define M_PI 3.14159265359
#include <limits.h>
#include "qmath.h"

#define N 256
#define DELAY 8000
short output[N];
short input[N];
fixedp processing[N];

unsigned short delayPointer = 0;
fixedp delay[DELAY];

void process() {
	int i;
	fixedp tmp;
	fixedp in;
	for(i=0; i < N; i++) {
		tmp = delay[delayPointer];
		delay[delayPointer] = (fixedp)input[i];
		output[i] = tmp;
	}
}


short chin;
interrupt void c_int11()    //interrupt service routine
{	
	chin = input_left_sample(); 
	output_left_sample(chin);
  	return;					//return from interrupt
}

void main()
{
  	comm_intr();              // initialise DSK
    while(1);             	// infinite loop
}
