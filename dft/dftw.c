//dftw.c N-point DFT of sequence read from lookup table
//using pre-computed twiddle factors

#include <stdio.h>
#include <math.h> 

#define PI 3.14159265358979
#define N 100
#define TESTFREQ 800.0
#define SAMPLING_FREQ 8000.0

typedef struct
{
  float real;
  float imag;
} COMPLEX;

COMPLEX samples[N];
COMPLEX twiddle[N];

void dftw(COMPLEX *x, COMPLEX *w)
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

void main()
{    
  int n;

  for(n=0 ; n<N ; n++)
  {
    twiddle[n].real = cos(2*PI*n/N);
    twiddle[n].imag = -sin(2*PI*n/N);
  }
  for(n=0 ; n<N ; n++)
  {
  samples[n].real = cos(2*PI*TESTFREQ*n/SAMPLING_FREQ);
  samples[n].imag = 0.0;
  }
  printf("real input data stored in array samples[]\n");
  printf("\n"); // place breakpoint here
  dftw(samples,twiddle);              	   //call DFT function
  printf("done!\n");
}	
