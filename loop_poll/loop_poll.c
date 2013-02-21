//loop_poll.c loop program using polling
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

void main()
{
  short sample_data;

  comm_poll();	            //init DSK, codec, McBSP
  while(1)				    //infinite loop
  {
    sample_data = input_left_sample(); //input sample
    output_left_sample(sample_data);   //output sample
  }
}
 
