//bios_fastconv_TSK.c

#include <std.h>
#include <tsk.h>

#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input source

#include "lp55.cof"

#include <math.h>
#include "fft.h"
#define PI 3.14159265358979
#define TRIGGER 32000
#define PTS 128
 
short buffercount = 0;    // index into frames 
COMPLEX A[PTS], B[PTS], C[PTS];   //three buffers used 
COMPLEX twiddle[PTS];              //twiddle factors
COMPLEX coeffs[PTS];              //zero padded freq domain filter coeffs
COMPLEX *input_ptr, *output_ptr, *process_ptr, *temp_ptr; //pointers to frames
short bufferfull=0;
float a,b;                        //variables used in complex multiply

void process_buffer()
{
  int n,i;
  for (n=0 ; n<PTS ; n++)         //set up twiddle factors in array w
  {
    twiddle[n].real = cos(PI*n/PTS);
    twiddle[n].imag = -sin(PI*n/PTS);
  }
  for (n=0 ; n<PTS ; n++)         //set up complex freq domain filter coeffs
  {
    coeffs[n].real = 0.0;
    coeffs[n].imag = 0.0;
  }
  for (n=0 ; n<N ; n++)        
  {
    coeffs[n].real = h[n];     
  }
  fft(coeffs,PTS,twiddle);              //transform filter coeffs to freq domain
  input_ptr = A;  //initialise pointers to frames/buffers
  process_ptr = B;
  output_ptr = C;
  comm_intr();


  while(1)                        //frame processing loop
  {
    while (bufferfull == 0);            //wait for iobuffer full
    bufferfull = 0;
    temp_ptr = process_ptr;
    process_ptr = input_ptr;
    input_ptr = output_ptr;
    output_ptr = temp_ptr;

    for (i=0 ; i< PTS ; i++) (process_ptr + i)->imag = 0.0;
    for (i=PTS/2 ; i< PTS ; i++) (process_ptr + i)->real = 0.0;
  
    fft(process_ptr,PTS,twiddle);           //transform samples into frequency domain

    for (i=0 ; i<PTS ; i++)       //filter frequency domain representation
    {                             //i.e. complex multiply samples by coeffs
      a = (process_ptr + i)->real;
      b = (process_ptr + i)->imag;
      (process_ptr + i)->real = coeffs[i].real*a - coeffs[i].imag*b;
      (process_ptr + i)->imag = -(coeffs[i].real*b + coeffs[i].imag*a);
    }
    fft(process_ptr,PTS,twiddle);
    for (i=0 ; i<PTS ; i++)
    {
      (process_ptr + i)->real /= PTS;
      (process_ptr + i)->imag /= -PTS; //if result is real, do we need this?
    }
    for (i=0 ; i<PTS/2 ; i++) //overlap add (real part only!!)   
    {
      (process_ptr + i)->real += (output_ptr + i + PTS/2)->real;
    }

  } // end of while
}

// attach to HWI
void c_int11(void)      //ISR
{
  output_left_sample((short)((output_ptr + buffercount)->real));
  (input_ptr + buffercount)->real = (float)(input_left_sample());
  (input_ptr + buffercount++)->imag = 0.0;
  if (buffercount >= PTS/2)       
  {                            
    bufferfull = 1;
    buffercount = 0;
  }
}
     
void main()
{
return;
}                                 //end of main()

