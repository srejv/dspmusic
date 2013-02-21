//loop_intr.c loop program using interrupts
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

interrupt void c_int11()         //interrupt service routine
{
   short sample_data;

   sample_data = input_sample(); //input data
   output_sample(sample_data);   //output data
   return;
}

void main()
{
  comm_intr();                 //init DSK, codec, McBSP
  while(1);                	   //infinite loop
}
 
