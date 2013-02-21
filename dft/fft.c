//dft.c N-point DFT of sequence read from lookup table

#include <stdio.h>
#include <math.h> 
#include "fft.h"

#define PI 3.14159265358979
#define N 128
#define TESTFREQ 800.0
#define SAMPLING_FREQ 8000.0


COMPLEX samples[N];
COMPLEX twiddle[N];


void main()
{    
  int n;
  
  for (n=0 ; n<N ; n++)         //set up DFT twiddle factors
  {
    twiddle[n].real = cos(PI*n/N);
    twiddle[n].imag = -sin(PI*n/N);
  }

  for(n=0 ; n<N ; n++)
  {
  samples[n].real = cos(2*PI*TESTFREQ*n/SAMPLING_FREQ);
  samples[n].imag = 0.0;
  }
  printf("real input data stored in array samples[]\n");
  printf("\n"); // place breakpoint here
  fft(samples,N,twiddle);              	   //call DFT function
  printf("done!\n");
}	
