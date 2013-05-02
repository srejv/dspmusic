#include <std.h>
#include <swi.h>
#include "DSK6416_AIC23.h"
Uint32 fs = DSK6416_AIC23_FREQ_48KHZ;
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource = DSK6416_AIC23_INPUT_LINE;

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
fixedp *processOut;
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

#include "brad_dynamics.h"
DynProc dynProc;

#include "brad_envelopedetector.h"
EnvelopeDetector envDetect;

#include "brad_3bandeq.h"
EQSTATE eq;


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



typedef struct Distortion_t {
	fixedp gain;
	fixedp fdb;
	fixedp lvl1;
	fixedp lvl2;
	
	fixedp prev;
} Distortion;

void process_fuzz(Distortion *t, fixedp *x) { 
	Uint32 n;
	
	if(t->fdb != 0) {
	
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qadd(qmul(x[n], t->gain), qmul(t->prev, t->fdb));

			if (t->prev > t->lvl1)
				t->prev = t->lvl1;
			else if (t->prev < -t->lvl2)
				t->prev = -t->lvl2;

			x[n] = t->prev;
		}

	}
	else {
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qmul(x[n], t->gain);

			if (t->prev > t->lvl1)
				t->prev = t->lvl1;
			else if (t->prev < -t->lvl2)
				t->prev = -t->lvl2;

			x[n] = t->prev;
		}
	}

}

void process_thunderFuzz( Distortion *t, fixedp *x ) {
	Uint32 n;
	
	// remember to keep sample from previous cycle. :) keep in structure. disttruc
	if(t->fdb) {
		for(n=0; n < PROCESS_SIZE; n++) {
			t->prev = qadd(qmul(x[n],t->gain), qmul(t->prev, t->fdb));
		
			if (t->prev > t->lvl1) 
				t->prev = t->lvl1;
			else if (t->prev < -t->lvl2)
				t->prev = -t->lvl2;

			x[n] = qabs(t->prev);
		}
	}
	else {
		for(n=0; n < PROCESS_SIZE; n++) {
			t->prev = qmul(x[n],t->gain);

			if (t->prev > t->lvl1) 
				t->prev = t->lvl1;
			else if (t->prev < -t->lvl2)
				t->prev = -t->lvl2;
			

			x[n] = qabs(t->prev);
		}
	}
}

void process_overdrive(Distortion *t, fixedp *x) {
	
	Uint32 n;
	fixedp numeratorLvl1, numeratorLvl2, denom;

	// kom ihåg förra processens sampel?
	if(t->fdb) {
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qadd( qmul( x[n],t->gain ), qmul( qmul(t->prev, t->gain), t->fdb ) );
			
			// 6554 = 0.2
			numeratorLvl1 = qmul(t->lvl1, qexp(qmul(t->prev, qadd(Q1, qmul(6554, qsub(Q1, t->lvl1))))));
			numeratorLvl2 = qmul(t->lvl2, qexp(qmul(t->prev, qadd(Q1, qmul(6554, qsub(Q1, t->lvl2))))));
			denom = qadd(qexp(t->prev), qexp(-t->prev));
			x[n] = qmul(qsub(numeratorLvl1, numeratorLvl2), qinv(denom));
		}
	} 
	else {
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qmul(x[n],t->gain);
			
			numeratorLvl1 = qmul(t->lvl1, qexp(qmul(t->prev, qadd(Q1, qmul(6554, qsub(Q1, t->lvl1))))));
			numeratorLvl2 = qmul(t->lvl2, qexp(qmul(t->prev, qadd(Q1, qmul(6554, qsub(Q1, t->lvl2))))));
			denom = qadd(qexp(t->prev), qexp(-t->prev));
			x[n] = qmul(qsub(numeratorLvl1, numeratorLvl2), qinv(denom));
		}
	}

	// 
}

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
	fixedp y, yq;
	for(i = 0; i < PROCESS_SIZE; i++) {
		WaveTable_doOscillate(&LFO, &y, &yq);
		result = (short)qmul(process[i],y);
		process[i] = result;
	}
	return;
}

// HARD CLIPPING
void process_HARDCLIP(fixedp *pInput) {
	int i;
	for(i = 0; i < PROCESS_SIZE; i++) {
		if(pInput[i] >= AUDIOMAX) pInput[i] = AUDIOMAX;
		else if(pInput[i] <= AUDIOMIN) pInput[i] = AUDIOMIN;
	}
}


// HW Interrupt 11, audio input/output
void c_int11(void)
{
	input[bufferindex] = (fixedp)input_left_sample();	
	output_left_sample(output[bufferindex]);
	//output_right_sample(output[bufferindex]);
	if(++bufferindex >= PROCESS_SIZE) {
		bufferindex = 0;
		bufferflag = 1;
		
		SWI_post(&SWI_process_isr);
	}
	return;
}

void process_eq() {
	Uint32 n;
	for(n = 0; n < PROCESS_SIZE; n++) {
		process[n] = do_3band(&eq, process[n]);
	}
}

// Processval för ljudsignalen, skiftar vår 3-buffer. 
void process_isr(void) 
{
	// buffer switching: input -> process, process -> output, output -> input
	tmp = output;
	output = process;
	process = input;
	input = tmp;
	
	process_eq();
	// DO PROCESSING
	/*pGain = (fixedp)(*(unsigned volatile int *)GAINADRESS);*/
	/*
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
		//process_vibrato();
		break;
	case FLANGER:
		process_ModDelay(&modDelayParams, &process[0]);
		break;
	case COMPRESSOR:
		process_dynamics(&dynProc, &process[0], PROCESS_SIZE);
		break;
	default:
		process_bypass();
	}*/

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
//	setFlangerSettings(&modDelayParams, float2q(2.0f), float2q(0.8f), 0);
//	setVibratoSettings(&modDelayParams, float2q(0.7f), float2q(0.9f));
	setChorusSettings(&modDelayParams, float2q(0.6f), float2q(0.4f), 0, float2q(0.010));
	
	envDetect.attack_in_ms = 50;
	envDetect.release_in_ms = 300;
	EnvDetector_setupDetector(&envDetect);

	dynProc.AttackTime = 50;
	dynProc.DetectorGain = float2q(2.0f);
	dynProc.KneeWidth = float2q(2.0f);
	dynProc.OutputGain = short2q(1);
	dynProc.procType = Comp;
	dynProc.Ratio = 3;
	dynProc.ReleaseTime = 200;
	dynProc.Threshold = float2q(-20.0f);
	dynProc.timeType = DIGITAL;
	dynProc.detector = &envDetect;

	// EQ
	init_3band_state(&eq,880,5000,48000);
	
	eq.hg = float2q(1.5);
	eq.mg = Q1;
	eq.lg = float2q(0.5);

	// setup switch
	fx = COMPRESSOR;

	// Initialize interrupts
	comm_intr();
}
