// sysid.c
#include "DSK6416_AIC23.h"	              // codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	        //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select line in

#include "noise_gen.h"		       //support file for noise generation 
#define beta 1E-12          	   //learning rate
#define WLENGTH 256
#define NOISELEVEL 8000

float w[WLENGTH+1];		           //buffer coeff for adaptive FIR
float dly_adapt[WLENGTH+1];	       //buffer samples of adaptive FIR
int fb;                            //feedback variable
shift_reg sreg;                    //shift register

int prand(void) 			       //gen pseudo-random sequence {-1,1}
{
  int prnseq;			 
  if(sreg.bt.b0)
	prnseq = -NOISELEVEL;		           //scaled negative noise level
  else
	prnseq = NOISELEVEL;		           //scaled positive noise level
  fb =(sreg.bt.b0)^(sreg.bt.b1);   //XOR bits 0,1
  fb^=(sreg.bt.b11)^(sreg.bt.b13); //with bits 11,13 -> fb
  sreg.regval<<=1;
  sreg.bt.b0=fb;			       //close feedback path
  return prnseq;			       //return noise sequence value 
}
 
interrupt void c_int11()               //interrupt service routine
{                         
 int i;
 float adaptfir_out = 0.0;             //initialise adaptive filter output
 float fir_out = 0.0;
 float E;                              //error=difference of fixed/adapt out

 fir_out = (float)(input_left_sample());    //read output from external system
 dly_adapt[0]=(float)(prand());          	   //pseudo-random noise sample used as 
 output_left_sample((short)(dly_adapt[0])); //input to adaptive filter and output
                                       //to external system
 for (i = 0; i < WLENGTH; i++)
   adaptfir_out +=(w[i]*dly_adapt[i]); //compute adaptive filter output 
 
 E = fir_out - adaptfir_out;           //error signal           
 
 for (i = WLENGTH-1; i >= 0; i--)         
  {
   w[i] = w[i]+(beta*E*dly_adapt[i]);  //update weights of adaptive FIR  
   dly_adapt[i+1] = dly_adapt[i];      //update samples of adaptive FIR   
  } 
 return;
}

void main()
{
 int i = 0;
 for (i = 0; i <= WLENGTH; i++)
  {
   w[i] = 0.0;       			   //init coeff for adaptive FIR
   dly_adapt[i] = 0.0;			   //init buffer for adaptive FIR
  }  
 
 sreg.regval=0xFFFF;	               //initial seed value
 fb = 1;			               //initial feevack value
 comm_intr(); 			         //init DSK, codec, McBSP
 while (1);   			         //infinite loop
}


