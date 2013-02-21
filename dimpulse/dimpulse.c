//dimpulse.c generate a sequence of impulses
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input
#define LOOPLENGTH 64								
short impulse_table[LOOPLENGTH] =  //lookup table
{-10000, 0, 0, 0, 0, 0, 0, 0,      //contains one non-zero
0, 0, 0, 0, 0, 0, 0, 0,            //value in 64
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0};
short loopindex = 0;

interrupt void c_int11()
{
  output_sample(impulse_table[loopindex++]);
  if (loopindex >= LOOPLENGTH)
    loopindex = 0;
  return;										
}

void main()
{
  comm_intr();
  while(1);    
}
