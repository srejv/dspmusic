//Fir4types.c  Four FIR filters: Lowpass, Highpass, bandpass, Bandstop

#include "DSK6416_AIC23.h"
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select line in
#include "lp1500.cof"		 		 //coeff file LP @ 1500 Hz 
#include "hp2200.cof"				 //coeff file HP @ 2200 Hz 
#include "bp1750.cof"				 //coeff file BP @ 1750 Hz 
#include "bs790.cof"				 //coeff file BS @  790 Hz
short FIR_number = 0; 		      	 //start with 1st LP filter
int yn = 0;					 		 //initialize filter's output
short dly[N];            		       //delay samples
short h[4][N];  	    			       //filter characteristics 3xN

interrupt void c_int11()			 //ISR
   {
	short i;
 
	dly[0] = input_left_sample();     		 //newest input @ top of buffer
	yn = 0;                          	 //initialize filter output
	for (i = 0; i< N; i++)
		yn +=(h[FIR_number][i]*dly[i]); //y(n) += h(LP#,i)*x(n-i)
	for (i = N-1; i > 0; i--)         	 //starting @ bottom of buffer
		dly[i] = dly[i-1];           	 //update delays with data move
 	output_left_sample(yn >> 15);		 //output filter
 	return;					 	//return from interrupt
   }

void main()
{
 	short i;
	for (i=0; i<N; i++)
         { 
		dly[i] = 0;             //init buffer
  		h[0][i] = hlp[i];  		//start addr of lp1500 coeff
   		h[1][i] = hhp[i];  		//start addr of hp2200 coeff
   		h[2][i] = hbp[i]; 		//start addr of bp1750 coeff
		h[3][i] = hbs[i];		//start addr of bs790 coeff
         }
    comm_intr();                //init DSK, codec, McBSP
    while(1);    		   		//infinite loop
}

