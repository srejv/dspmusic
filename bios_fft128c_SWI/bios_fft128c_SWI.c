//bios_fft128c_SWI.c FFT implementation calling a C-coded FFT function
//uses triple buffering for frame-based processing
//BIOS SWI version
#include <std.h>
#include <swi.h>

#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input source

#include <math.h>
#include "fft.h"
#define PI 3.14159265358979
#define TRIGGER 32000
#define N 128
 
short buffercount = 0;    // index into frames 
COMPLEX A[N], B[N], C[N];   //three buffers used 
COMPLEX twiddle[N];              //twiddle factors
COMPLEX *input_ptr, *output_ptr, *process_ptr, *temp_ptr; //pointers to frames
short ok=0;
short bufferfull=0;

extern far SWI_Obj SWI_process;

// attach to HWI
void c_int11(void)      //ISR
{
  output_left_sample((short)((output_ptr + buffercount)->real));
  (input_ptr + buffercount)->real = (float)(input_left_sample());
  (input_ptr + buffercount++)->imag = 0.0;
  if (buffercount >= N)       
  {                            
    SWI_post(&SWI_process);
    buffercount = 0;
  }
}


void process_buffer()
{
  int n;
    temp_ptr = process_ptr;  //rotate pointers to frames/buffers
    process_ptr = input_ptr;
    input_ptr = output_ptr;
    output_ptr = temp_ptr;

    fft(process_ptr,N,twiddle);           //transform samples into frequency domain

    for (n=0 ; n<N ; n++)       // compute magnitude of frequency domain representation
    {                           // and place in real part
      (process_ptr+n)->real = -sqrt((process_ptr+n)->real*(process_ptr+n)->real
                              + (process_ptr+n)->imag*(process_ptr+n)->imag)/16.0;
    }
    (process_ptr)->real = TRIGGER; // add oscilloscope trigger pluse
 
  return;
  }


     
void main()
{
  int n;
  for (n=0 ; n<N ; n++)         //set up twiddle factors in array w
  {
    twiddle[n].real = cos(PI*n/N);
    twiddle[n].imag = -sin(PI*n/N);
  }
  input_ptr = A;  //initialise pointers to frames/buffers
  process_ptr = B;
  output_ptr = C;
  comm_intr();


  return;                    //initialise DSK, codec, McBSP
}                                 //end of main()

