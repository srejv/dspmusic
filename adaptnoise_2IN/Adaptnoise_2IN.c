//Adaptnoise_2IN.c  Adaptive FIR for sinusoidal noise interference 

#include "DSK6416_AIC23.h"		//codec-DSK support file
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ; //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;
#define beta 1E-12        	  //rate of convergence     
#define N 30                 //# of weights (coefficients)   
#define LEFT 0				  //left channel
#define RIGHT 1			        //right channel
float w[N];                  //weights for adapt filter
float delay[N];                //input buffer to adapt filter  
short output;		          //overall output 
short out_type = 1;            //output type for slider 
volatile union{unsigned int uint; short channel[2];}AIC23_data;

interrupt void c_int11()      	  //ISR
 {                         
  short i;
  float yn=0, E=0, dplusn=0, desired=0, noise=0;    
  
  AIC23_data.uint = input_sample(); //input 32-bit from both channels
  desired =(AIC23_data.channel[LEFT]);//input left channel
  noise = (AIC23_data.channel[RIGHT]); //input right channel
  
  dplusn = desired + noise;         //desired+noise
  delay[0] = noise;		        //noise as input to adapt FIR
    
  for (i = 0; i < N; i++)          //to calculate out of adapt FIR
	yn += (w[i] * delay[i]);       //output of adaptive filter
  E = (desired + noise) - yn;      //"error" signal=(d+n)-yn   
  for (i = N-1; i >= 0; i--)       //to update weights and delays
   {            
	w[i] = w[i] + beta*E*delay[i]; //update weights   
   	delay[i] = delay[i-1];       //update delay samples   
   }  
  if(out_type == 1) 				//if slider in position 1
  	output=((short)E);	//error signal as overall output  
  else if(out_type==2)  //if slider in position 2
  	output=((short)dplusn);//output (desired+noise)
  output_left_sample(output);	    	//overall output result	
  return;
}

 void main()
{
  short T=0;                          
  for (T = 0; T < 30; T++)
   {
   	w[T] = 0;				  //init buffer for weights
   	delay[T] = 0;			  //init buffer for delay samples
   } 
  comm_intr();                 	  //init DSK, codec, McBSP
  while(1);                    	  //infinite loop
}
