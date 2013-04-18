#include <std.h>
#include <swi.h>
#include "DSK6416_AIC23.h"
Uint32 fs = DSK6416_AIC23_FREQ_8KHZ;
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource = DSK6416_AIC23_INPUT_MIC;

#include "qmath.h"
#include <csl_hpi.h>
#include "wt.h"
#define N 256

#define GAINADRESS 0x80001000

extern far SWI_Obj SWI_process_isr;

fixedp *input;
fixedp *output;
fixedp *process;
fixedp *tmp;

fixedp delay[8000];
unsigned int delayp;

fixedp pGain;

Uint32 bufferindex;
char bufferflag;

WaveTable t;
short result;
fixedp oscVal;

void process_delay() {
	int i;
	fixedp out;
	for(i = 0; i < N; i++) {
		out = delay[delayp];
		delay[delayp] = process[i];
		process[i] = out;
		
		if(++delayp >= 8000) { delayp = 0; }
	}
	return;
}

void process_tremolo() {
	int i;
	for(i = 0; i < N; i++) {
		WaveTable_doOscillate(&t, &oscVal);
		result = (short)qmul(process[i],oscVal);
		process[i] = result;
	}
	return;
}

void process_pitchshift() {
	return;
}

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

enum { TREMOLO, DELAY, PITCH };
unsigned int fx; 

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
	input = tmp;
	

	// DO PROCESSING
	/*pGain = (fixedp)(*(unsigned volatile int *)GAINADRESS);*/
	switch(fx) {
	case TREMOLO:
		process_tremolo();
		break;
	case DELAY:
		process_delay();
		break;
	case PITCH:
		process_pitchshift();
		break;
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
		
		// Generate wavetables
		WaveTable_generateTables();
		
		t.invert = 0;
		t.table = SinTable;
		t.unipolar = 1;
		t.mInc = 0;
		t.readPointer = 0;

		WaveTable_reset(&t);		
		WaveTable_cookFrequency(&t, float2q(0.707f));
	
	// Delay
	delayp = 0;
	
	// se tup switch
	fx = DELAY;

	// Initialize interrupts
	comm_intr();
}
