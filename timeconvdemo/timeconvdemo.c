//timeconvdemo.c overlap-add convolution demonstration program

#include <math.h>
#include <stdio.h>
#include "lp55f.cof"           //low pass FIR filter coefficients
#define PI 3.14159265358979

#define PTS 128                //frame size is PTS/2
 
float coeffs[PTS/2];           //zero-padded filter coeffs
float A[PTS], B[PTS], C[PTS];  //buffers
float *input_ptr, *output_ptr, *temp_ptr, *process_ptr;
float result[PTS];             //temporary storage for convolution result
char in_buffer, proc_buffer, out_buffer, temp_buffer;
short  i;
int wt = 0;

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


main()
{
  input_ptr = A;      //initialise pointers
  output_ptr = B;
  process_ptr = C;
  in_buffer = 'A';    //initialise names of buffers, for diagnostic
  out_buffer = 'B';   //messages
  proc_buffer = 'C';

  for (i=0 ; i<PTS/2 ; i++) coeffs[i] = 0.0; //zero pad filter coeffs
  for (i=0 ; i<N ; i++) coeffs[i] = h[i];    //and store in array coeffs 

  for (i=0 ; i<PTS ; i++)        // zero all buffer contents
  {
    *(output_ptr + i) = 0.0;
    *(process_ptr + i) = 0.0;
    *(input_ptr + i) = 0.0;
  }
  while (1)           //loop forever
  {
    conv(process_ptr,coeffs,result,PTS/2); //convolve contents of process buffer
    for (i=0 ; i<PTS ; i++) *(process_ptr+i)=*(result+i); //with filter coeffs
    
    for (i=0 ; i<PTS/2 ; i++)      //read new input into input buffer
    {
      *(input_ptr + i) = (float)(sin(2*PI*wt/50)) + 0.25*sin(2*PI*wt/3);
      wt++; 
    }
    printf("convolution completed in process buffer (%c)\n",proc_buffer);
    printf("new input samples read into input buffer (%c)\n",in_buffer);
    printf("output written from first part of output buffer (%c)\n",out_buffer);
    printf("\n");                  //insert breakpoint here
    for (i=0 ; i<PTS/2 ; i++)      //add overlapping output sections
    {                              //in process buffer
      *(process_ptr + i) += *(output_ptr + i + PTS/2);
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
  } // end of while(1)
}                                 //end of main()

