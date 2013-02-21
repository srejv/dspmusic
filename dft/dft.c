//dft.c N-point DFT of sequence read from lookup table

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

void dft(COMPLEX *x)
{
  COMPLEX result[N];
  int k,n;

  for (k=0 ; k<N ; k++)
  {
    result[k].real=0.0;
    result[k].imag = 0.0;

    for (n=0 ; n<N ; n++)
    {
      result[k].real += x[n].real*cos(2*PI*k*n/N) + x[n].imag*sin(2*PI*k*n/N);
      result[k].imag += x[n].imag*cos(2*PI*k*n/N) - x[n].real*sin(2*PI*k*n/N);
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
  samples[n].real = cos(2*PI*TESTFREQ*n/SAMPLING_FREQ);
  samples[n].imag = 0.0;
  }
  printf("real input data stored in array samples[]\n");
  printf("\n"); // place breakpoint here
  dft(samples);          //call DFT function
  printf("done!\n");
}	
