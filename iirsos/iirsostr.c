// iirsos.c generic iir filter using cascaded second order sections
 
#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate

#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;
#include "bp2000.cof"

float w[NUM_SECTIONS][2] = {0};


interrupt void c_int11()	 //interrupt service routine
{
  int section;   // index for section number
  float input;   // input to each section
  float yn;   // intermediate and output values in each stage

  input = ((float)input_left_sample());
  
  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    yn = b[section][0]*input + w[section][0];
    w[section][0] = b[section][1]*input + w[section][1] - a[section][0]*yn;
    w[section][1] = b[section][2]*input - a[section][1]*yn;
    input = yn;              // output of current section will be input to next
  }

  output_left_sample((short)(yn)); // before writing to codec

  return;                       //return from ISR
}

void main()
{
  comm_intr();                  //init DSK, codec, McBSP
  while(1);                     //infinite loop
}

