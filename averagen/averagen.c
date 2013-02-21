// program averagen.c
#include "DSK6416_AIC23.h"	              // codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	      //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input

#include "noise_gen.h" //support file for noise sequence generation
int fb;
shift_reg sreg;
#define NOISELEVEL 8000  //scale factor for noise sequence

#define N 5  // number of points averaged
float x[N]; //filter input delay line
float h[N]; //filter coefficients

int prand(void) //generates pseudo-random sequence {-1,1}
{
  int prnseq;
  if(sreg.bt.b0)
    prnseq = -NOISELEVEL; //scaled negative noise level
  else
    prnseq = NOISELEVEL; //scaled positive noise level
  fb =(sreg.bt.b0)^(sreg.bt.b1); //XOR bits 0,1
  fb^=(sreg.bt.b11)^(sreg.bt.b13); //with bits 11,13 -> fb
  sreg.regval<<=1;
  sreg.bt.b0=fb; //close feedback path
  return prnseq;
}

void resetreg(void)                //reset shift register to nominal values
{
  sreg.regval=0xFFFF;              //initial seed value
  fb = 1;                          //initial feedback value
}

interrupt void c_int11() //ISR - AIC23 codec interrupts at 8kHz
{
  short i;
  float yn = 0.0;
  
  x[0] = (float)(prand()); //get new input into delay line
  for (i=0 ; i<N ; i++) //calculate filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--) //shuffle delay line contents
    x[i] = x[i-1]; 
  output_left_sample((short)(yn)); //output to codec
  return;
}

void main() //main body of program does nothing
{
  short i; //index variable

  resetreg();
  for (i=0 ; i<N ; i++) //initialise filter coefficients
    h[i] = 1.0/N;
  comm_intr(); //initialise DSK
  while(1); //infinite loop
}
