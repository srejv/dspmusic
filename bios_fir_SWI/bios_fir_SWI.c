//bios_fir_SWI.c
#include <std.h>
#include <swi.h>
#include "DSK6416_AIC23.h"	              // codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	        //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select line in


#include "lp33.cof"                //filter coefficient file
float yn;                          //filter output
float x[N];                        //filter delay line

extern far SWI_Obj SWI_fir_isr;


void c_int11()         //ISR - AD535 codec interrupts at 8kHz
{  
  x[0] = (float)(input_left_sample()); //get new input into delay line
  output_left_sample((short)(yn));     //output to codec
  SWI_post(&SWI_fir_isr);
  return;
}

void fir_isr(void)
{
  short i;
    yn = 0.0;                       //initialise filter output
  for (i=0 ; i<N ; i++)           //calculate filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--)       //shuffle delay line contents
    x[i] = x[i-1];                //I know it's inefficient
  return;
}

void main()                      //main body of program does nothing
{
  comm_intr();                   //initialise McBSP, AD535
}

