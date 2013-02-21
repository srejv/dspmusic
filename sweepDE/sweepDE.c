//sweepDE.c generates sweeping sinusoid using difference equations 

#include "DSK6416_AIC23.h"		//codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select mic in
#include <math.h>
#define MIN_FREQ 200
#define MAX_FREQ 3800
#define STEP_FREQ 20
#define SWEEP_PERIOD 400
#define SAMPLING_FREQ 8000
#define AMPLITUDE 5000
#define PI 3.14159265358979

float y[3] = {0.0, 0.0, 0.0};
float a1;
float freq = MIN_FREQ;
short sweep_count = 0;

void coeff_gen(float freq)
{
  a1 = 2.0*cos(2.0*PI*freq/SAMPLING_FREQ);
  y[0] = 0.0;
  y[1] = sin(2.0*PI*freq/SAMPLING_FREQ);
  y[2] = 0.0;
  return;
}

interrupt void c_int11()            //ISR
{
  sweep_count++;
  if (sweep_count >= SWEEP_PERIOD)
  {
    if (freq >= MAX_FREQ) freq = MIN_FREQ;
    else freq += STEP_FREQ;
    
    coeff_gen(freq);
    sweep_count = 0;
    }
    
      y[0] =(y[1]*a1)-y[2];
 y[2] = y[1];                             //update y1(n-2)
 y[1] = y[0];                             //update y1(n-1)
 output_left_sample((short)(y[0]*AMPLITUDE));             //output result
 return;					//return to main
}

void main()
{
y[1] = sin(2.0*PI*freq/SAMPLING_FREQ);
a1 = 2.0*cos(2.0*PI*freq/SAMPLING_FREQ);
 comm_intr();                  		  //init DSK, codec, McBSP
 while(1);                     		  //infinite loop
}
