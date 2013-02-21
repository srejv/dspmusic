//record.c record/play input using external memory

#include "dsk6416_aic23.h"				//codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;		//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input
#define N	2400000                    //large buffer size 300 secs
long i;
short buffer[N];
#pragma DATA_SECTION(buffer,".EXT_RAM") //buffer ->external memory

void main()
{
  short recording = 0;
  short playing = 0;
  for (i=0 ; i<N ; i++) buffer[i] = 0;
  DSK6416_DIP_init();
  DSK6416_LED_init();
  comm_poll();                   		//init DSK, codec, McBSP
  while(1)                           	//infinite loop
  {
  	if(DSK6416_DIP_get(3) == 0)      	//if SW#3 is pressed
    {
      i=0;
      recording = 1;
      while (recording == 1)
      {
        DSK6416_LED_on(3);           		//turn on LED#3
        buffer[i++] = input_left_sample(); 		//input data
  	    if (i>2000)
  	      if (DSK6416_DIP_get(3) == 1)
  	      {
  	        recording = 0;
  	        DSK6416_LED_off(3);
  	      }            	//LED#3 off when buffer full
    
      } // while (recording == 1)
    } //if 
  	if(DSK6416_DIP_get(0)==0) //if SW#0 pressed
    {
      i=0;
      playing = 1;
      while (playing == 1)
      {
        DSK6416_LED_on(0);             	//turn on LED#0
        output_left_sample(buffer[i++]); 		//input data
  	    if (i>2000)
  	      if (DSK6416_DIP_get(0) == 1)
  	      {
  	        playing = 0;
  	        DSK6416_LED_off(0);
  	      }            	//LED#3 off when buffer full
    
      } // while (playing == 1)
    } 
  }
}
 
