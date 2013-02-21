//Sin1500MATL.c Generates sine from table created with MATLAB

#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input
#include "sin1500.h"	     //sin(1500) created with MATLAB
int i=0;

interrupt void c_int11()
{
 output_sample(sin1500[i]*10);  //output each sine value
 if (i < 127) ++i;           //incr index until end of table
    else i = 0;
 return;			     //return from interrupt
}
 
void main()
{  
 comm_intr();                //init DSK, codec, McBSP
 while(1);                   //infinite loop
}
