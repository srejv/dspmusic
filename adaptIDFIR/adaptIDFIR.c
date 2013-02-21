//AdaptIDFIR.c Adaptive FIR for system ID of an FIR

#include "DSK6416_AIC23.h"		//codec-DSK support file
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	  //set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;
#include "bs55.cof"  		         //fixed FIR filter coefficients
#include "noise_gen.h"		         //support  noise generation file
#define beta 1E-12          	         //rate of convergence
#define WLENGTH 60                     //# of coefffor adaptive FIR
float w[WLENGTH+1];		         //buffer coeff for adaptive FIR
int dly_adapt[WLENGTH+1];	         //buffer samples of adaptive FIR
int dly_fix[N+1];  	               //buffer samples of fixed FIR
short out_type = 1; 		         //output for adaptive/fixed FIR
int fb;                                //feedback variable
shift_reg sreg;                        //shift register

int prand(void) 			         //pseudo-random sequence {-1,1}
{
  int prnseq;
  if(sreg.bt.b0)
	prnseq = -8000;		         //scaled negative noise level
  else
	prnseq = 8000;		         //scaled positive noise level
  fb =(sreg.bt.b0)^(sreg.bt.b1);       //XOR bits 0,1
  fb^=(sreg.bt.b11)^(sreg.bt.b13);     //with bits 11,13 -> fb
  sreg.regval<<=1;
  sreg.bt.b0=fb;			         //close feedback path
  return prnseq;			         //return noise sequence
}

interrupt void c_int11()               //ISR
{
 int i;
 int fir_out = 0;                      //init output of fixed FIR
 int adaptfir_out = 0;                 //init output of adapt FIR
 float E;                              //error=diff of fixed/adapt out

 dly_fix[0] = prand();  	         //input noise to fixed FIR
 dly_adapt[0]=dly_fix[0];          	   //as well as to adaptive FIR
 for (i = N-1; i>= 0; i--)
  {
   fir_out +=(h[i]*dly_fix[i]);        //fixed FIR filter output
   dly_fix[i+1] = dly_fix[i];          //update samples of fixed FIR
  }
 for (i = 0; i < WLENGTH; i++)
   adaptfir_out +=(w[i]*dly_adapt[i]); //adaptive FIR filter output

 E = fir_out - adaptfir_out;           //error signal

 for (i = WLENGTH-1; i >= 0; i--)
  {
   w[i] = w[i]+(beta*E*dly_adapt[i]);  //update weights of adaptive FIR
   dly_adapt[i+1] = dly_adapt[i];      //update samples of adaptive FIR
  }
 if (out_type == 1)		         //slider position for adapt FIR
   output_left_sample((short)adaptfir_out);  	   //output of adaptive FIR filter
 else if (out_type == 2)		   //slider position for fixed FIR
   output_left_sample((short)fir_out);             //output of fixed FIR filter
 else if (out_type == 3)		   //slider position for fixed FIR
   output_left_sample((short)E);             //output of fixed FIR filter
 return;
}

void main()
{
 int T=0, i=0;
 for (i = 0; i < WLENGTH; i++)
  {
   w[i] = 0.0;       			   //init coeff for adaptive FIR
   dly_adapt[i] = 0;			   //init buffer for adaptive FIR
  }
 for (T = 0; T < N; T++)
   dly_fix[T] = 0;			   //init buffer for fixed FIR
 sreg.regval=0xFFFF;	               //initial seed value
 fb = 1;			               //initial feevack value
 comm_intr(); 			         //init DSK, codec, McBSP
 while (1);   			         //infinite loop
}


