//sweep8000.c sweep sinusoid using table with 8000 points
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input
#include "sine8000_table.h" 	         //one cycle with 8000 points
#define SAMPLING_FREQ 8000.0
#define N 8000
#define START_FREQ 100.0
#define STOP_FREQ 3500.0
#define START_INCR START_FREQ*N/SAMPLING_FREQ
#define STOP_INCR STOP_FREQ*N/SAMPLING_FREQ
#define SWEEPTIME 5.0
#define DELTA_INCR (STOP_INCR - START_INCR)/(N*SWEEPTIME)


short amplitude = 10;                    //amplitude 
float float_index = 0.0;
float float_incr = START_INCR;
short i;

void main()
{
 comm_poll();                          //init DSK, codec, McBSP
 while(1)					   //infinite loop
  {
    float_incr += DELTA_INCR;
	if (float_incr > STOP_INCR) float_incr = START_INCR;
    float_index += float_incr;
    if (float_index > N) float_index -= N;
    i = (short)(float_index);
    output_left_sample(amplitude*sine8000[i]); //output 
  }
}
 
