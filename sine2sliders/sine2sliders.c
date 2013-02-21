//sine2sliders.c Sine generation with different # of points

#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input

short loop = 0;
short sine_table[32]={0,195,383,556,707,831,924,981,1000,
					 981,924,831,707,556,383,195,
					 0,-195,-383,-556,-707,-831,-924,-981,-1000,
                    -981,-924,-831,-707,-556,-383,-195};//sine data
short gain = 1;               	 	   //for slider
short frequency = 2;               	    //for slider

void main()
{
 comm_poll();                           //init DSK,codec,McBSP
 while(1)                               //infinite loop
  {
   output_left_sample(sine_table[loop]*gain);//output scaled value 
   loop += frequency;                   //incr frequency index 
   loop = loop % 32;                    //modulo 32 to reinit index 
  }
}

