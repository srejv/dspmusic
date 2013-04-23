#include <std.h>
#include <swi.h>
#include "DSK6416_AIC23.h"
Uint32 fs = DSK6416_AIC23_FREQ_48KHZ;
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource = DSK6416_AIC23_INPUT_MIC;

// HPI header
//#include <csl_hpi.h>

#include "qmath.h"
#include "wt.h"
#include "definebrad.h"



// Parameter adresses
// Gain
#define GAINADRESS 0x80001000
fixedp pGain;

// Interrupts
extern far SWI_Obj SWI_process_isr;

// Tripple buffer
fixedp *input;
fixedp *output;
fixedp *process;
fixedp *tmp;

Uint32 bufferindex;
char bufferflag;

#include "brad_delay.h"

// Delay
fixedp delay[48000];
unsigned int delayp;
DelayParams delayParams;

#include "brad_pitch_instrumental.h"
PShift pitchC;

#include "brad_moddelay.h"
ModDelayParams modDelayParams;


// Våra coola effekter. :) Tusen.  
enum { 
		TREMOLO, DELAY, PITCH, 
		FLANGER, VIBRATO, CHORUS, 
		REVERB, FFTPITCH, OVERDRIVE, 
		FUZZ, DEATHMETALHEAD, BARBERSHOP, 
		COMPRESSOR, GATE, LIMITER, EXPANDER,
		AUTOWAH
	};
unsigned int fx; 


// Wavetable
WaveTable LFO;
fixedp oscVal;

// Temp result
short result;

// Gör inget med signalen, skickar bara igenom den som den är.
void process_bypass() {
	return;
}

// Tremolo - Ändrar amplituden mellan [0,1].
void process_tremolo() {
	int i;
	for(i = 0; i < PROCESS_SIZE; i++) {
		WaveTable_doOscillate(&LFO, &oscVal);
		result = (short)qmul(process[i],oscVal);
		process[i] = result;
	}
	return;
}

// HARD CLIPPING
void process_HARDCLIP(fixedp *pInput) {
	int i;
	for(i = 0; i < PROCESS_SIZE; i++) {
		if(pInput[i] >= 32767) pInput[i] = 32767;
		else if(pInput[i] <= -32767) pInput[i] = -32757;
	}
}


// HW Interrupt 11, audio input/output
void c_int11(void)
{
	input[bufferindex] = (fixedp)input_left_sample();	
	output_left_sample(output[bufferindex]);
	if(++bufferindex >= PROCESS_SIZE) {
		bufferindex = 0;
		bufferflag = 1;
		
		SWI_post(&SWI_process_isr);
	}
	return;
}

// Processval för ljudsignalen, skiftar vår 3-buffer. 
void process_isr(void) 
{
	// buffer switching: input -> process, process -> output, output -> input
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
		process_delay(&delayParams, &process[0], PROCESS_SIZE);
		break;
	case PITCH:
		process_pitchshift(&pitchC, &process[0]);
		break;
	case VIBRATO:
		process_vibrato();
		break;
	case FLANGER:
		process_ModDelay(&modDelayParams, &process[0]);
	default:
		process_bypass();
	}

	//process_HARDCLIP(&process[0]);
	return;
}

void main()
{
	// setup buffers
	input = (fixedp*)malloc(PROCESS_SIZE*sizeof(fixedp));
	output = (fixedp*)malloc(PROCESS_SIZE*sizeof(fixedp));
	process = (fixedp*)malloc(PROCESS_SIZE*sizeof(fixedp));
	memset(&input[0], 0, PROCESS_SIZE*sizeof(fixedp));
	memset(&output[0], 0, PROCESS_SIZE*sizeof(fixedp));
	memset(&process[0], 0, PROCESS_SIZE*sizeof(fixedp));
	bufferindex = 0;
	bufferflag = 0;
	
	*(unsigned volatile int *)GAINADRESS = float2q(0.25f);

	// setup hpi - nothing :O
		
	// Generate wavetables
	WaveTable_generateTables();
	
	LFO.invert = 0;
	LFO.table = SinTable;
	LFO.unipolar = 1;
	LFO.mInc = 0;
	LFO.readPointer = 0;

	WaveTable_reset(&LFO);		
	WaveTable_cookFrequency(&LFO, float2q(0.1f));
	
	// Delay
	delayp = 0;
	setupDelayParams(&delayParams, &delay[0], 48000);
	delayParams.delayInSamples = 48000;
	
	// PShift
	PShift_setupPitchParams(&pitchC);
	//memset(delay, 0, sizeof(fixedp)*48000);	

	// Mod delay
	setFlangerSettings(&modDelayParams, float2q(0.5f), float2q(0.8f), float2q(0.5f));

	// setup switch
	fx = DELAY;

	// Initialize interrupts
	comm_intr();
}
