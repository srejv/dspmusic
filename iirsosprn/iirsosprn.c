// iirsosprn.c generic iir filter using cascaded second order sections
// input from PRBS generator function, output to line out
// float coefficients read from included .cof file
 
#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate

#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;

#include "elliptic.cof"

float w[NUM_SECTIONS][2] = {0};

#include "noise_gen.h"	            //support file for noise sequence generation 
int fb;                            //feedback variable
shift_reg sreg;                    //shift register 
#define NOISELEVEL 8000            //scale factor for +/- 1 noise sequence

int prand(void) 	                  //pseudo-random noise generation
{
  int prnseq;			            
  if(sreg.bt.b0)
    prnseq = -NOISELEVEL;          //scaled negative noise level
  else
    prnseq = NOISELEVEL;           //scaled positive noise level
  fb =(sreg.bt.b0)^(sreg.bt.b1);   //XOR bits 0,1
  fb^=(sreg.bt.b11)^(sreg.bt.b13); //with bits 11,13 -> fb
  sreg.regval<<=1;                 //shift register 1 bit to left
  sreg.bt.b0=fb;                   //close feedback path
  return prnseq;
}


void resetreg(void)
{
  sreg.regval = 0xFFFF;        //shift register to nominal values
  fb = 1;                      //initial feedback value
  return;
}


interrupt void c_int11()	 //interrupt service routine
{
  int section;   // index for section number
  float input;   // input to each section
  float wn,yn;   // intermediate and output values in each stage

  input =((float)prand());   // input to first section read from codec

  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    wn = input - a[section][0]*w[section][0] - a[section][1]*w[section][1];
    yn = b[section][0]*wn + b[section][1]*w[section][0] + b[section][2]*w[section][1];
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;              // output of current section will be input to next
  }

  output_left_sample((short)yn); // before writing to codec
  return;                       //return from ISR
}

void main()
{
  resetreg();
  comm_intr();                  //init DSK, codec, McBSP
  while(1);                     //infinite loop
}

