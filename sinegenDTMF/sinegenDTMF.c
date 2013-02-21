//sinegenDE.c generates sinusoid using difference equations 

#include "DSK6416_AIC23.h"		//codec-DSK support file
#include <math.h>
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select mic in
#define FREQLO 770
#define FREQHI 1336
#define SAMPLING_FREQ 8000
#define AMPLITUDE 5000
#define BUFSIZE 256
#define PI 3.14159265358979

float ylo[3] = {0.0, 0.0, 0.0};
float yhi[3] = {0.0, 0.0, 0.0};
float a1lo, a1hi;
float out_buffer[BUFSIZE];
int bufindex = 0;
float output;
float DIP0pressed = 0;

interrupt void c_int11()            //ISR
{
  ylo[0] =(ylo[1]*a1lo)-ylo[2];
 ylo[2] = ylo[1];                             //update y1(n-2)
 ylo[1] = ylo[0];                             //update y1(n-1)
  yhi[0] =(yhi[1]*a1hi)-yhi[2];
 yhi[2] = yhi[1];                             //update y1(n-2)
 yhi[1] = yhi[0];                             //update y1(n-1)
  output = (yhi[0]+ylo[0])*AMPLITUDE;
   out_buffer[bufindex++] = output;
 if (bufindex >= BUFSIZE) bufindex = 0;
if (DIP0pressed)  output_left_sample((short)(output));
else output_left_sample(0);             //output result
 return;					//return to main
}

void main()
{
ylo[1] = sin(2.0*PI*FREQLO/SAMPLING_FREQ);
a1lo = 2.0*cos(2.0*PI*FREQLO/SAMPLING_FREQ);
yhi[1] = sin(2.0*PI*FREQHI/SAMPLING_FREQ);
a1hi = 2.0*cos(2.0*PI*FREQHI/SAMPLING_FREQ);
DSK6416_DIP_init();
 comm_intr();                  		  //init DSK, codec, McBSP
 while(1)                     		  //infinite loop
{
  if (DSK6416_DIP_get(0) == 0) DIP0pressed = 1;
  else DIP0pressed = 0;
}
}
