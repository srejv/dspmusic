// program scrambler.c
#include "DSK6416_AIC23.h"	              // codec support
Uint32 fs=DSK6416_AIC23_FREQ_16KHZ;	        //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select line in
#include "sine160.h" 
#include "lp3k64.cof"                //filter coefficient file
float yn1, yn2;                          //filter outputs
float x1[N],x2[N];                        //filter delay lines
int index = 0;

interrupt void c_int11()         
{  
 short i;
// first filter input
 x1[0] = (float)(input_left_sample()); //get new input into delay line
     yn1 = 0.0;                       //initialise filter output
 for (i=0 ; i<N ; i++) yn1 += h[i]*x1[i];
 for (i=(N-1) ; i>0 ; i--) x1[i] = x1[i-1];
// next mix with 3300Hz
 yn1 *= sine160[index++];
 if (index >= NSINE) index = 0;
// now filter again
 x2[0] = yn1; //get new input into delay line
 yn2 = 0.0;                       //initialise filter output
 for (i=0 ; i<N ; i++) yn2 += h[i]*x2[i];
 for (i=(N-1) ; i>0 ; i--) x2[i] = x2[i-1];  
 output_left_sample((short)(yn2));     //output to codec
 return;
}

void main()                      //main body of program does nothing
{
  comm_intr();                   //initialise McBSP, AD535
  while(1);                      //infinite loop
}

