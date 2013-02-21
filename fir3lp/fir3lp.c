//Fir3LP.c FIR using 3 lowpass coefficients with three different BW

#include "lp600.cof"		 	    //coeff file LP @ 600 Hz 
#include "lp1500.cof"			    //coeff file LP @ 1500 Hz 
#include "lp3000.cof"			    //coeff file LP @ 3000 Hz
#include "dsk6416_aic23.h"			//codec-dsk support file
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select MIC IN
short LP_number = 0; 		        //start with 1st LP filter
int yn = 0;					 	    //initialize filter's output
short dly[N];            		    //delay samples
short h[3][N];  	    			//filter characteristics 3xN

interrupt void c_int11()		    //ISR
{
	short i;
 
	dly[0] = input_left_sample();     	 //newest input @ top of buffer
	yn = 0;                          //initialize filter output
	for (i = 0; i< N; i++)
	   yn +=(h[LP_number][i]*dly[i]);//y(n) += h(LP#,i)*x(n-i)
	for (i = N-1; i > 0; i--)        //starting @ bottom of buffer
	   dly[i] = dly[i-1];            //update delays with data move
 	output_left_sample(yn >> 15);	     //output filter
 	return;				   			 //return from interrupt
}

void main()
{
 	short i;
       	for (i=0; i<N; i++)
         { 
		 dly[i] = 0;                //init buffer
  		h[0][i] = hlp600[i];  	    //start addr of LP600 coeff
   		h[1][i] = hlp1500[i];  	    //start addr of LP1500 coeff
   		h[2][i] = hlp3000[i]; 	    //start addr of LP3000 coeff
         }
    comm_intr();                    //init DSK, codec, McBSP
    while(1);    		   	    	//infinite loop
}

