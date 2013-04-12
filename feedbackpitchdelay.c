//echo.c echo with fixed delay and feedback
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_48KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input
#include <math.h>

#define GAIN 0.8                // fraction (0 - 1) of output fed back
#define INPGAIN 10
#define BUF_SIZE 48000           // this sets length of delay
#define DELAY_SIZE 1024

short buffer[BUF_SIZE];         // storage for previous samples
short input,output,delayed;
int i;                          // index into buffer

float readoffset = 0.0;
float pitch = 2.;
float inc = 1.5;

interrupt void c_int11()	    // interrupt service routine
{
  input = input_left_sample();  // read new input sample from ADC 
  delayed = buffer[(int)readoffset];          // read delayed value from buffer
  output = input + delayed;     // output sum of input and delayed values
  output_left_sample(output);
  //output_right_sample(output);     
  buffer[i] = input + delayed*GAIN; // store new input and a fraction 
                                // of the delayed value in buffer
  if(++i >= BUF_SIZE) i=0;      // test for end of buffer
  readoffset += inc;
  if(readoffset >= BUF_SIZE) readoffset = 0.0;
  return;                       // return from ISR
}

void main()
{
  comm_intr();                  // init DSK, codec, McBSP
  for(i=0 ; i<BUF_SIZE ; i++)   // clear buffer
    buffer[i] = 0;
  while(1) {
  	inc = pow(2.0, pitch/12);
  }                     //infinite loop
}

