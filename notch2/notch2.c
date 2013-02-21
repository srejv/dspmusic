//Notch2.C Two FIR notch filters to remove sinusoidal noise signals

#include "DSK6416_AIC23.h"	//codec-DSK support file
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select line in
#include "bs900.cof"		      //BS @ 900 Hz coefficient file 
#include "bs2700.cof"               //BS @ 2700 Hz coefficient file
short dly1[N]={0};                  //delay samples for 1st filter
short dly2[N]={0};		      //delay samples for 2nd filter   
int y1out = 0, y2out = 0;		//init output of each filter
short out_type = 2;			//slider for output type

interrupt void c_int11()	      //ISR
{
	short i;

   	dly1[0] = input_left_sample();     //newest input @ top of buffer
   	y1out = 0;                    //init output of 1st filter
   	y2out = 0;                    //init output of 2nd filter
     	for (i = 0; i< N; i++)	     	
         y1out += h900[i]*dly1[i];  //y1(n)+=h900(i)*x(n-i)  
      
	dly2[0]=(y1out>>15);         //out of 1st filter->in 2nd filter
	for (i = 0; i< N; i++)
	   y2out += h2700[i]*dly2[i]; //y2(n)+=h2700(i)*x(n-i)
  
   	for (i = N-1; i > 0; i--)     //from bottom of buffer
     	 {
	   dly1[i] = dly1[i-1];       //update samples of 1st buffer
         dly2[i] = dly2[i-1];       //update samples of 2nd buffer
       }
   	if (out_type==1)            	//if slider is in position 1
      output_left_sample((short)(y2out>>15));       //corrupted input(voice+sines)
	if (out_type==2)        
      output_left_sample((short)(y1out>>15));	//output of 2nd filter (voice)
      return;				//return from ISR
}

void main()
{
  	comm_intr();                  //init DSK, codec, McBSP
	while(1);                     //infinite loop
}

