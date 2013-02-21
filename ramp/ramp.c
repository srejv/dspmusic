//Ramp.c Generates a ramp

#include "dsk6416_aic23.h"	//codec-DSK support file
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling frequency
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select mic in
short output;
                      
interrupt void c_int11() //interrupt service routine       	
{   
 output_left_sample(output);	 //output for each sample period
 output += 2000;		 //incr output value
 if (output >= 30000)	 //if peak is reached
 output = -30000;             //reinitialize
 return;			 //return from interrupt
}

 void main()
{
 output = 0;		 //init output to zero
 comm_intr();            //init DSK, codec, McBSP
 while(1);               //infinite loop
}
