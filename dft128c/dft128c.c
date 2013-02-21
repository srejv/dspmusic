//dft128c.c
 
#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;

#include <math.h>
#define PI 3.14159265358979
#define TRIGGER 32000
#define N 128
#include "hamm128.h"

typedef struct
{
  float real;
  float imag;
} COMPLEX;

short buffercount = 0;            //number of new input samples in iobuffer 
short bufferfull = 0;                   //set by ISR to indicate iobuffer full
COMPLEX A[N], B[N], C[N];
COMPLEX *input_ptr, *output_ptr, *process_ptr, *temp_ptr;
COMPLEX twiddle[N];
short outbuffer[N];

void dft(COMPLEX *x, COMPLEX *w)
{
 COMPLEX result[N];
 int k,n;

  for (k=0 ; k<N ; k++)
  {
    result[k].real=0.0;
    result[k].imag = 0.0;

    for (n=0 ; n<N ; n++)
    {
      result[k].real += x[n].real*w[(n*k)%N].real - x[n].imag*w[(n*k)%N].imag;
      result[k].imag += x[n].imag*w[(n*k)%N].real + x[n].real*w[(n*k)%N].imag;
    }
  }
  for (k=0 ; k<N ; k++)
  {
    x[k] = result[k];
  }
}

interrupt void c_int11(void)      //ISR
{
  output_left_sample((short)((output_ptr + buffercount)->real));
  outbuffer[buffercount] = -(short)((output_ptr + buffercount)->real);
  (input_ptr + buffercount)->real = (float)(input_left_sample());
  (input_ptr + buffercount++)->imag = 0.0;
  if (buffercount >= N)       //for overlap-add method iobuffer
  {                             // is half size of FFT used
    buffercount = 0;
    bufferfull = 1;
  }
}

     
main()
{
  int n;
  
  for (n=0 ; n<N ; n++)         //set up DFT twiddle factors
  {
    twiddle[n].real = cos(2*PI*n/N);
    twiddle[n].imag = -sin(2*PI*n/N);
  }
  input_ptr = A;
  output_ptr = B;
  process_ptr = C;
  comm_intr();                    //initialise DSK, codec, McBSP
  while(1)                      //frame processing loop
  {
    while(bufferfull==0);    //wait for new frame of input samples
    bufferfull = 0;

    temp_ptr = process_ptr; //rotate buffer/frame pointers
    process_ptr = input_ptr;
    input_ptr = output_ptr;
    output_ptr = temp_ptr;

    dft(process_ptr,twiddle); //process contents of buffer

    for (n=0 ; n<N ; n++)       // compute magnitude of frequency domain representation
    {                           // and place in real part
      (process_ptr+n)->real = -sqrt((process_ptr+n)->real*(process_ptr+n)->real
                              + (process_ptr+n)->imag*(process_ptr+n)->imag)/16.0;
    }
    (process_ptr)->real = TRIGGER; // add oscilloscope trigger pluse
  }                               //end of while(1)
}                                 //end of main()
