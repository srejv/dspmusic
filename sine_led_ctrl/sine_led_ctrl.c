//Sine_led_ctrl.c Sine generation with DIP Switch control

#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input
short sine_table[8]={0,707,1000,707,0,-707,-1000,-707};//sine values
short loop=0, gain=10;
short j=0, k = 0;											//delay counter			   
short flag = 0;                             //for LED on
short const delay = 800;					//for delay loop
short on_time = 1;							//led is on for on_time secs

void main()
{
  comm_poll();                            //init BSL
  DSK6416_LED_init();  					//init LEDs
  DSK6416_DIP_init();                      //init DIP SWs
  while(1)                                //infinite loop
  {
    if(DSK6416_DIP_get(0)==0 &&(k<=(on_time*5)))//if SW0 pressed
    {
	  if (flag==0) DSK6416_LED_toggle(0);			//LED0 toggles
      else	DSK6416_LED_off(0);						//turn LED0 off
      output_sample(sine_table[loop]*gain); 		//output with gain
	  if (loop < 7) loop++;                  //increment loop index
	  else	loop = 0;					//reinit index if end of table
	  if (j < delay) ++j;					//delay counter
      else
      {
   		j = 0;								//reset delay counter
        if (flag == 1)                         
        {
          flag = 0;                        //if flag=1 toggle LED
          k++;
        }
        else flag = 1;					//toggle flag
      }
    }
    else
    {
	  DSK6416_LED_off(0);					//turn off LED0
      if (DSK6416_DIP_get(0) == 1) k = 0;	//If LED0 off reset counter
    }
  }
}

