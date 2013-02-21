// iirsosdelta.c generic iir filter using cascaded second order sections
// input internally generated delta sequence, output to line out
// and save in buffer
// float coefficients read from included .cof file
 
#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate

#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;
#define BUFSIZE 256
#define AMPLITUDE 20000
#include "elliptic.cof"

float w[NUM_SECTIONS][2] = {0};

float dimpulse[BUFSIZE];
float response[BUFSIZE];
int index = 0;

interrupt void c_int11()	 //interrupt service routine
{
  int section;   // index for section number
  float input;   // input to each section
  float wn,yn;   // intermediate and output values in each stage

  input = dimpulse[index];   // input to first section read from impulse sequence

  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    wn = input - a[section][0]*w[section][0] - a[section][1]*w[section][1];
    yn = b[section][0]*wn + b[section][1]*w[section][0] + b[section][2]*w[section][1];
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;              // output of current section will be input to next
  }

  response[index++] = yn;
  if (index >= BUFSIZE) index = 0;

  output_left_sample((short)(-yn*AMPLITUDE)); // before writing to codec

  return;                       //return from ISR
}

void main()
{
  int i;
  
  for (i=0 ; i< BUFSIZE ; i++) dimpulse[i] = 0.0;
  dimpulse[0] = 1.0;
  comm_intr();                  //init DSK, codec, McBSP
  while(1);                     //infinite loop
}

