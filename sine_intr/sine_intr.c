//sine_intr.c Sine generation using sin() function
#include <math.h>
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#define SAMPLING_FREQ 8000
#define PI 3.14159265358979

#include "qmath.h"
#include "wt.h"
/*
float frequency = 1000.0;
float amplitude = 10000.0;
float theta_increment;
float theta = 0.0;*/
WaveTable t;
fixedp tval;
int i;

interrupt void c_int11()
{
  //theta_increment = 2*PI*frequency/SAMPLING_FREQ;
  //theta += theta_increment;
  //if (theta > 2*PI) theta -= 2*PI;
  //output_left_sample((short)(amplitude*sin(theta)));
  
	WaveTable_doOscillate(&t, &tval);
  output_left_sample(tval);
  if(++i >= 1024) {
	i = 0;
  }
  return;
}

void main()
{
	WaveTable_generateTables();		
	i = 0;
	t.readPointer = 0;
	t.mInc = 0;
	t.unipolar = 0;
	t.invert = 0;
	t.table = SawtoothTable;
	
	WaveTable_cookFrequency(&t, short2q(5));
  comm_intr();
  while(1);
}
