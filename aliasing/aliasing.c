//aliasing.c illustration of downsampling, aliasing, upsampling

#include "DSK6416_AIC23.h"	     //codec support
Uint32 fs=DSK6416_AIC23_FREQ_16KHZ;  //set sampling rate
#include "lp6545.cof"	             //filter coefficients

#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input

#define DISCARD 0
#define SAMPLE 1

short flag = DISCARD;             //toggles for 2x down-sampling
short indly[N],outdly[N];         //antialias and reconst delay lines
float yn; int i;                  //filter output, index
short antialiasing = 1;           //init for no antialiasing filter

interrupt void c_int11()	     	 //ISR
{
 indly[0]=(float)(input_left_sample());//new sample to antialias filter
 yn = 0.0;                        //initialize downsampled value
 if (flag == DISCARD) flag = SAMPLE; //don't discard at next sampling
 else
  {
   if (antialiasing == 1)         //if antialiasing filter desired
    {                             //compute downsampled value
     for (i = 0 ; i < N ; i++)    //using LP filter coeffs
        yn += (h[i]*indly[i]);    //filter is implemented using float
    }
   else                           //if filter is bypassed
        yn = indly[0];            //downsampled value is input value
   flag = DISCARD;                //next input value will be discarded
  }
 for (i = N-1; i > 0; i--)
 		indly[i] = indly[i-1];	  //update input buffer

 outdly[0] = (yn); 		     	  //input to reconst filter
 yn = 0.0;                        //8 kHz sample values and zeros
 for (i = 0 ; i < N ; i++)        //are filtered at 16 kHz rate
      yn += (h[i]*outdly[i]);     //by reconstruction lowpass filter

 for (i = N-1; i > 0; i--)
      outdly[i] = outdly[i-1];    //update delays

 output_left_sample((short)yn);   //16 kHz rate sample
 return;                          //return from interrupt
}

void main()
{
 comm_intr();                     //init DSK, codec, McBSP
 while(1);                        //infinite loop
}

