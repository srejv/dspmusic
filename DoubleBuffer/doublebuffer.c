#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_96KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;


#include "dsp_fft.h"
#include "twiddle16x16n1024.inc"



#define BUF_SIZE 2048
short A[BUF_SIZE], B[BUF_SIZE], C[BUF_SIZE];
short D[BUF_SIZE];

short index = 0;
short leftInput;
short *input_ptr, *output_ptr, *process_ptr, *temp_ptr;
short *fft_out;

short filledBufferFlag = 0;
short outbuffer[BUF_SIZE];

void processDoubleBuffer() {
	int i;
	for(i = 0; i < BUF_SIZE; i++) {
		// Process
		process_ptr[i] = process_ptr[i];
	}
}

interrupt void c_int11() {
	output_left_sample(output_ptr[index]);
	outbuffer[index] = output_ptr[index];
	input_ptr[index++] = input_left_sample();
	input_ptr[index++] = 0;

	if (index >= BUF_SIZE)
	{
		index = 0;
		filledBufferFlag = 1;
	}
}

void main() {
	int i;
	// Set all to zero
	for(i = 0; i < BUF_SIZE; i++) {
		A[i] = 0;
		B[i] = 0;
		C[i] = 0;
		D[i] = 0;
		outbuffer[i] = 0;
	}

	input_ptr = A;
	output_ptr = B;
	process_ptr = C;
	fft_out = D;

	comm_intr();
	while(1) {
		while(filledBufferFlag == 0) ;
		filledBufferFlag = 0;

		temp_ptr = process_ptr;
		process_ptr = input_ptr;
		input_ptr = output_ptr;
		output_ptr = temp_ptr;
		
		//DSP_fft(w,1024,process_ptr,fft_out);

		processDoubleBuffer();
	}	
}
