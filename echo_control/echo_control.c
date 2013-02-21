//echo_control.c echo with variable delay and feedback
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#define MAX_BUF_SIZE 8000       // this sets maximum length of delay
float gain = 0.5;
short buflength = 1000;
short buffer[MAX_BUF_SIZE];     // storage for previous samples
short input,output,delayed;
int i = 0;                      // index into buffer

interrupt void c_int11()	    // interrupt service routine
{
  input = input_left_sample();  // read new input sample from ADC 
  delayed = buffer[i];          // read delayed value from buffer
  output = input + delayed;     // output sum of input and delayed values
  output_left_sample(output);     
  buffer[i] = input + delayed*gain; // store new input and a fraction 
                                // of the delayed value in buffer
  if(++i >= MAX_BUF_SIZE)       // test for end of buffer
    i = MAX_BUF_SIZE - buflength;
  return;                       // return from ISR
}

void main()
{
  for(i=0 ; i<MAX_BUF_SIZE ; i++)   // clear buffer
    buffer[i] = 0;
  comm_intr();                  // init DSK, codec, McBSP
  while(1);                     //infinite loop
}

