//bios_sine8_intr.c DSP/BIOS application to generate sine wave
#include "DSK6416_AIC23.h"	            // codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	    //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select mic in

#define LOOPLENGTH 8        // size of look up table
short sine_table[LOOPLENGTH]={0,7071,10000,7071,0,-7071,-10000,-7071};
short loopindex = 0;   	    // look up table index

void c_int11(void)
{
  output_left_sample(sine_table[loopindex++]);
  if (loopindex >= LOOPLENGTH ) loopindex = 0;
  return;
}

void main()
{
  comm_intr();
}
