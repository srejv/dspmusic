//sine_stereo Sine generation to both LEFT and RIGHT channels

#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#define LEFT  0
#define RIGHT 1
union {Uint32 uint; short channel[2];} AIC23_data; 

#define LOOPLENGTH 8        // size of look up table
short sine_table_left[LOOPLENGTH]={0,7071,10000,7071,0,-7071,-10000,-7071};
short sine_table_right[LOOPLENGTH]={0,-7071,10000,-7071,0,7071,-10000,7071};
short loopindex = 0;   	    // look up table index

interrupt void c_int11()	//interrupt service routine 
{  
	AIC23_data.channel[RIGHT]=sine_table_right[loopindex]; //for right channel;
	AIC23_data.channel[LEFT]=sine_table_left[loopindex]; //for leftchannel;

	output_sample(AIC23_data.uint); //output to both channels 
    if (++loopindex >= LOOPLENGTH) 
      loopindex = 0; // check for end of look up table
    return;
} 

void main()					
{   
  	comm_intr();               //init DSK,codec,McBSP 
 	while(1) ;					//infinite loop
}            
 
