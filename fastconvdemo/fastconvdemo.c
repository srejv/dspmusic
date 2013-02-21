//fastconvdemo.c overlap-add convolution demonstration program

#include <math.h>
#include <stdio.h>
#include "lp55f.cof"               //time domain FIR filter coefficients
#define PI 3.14159265358979

#define PTS 128                   //number of points used in FFT
#define FREQHI 2666.67
#define FREQLO 156.25
#define SAMPLING_FREQ 8000

#include "fft.h" 

short buffercount = 0;            //number of new input samples in iobuffer 
COMPLEX twiddle[PTS];                   //twiddle factors stored in w
COMPLEX coeffs[PTS];              //zero padded freq domain filter coeffs
COMPLEX A[PTS], B[PTS], C[PTS];
short i;                          //general purpose index variable 
float a,b;                        //variables used in complex multiply
COMPLEX *input_ptr, *output_ptr, *temp_ptr, *process_ptr;
char in_buffer, proc_buffer, out_buffer, temp_buffer;
int wt = 0;


main()
{
input_ptr = A;
output_ptr = B;
process_ptr = C;
  in_buffer = 'A';    //initialise names of buffers, for diagnostic
  out_buffer = 'B';   //messages
  proc_buffer = 'C';

  for (i=0 ; i<PTS ; i++)         //set up twiddle factors in array w
  {
    twiddle[i].real = cos(PI*(i)/PTS);
    twiddle[i].imag = -sin(PI*(i)/PTS);
  }
  for (i=0 ; i<PTS ; i++)         //set up complex freq domain filter coeffs
  {
    coeffs[i].real = 0.0;
    coeffs[i].imag = 0.0;
  }
  for (i=0 ; i<N ; i++)        
  {
    coeffs[i].real = h[i];     
  }
  fft(coeffs,PTS,twiddle);              //transform filter coeffs to freq domain


for (i=0 ; i<PTS ; i++)        // zero all buffer contents
    {
    (output_ptr + i)->real = 0.0;
    (output_ptr + i)->imag = 0.0;
    (process_ptr + i)->real = 0.0;
    (process_ptr + i)->imag = 0.0;
    (input_ptr + i)->real = 0.0;
    (input_ptr + i)->imag = 0.0;
    }
while (1)
{
    for (i=0 ; i< PTS ; i++) (process_ptr + i)->imag = 0.0;
    for (i=PTS/2 ; i< PTS ; i++) (process_ptr + i)->real = 0.0;
  
    fft(process_ptr,PTS,twiddle);           //transform samples into frequency domain
    printf("frequency domain representation of zero padded input data");
    printf(" in process buffer (%c) \n", proc_buffer);
    printf("\n");                     //insert breakpoint

    for (i=0 ; i<PTS ; i++)       //filter frequency domain representation
    {                             //i.e. complex multiply samples by coeffs
      a = (process_ptr + i)->real;
      b = (process_ptr + i)->imag;
      (process_ptr + i)->real = coeffs[i].real*a - coeffs[i].imag*b;
      (process_ptr + i)->imag = -(coeffs[i].real*b + coeffs[i].imag*a);
    }
    printf("frequency domain result of multiplying by filter response");
    printf(" in process buffer (%c) \n", proc_buffer);
    printf("\n");                     //insert breakpoint
    fft(process_ptr,PTS,twiddle);
    for (i=0 ; i<PTS ; i++)
    {
      (process_ptr + i)->real /= PTS;
      (process_ptr + i)->imag /= -PTS; //if result is real, do we need this?
    }
    printf("time domain result of processing now");
    printf(" in process buffer (%c) \n", proc_buffer);
    printf("\n");                     //insert breakpoint

    for (i=0 ; i<PTS/2 ; i++)      //read new input into input buffer
    {
      (input_ptr + i)->real = (float)(sin(2*PI*wt*FREQLO/SAMPLING_FREQ))
                                     + 0.25*sin(2*PI*wt*FREQHI/SAMPLING_FREQ);
      wt++; 
    }

    printf("new input samples read into input buffer (%c)\n",in_buffer);
    printf("output written from first part of output buffer (%c)\n",out_buffer);
    printf("\n");                  //insert breakpoint here

    for (i=0 ; i<PTS/2 ; i++) //overlap add (real part only!!)   
    {
      (process_ptr + i)->real += (output_ptr + i + PTS/2)->real;
    }
	printf("second part of output buffer (%c) ", out_buffer);
	printf("has been added to first part of process buffer (%c)\n"
	, proc_buffer);
	printf("\n");                     //insert breakpoint here


    temp_ptr = process_ptr;  // rotate input, output and process buffers
    process_ptr = input_ptr;
    input_ptr = output_ptr;
    output_ptr = temp_ptr;
    temp_buffer = proc_buffer; //rotate names of buffer
	proc_buffer = in_buffer;
	in_buffer = out_buffer;
	out_buffer = temp_buffer;
	printf("buffer pointers rotated - for next section of input\n");
	printf("input buffer is (%c), process buffer is (%c)", in_buffer, proc_buffer);
	printf(", output buffer is (%c)\n", out_buffer);
	printf("\n");
  }   // end of while(1)
}   //end of main()

