// iirsosadapt.c generic iir filter using cascaded second order sections
// characteristic identified experimentally using adaptive FIR filter
// float coefficients read from included .cof file
 
#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;

#include "noise_gen.h"
#include "elliptic.cof" // contains a and b coefficient values and defines
                        // NUM_SECTIONS

float w[NUM_SECTIONS][2] = {0};

#define beta 1.0E-11                    // learning rate
#define WLENGTH 256                    //# of coeff for adaptive FIR
float h[WLENGTH+1]={0.0};                    //buffer coeff for adaptive FIR
float dly_adapt[WLENGTH+1]={0.0};              //buffer samples of adaptive FIR

short fb;				     //feedback variable
shift_reg sreg;

short prn(void)			     //pseudorandom noise generation 
{
 short prnseq;                     //for pseudorandom sequence
 
 if(sreg.bt.b0)                    //sequence {1,-1}
	prnseq = -4000; 	           //scaled negative noise level
 else
	prnseq = 4000;	           //scaled positive noise level
 fb  =(sreg.bt.b0)^(sreg.bt.b1);   //XOR bits 0,1
 fb ^=(sreg.bt.b11)^(sreg.bt.b13); //with bits 11,13 ->fb
 sreg.regval<<=1;                  //shift register 1 bit to left
 sreg.bt.b0 = fb;	     	           //close feedback path
                          
 return prnseq;			     //return sequence
}

interrupt void c_int11()	 //interrupt service routine
{
  int section;   // index for section number
  float input;   // input to each section
  float wn,yn;   // intermediate and output values in each stage
  int i;
  float adaptfir_out=0.0;		         		//init output of adaptive FIR
  float E;				         			//error signal

  input = (float)(prn()); // input to first iir section is PRBS value
  dly_adapt[0] = input;   // copy input value to fir

  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    wn = input - a[section][0]*w[section][0] - a[section][1]*w[section][1];
    yn = b[section][0]*wn + b[section][1]*w[section][0] + b[section][2]*w[section][1];
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;              // output of current section will be input to next
  }

//  yn = yn * CODEC_ATTEN;    // finally, scale output by codec attenuation factor

  for (i = 0; i < WLENGTH; i++) adaptfir_out +=(h[i]*dly_adapt[i]); //output of adaptive FIR
  E = yn - adaptfir_out;  	   	//error as difference of outputs
  for (i = WLENGTH-1; i >= 0; i--)
  {
    h[i] = h[i]+(beta*E*dly_adapt[i]);  //update weights of adaptive FIR
    dly_adapt[i+1] = dly_adapt[i];      //update samples of adaptive FIR
  }

  output_left_sample((short)adaptfir_out); 

  return;                       //return from ISR
}

void main()
{
 int i=0;
 for (i = 0; i < WLENGTH; i++)
  {
   h[i] = 0.0;       			  			//init coeff of adaptive FIR
   dly_adapt[i] = 0.0;                 //init samples of adaptive FIR
  }
  sreg.regval = 0xFFFF;        //shift register to nominal values
  fb = 1;                      //initial feedback value
  comm_intr();                  //init DSK, codec, McBSP
  while(1);                     //infinite loop
}

