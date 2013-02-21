//Adaptnoise.c  Adaptive FIR filter for noise cancellation 

#include "DSK6416_AIC23.h"
Uint32 fs= DSK6416_AIC23_FREQ_8KHZ;
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;
  
#include "refnoise.h" 			//cosine 312 Hz
#include "dplusn.h"           	//sin(1500) + sin(312)                
#define beta 1E-9        		//rate of convergence      
#define N 30                  	//# of weights (coefficients)
#define NS 128                	//# of output sample points 
float w[N];                  	      //buffer weights of adapt filter
float delay[N];              	      //input buffer to adapt filter
short output;		            //overall output 
short out_type = 1;         	      //output type for slider 

interrupt void c_int11()      	//ISR
{                         
 short i;
 static short buffercount=0;		//init count of # out samples
 float yn, E;               		//output filter/"error" signal

 delay[0] = refnoise[buffercount];  //cos(312Hz) input to adapt FIR
 yn = 0;                            //init output of adapt filter 
 for (i = 0; i < N; i++)            //to calculate out of adapt FIR
     yn += (w[i] * delay[i]);       //output of adaptive filter
  
 E = dplusn[buffercount] - yn;      //"error" signal=(d+n)-yn   
          			
 for (i = N-1; i >= 0; i--)         //to update weights and delays
   {            
     w[i] = w[i] + beta*E*delay[i]; //update weights   
     delay[i] = delay[i-1];         //update delay samples   
   } 
 buffercount++;                     //increment buffer count
 if (buffercount >= NS) 	      //if buffercount=# out samples                                                                                                                                 
     buffercount = 0;            	//reinit count
      
 if (out_type == 1)		     	//if slider in position 1				
     output = ((short)E*10);        //"error" signal overall output  
 else if (out_type == 2)      	  //if slider in position 2
     output=dplusn[buffercount]*10; //desired(1500)+noise(312)
 output_left_sample(output);	            //overall output result	
 return;					//return from ISR
}

void main()
{
 short T=0;                          
 for (T = 0; T < 30; T++)
   {
     w[T] = 0;			      //init buffer for weights
     delay[T] = 0;		      //init buffer for delay samples
   } 
 comm_intr();                       //init DSK, codec, McBSP
 while(1);                          //infinite loop
}
