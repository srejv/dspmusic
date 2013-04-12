#include <std.h>
#include <swi.h>
#include "DSK6416_AIC23.h"
Uint32 fs = DSK6416_AIC23_FREQ_8KHZ;
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource = DSK6416_AIC23_INPUT_MIC;

#include "qmath.h"
#include <csl_hpi.h>
#define N 256

extern far SWI_Obj SWI_process_isr;

fixedp *input;
fixedp *output;
fixedp *process;
fixedp *tmp;


#define GAINADRESS 0x80001000
#define GAIN (*(unsigned volatile fixedp *)GAINADRESS);
fixedp pGain;

Uint32 bufferindex;
char bufferflag;

void c_int11(void)
{
	input[bufferindex] = (fixedp)input_left_sample();	
	output_left_sample((short)output[bufferindex]);
	if(++bufferindex >= N) {
		bufferindex = 0;
		bufferflag = 1;
		SWI_post(&SWI_process_isr);
	}
	return;
}

short result;
void process_isr(void) 
{
	int i;
	// buffer switching
	// input -> process
	// process -> output
	// output -> input
	tmp = output;
	output = process;
	process = input;
	input = output;
	

	// DO PROCESSING
	pGain = (fixedp)(*(unsigned volatile int *)GAINADRESS);
	for(i = 0; i < N; i++) {
		result = (short)qmul(process[i],pGain);
		process[i] = result;
	}
	return;
}

void main()
{
	// setup buffers
	input = (fixedp*)malloc(N*sizeof(fixedp));
	output = (fixedp*)malloc(N*sizeof(fixedp));
	process = (fixedp*)malloc(N*sizeof(fixedp));
	memset(&input[0], 0, N*sizeof(fixedp));
	memset(&output[0], 0, N*sizeof(fixedp));
	memset(&process[0], 0, N*sizeof(fixedp));
	bufferindex = 0;
	bufferflag = 0;
	
	*(unsigned volatile int *)GAINADRESS = float2q(0.25f);

	// setup hpi - nothing :O
	

	// Initialize interrupts
	comm_intr();
}
