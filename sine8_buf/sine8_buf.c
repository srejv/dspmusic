//sine8_buf.c sine generation with output stored in buffer
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input
#define LOOPLENGTH 8
#define BUFFERLENGTH 256
int loopindex = 0;   						//table index
int bufindex = 0;                           //buffer index
short sine_table[LOOPLENGTH]={0,707,1000,707,0,-707,-1000,-707};
int out_buffer[BUFFERLENGTH];               //output buffer
short gain = 10;

interrupt void c_int11()                    //interrupt service routine
{
  short out_sample;
  
  out_sample = sine_table[loopindex++]*gain;
  output_left_sample(out_sample);	       //output sample value
  out_buffer[bufindex++] = out_sample;     //store in buffer
  if (loopindex >= LOOPLENGTH) loopindex = 0; //check for end of table
  if (bufindex >= BUFFERLENGTH) bufindex = 0; //check for end of buffer
  return;					               //return from interrupt
}

void main()
{
  comm_intr();	//initialise DSK
  while(1);     //infinite loop
}
 
