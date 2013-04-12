#include "DSK6416_AIC23.h"	//codec-DSK interface support
Uint32 fs=DSK6416_AIC23_FREQ_96KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE;

#include "dsp_fft32x32.h"
#include "dsp_ifft32x32.h"
#include <math.h>
#include "tw256.inc"

#define STOI 65000
#define ITOS 1000

typedef struct COMPLEX_tag {
	int real;
	int imag;
} COMPLEX;


#define BUF_SIZE 1024
COMPLEX A[BUF_SIZE], B[BUF_SIZE], C[BUF_SIZE];
COMPLEX D[BUF_SIZE];

short index = 0;
COMPLEX *input_ptr, *output_ptr, *process_ptr, *temp_ptr;
COMPLEX *fft_out;

short filledBufferFlag = 0;
short outbuffer[BUF_SIZE];
short o = 0;

void processDoubleBuffer() {
	int i;

	DSP_fft32x32(w,BUF_SIZE,(int*)process_ptr,(int*)fft_out);
	
	for(i = 0; i < BUF_SIZE; i++) {
		// Process
		//process_ptr[i] = process_ptr[i];
		//fft_out[i].imag++;
	}
	DSP_ifft32x32(w,BUF_SIZE,(int*)fft_out, (int*)process_ptr);
}


interrupt void c_int11() {
	o = (short)((output_ptr[index]).real);
	output_left_sample(o);
	outbuffer[index] = -o;
	input_ptr[index].real = ((int)input_left_sample());
	input_ptr[index++].imag = 0;

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
		A[i].real = 0;
		A[i].imag = 0;
		B[i].real = 0;
		B[i].imag = 0;
		C[i].real = 0;
		C[i].imag = 0;
		D[i].real = 0;
		D[i].imag = 0;
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
		
		processDoubleBuffer();
	}	
}
