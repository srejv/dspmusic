//loop_buf.c loop program with storage
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input
#define BUFSIZE 512

int buffer[BUFSIZE];
int buf_ptr = 0;

interrupt void c_int11()          // interrupt service routine
{
  int sample_data;
  
  sample_data = input_left_sample();    // read input sample
  buffer[buf_ptr] = sample_data;        // store in buffer
  if(++buf_ptr >= BUFSIZE) buf_ptr = 0; // update buffer index
  output_left_sample(sample_data);      // write output sample
  return;
}

void main()
{
  comm_intr();                    // init DSK, codec, McBSP
  while(1);                       // infinite loop
}
