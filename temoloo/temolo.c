//delay.c Basic time delay
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_48KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input
#include <math.h>

#define BUF_SIZE 48000
#define GAIN 0.5
int i;
float sr = 48000.f;
float gain = 0.9f, rp1 = 0.f, fdb = 0.f;
//int taps = 2, dsize = 360, wp = 0;
short input,output,delayed;
short delay[BUF_SIZE];

float inc = 0; 
int rpi =0;
float frac = 0.f;
short next = 0.f;
#define N 8000
float vibsine[N];
float depth = 0.4f;

int vibp = 0;

float offset;
float delaytime = 0.5;

interrupt void c_int11()	 // interrupt service routine
{
	input = input_left_sample();  // read new input sample from ADC 

	rpi = (int)rp1;
	frac = rp1 - rpi;
	next = (rpi != BUF_SIZE-1 ? delay[rpi+1] : delay[0]);
 	delayed = (int)((delay[(int)rp1] + frac*(next-delay[rpi])));
 	output = (1-depth) * delayed + delayed * (depth * vibsine[vibp]);     // output sum of input and delayed values
 	output_left_sample(output);
 	delay[i] = input + output * GAIN; // store new input and a fraction
	
	                          // of the delayed value in buffer
  	if(++i >= BUF_SIZE) i=0;      // test for end of buffer
	rp1 = rp1 + 1; // + vibsine[vibp];
	if(rp1 >= BUF_SIZE) rp1 = 0;
	if(++vibp >= N)  vibp = 0;
	return;
}

void main()
{
	float amp = pow(2.0, 1.0/12.0);
  	for(i=0 ; i<BUF_SIZE ; i++)
    	delay[i] = 0;
	
	for(i=0; i<N;i++) {
		vibsine[i] = sin(2*3.141592 * i/N); 
	}

	comm_intr();                  //init DSK, codec, McBSP
 	while(1) ;                  //infinite loop
}

