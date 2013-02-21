//am.c AM using table for carrier and baseband signals
 
#include "DSK6416_AIC23.h"	                // codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	        //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select mic in

short amp = 1;						//index for modulation


void main()
{

 short baseband[20]={1000,951,809,587,309,0,-309,-587,-809,-951,
	 -1000,-951,-809,-587,-309,0,309,587,809,951}; //400-Hz baseband
 short carrier[20] ={1000,0,-1000,0,1000,0,-1000,0,1000,0,
	 -1000,0,1000,0,-1000,0,1000,0,-1000,0}; //2-kHz carrier 
 short output[20];
 short k;

 comm_poll();                            //init DSK, codec, McBSP
 while(1)                                //infinite loop
  {
   for (k=0; k<20; k++)
    {
     output[k]= carrier[k] + ((amp*baseband[k]*carrier[k]/10)>>12);
     output_left_sample(20*output[k]);        //scale output
    }
  }
}

