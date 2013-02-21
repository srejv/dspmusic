// frames.c - basic illustration of triple-buffered
// N sample frame-based processing
 
#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate

#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input source

#define N 128
 
short buffercount = 0;            //number of new input samples in buffer 
short bufferfull = 0;                   //set by ISR to indicate iobuffer full
float A[N], B[N], C[N]; 
float *input_ptr, *output_ptr, *process_ptr, *temp_ptr;

interrupt void c_int11(void)      //ISR
{
  output_left_sample((short)(*(output_ptr + buffercount)));
  *(input_ptr + buffercount++) = (float)(input_left_sample());
  if (buffercount >= N)       
  {                            
    buffercount = 0;
    bufferfull = 1;
  }
}

     
main()
{
  int i;

  input_ptr = A;
  output_ptr = B;
  process_ptr = C;
  
  comm_intr();                    //initialise DSK, codec, McBSP

  while(1)                        //frame processing loop
  {
    while(bufferfull==0); //wait until buffer is full 
    bufferfull = 0;
    
    temp_ptr = process_ptr;  //rotate pointers to frames/buffers
    process_ptr = input_ptr;
    input_ptr = output_ptr;
    output_ptr = temp_ptr;

	//process the contents of the frame/buffer pointed to by process_ptr
	for (i=0 ; i<N ; i++)
	{
	  *(process_ptr+i) *= 0.5;  // apply a scalar gain of 0.5
	}

  }                               //end of while(1)
}                                 //end of main()
