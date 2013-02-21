// program average.c
#include "DSK6416_AIC23.h"	              // codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	      //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input

#define N 5  // number of points averaged
float x[N]; //filter input delay line
float h[N] = {0.0833, 0.25, 0.3333, 0.25, 0.0833}; //filter coefficients

interrupt void c_int11() //ISR - AIC23 codec interrupts at 8kHz
{
  short i;
  float yn = 0.0;
  
  x[0] = (float)(input_left_sample()); //get new input into delay line
  for (i=0 ; i<N ; i++) //calculate filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--) //shuffle delay line contents
    x[i] = x[i-1]; 
  output_left_sample((short)(yn)); //output to codec
  return;
}

void main() //main body of program does nothing
{
  short i; //index variable
  for (i=0 ; i<N ; i++) //initialise filter coefficients
    h[i] = 1.0/N;
  comm_intr(); //initialise DSK
  while(1); //infinite loop
}
