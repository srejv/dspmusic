//timeconv.c

#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input source

#include "lp55f.cof"

#include <math.h>
#define PI 3.14159265358979
#define PTS 128
 
short buffercount = 0;    // index into frames 
short bufferfull=0;
float A[PTS], B[PTS], C[PTS];   //three buffers used 
float coeffs[PTS];              //zero padded freq domain filter coeffs
float *input_ptr, *output_ptr, *process_ptr, *temp_ptr; //pointers to frames
float result[PTS];             //temporary storage for convolution result

interrupt void c_int11(void)      //ISR
{
  output_left_sample((short)(*(output_ptr + buffercount)));
  *(input_ptr + buffercount++) = (float)(input_left_sample());
  if (buffercount >= PTS/2)       
  {                            
    bufferfull = 1;
    buffercount = 0;
  }
}

// convolution function - z = conv(x,y)
void conv(float *x, float *y, float *z, int n)
{
  int i, n_lo, n_hi;
  float *xp, *yp;

  for(i=0 ; i<(2*n - 1) ; i++)
  {
    *z = 0.0;
    n_lo = i - (n) + 1;
    if (n_lo < 0) n_lo = 0;
    n_hi = i;
    if (n_hi > (n - 1)) n_hi = (n - 1);
    for(xp = x + n_lo, yp = y + i - n_lo; xp <= x + n_hi; xp++, yp--) 
      *z += *xp * *yp;
    z++;
  }
  *z = 0.0; // fill final value in result array
}
    
void main()
{
  int i;

  for (i=0 ; i<PTS/2 ; i++) coeffs[i] = 0.0; //zero pad filter coeffs
  for (i=0 ; i<N ; i++) coeffs[i] = h[i];    //and store in array coeffs 

  input_ptr = A;      //initialise pointers
  output_ptr = B;
  process_ptr = C;

  comm_intr();


  while(1)                        //frame processing loop
  {
    while (bufferfull == 0);            //wait for iobuffer full
    bufferfull = 0;
    temp_ptr = process_ptr;
    process_ptr = input_ptr;
    input_ptr = output_ptr;
    output_ptr = temp_ptr;

    conv(process_ptr,coeffs,result,PTS/2); //convolve contents of process buffer
    for (i=0 ; i<PTS ; i++) *(process_ptr+i)=*(result+i); //with filter coeffs
    for (i=0 ; i<PTS/2 ; i++)      //add overlapping output sections
    {                              //in process buffer
      *(process_ptr + i) += *(output_ptr + i + PTS/2);
    }

  } // end of while
}                                 //end of main()

