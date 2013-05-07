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
#pragma DATA_ALIGN (A, 8)
fixedp A[PROCESS_SIZE];
#pragma DATA_ALIGN (B, 8)
fixedp B[PROCESS_SIZE];
#pragma DATA_ALIGN (C, 8)
fixedp C[PROCESS_SIZE];
// Tripple buffer pointer for buffer switching
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
DelayParams fxDelay;


#include "brad_pitch_instrumental.h"
PShift fxPitch;

#include "brad_moddelay.h"
ModDelayParams fxChorus, fxVibrato, fxFlanger;

#include "brad_dynamics.h"
DynProc fxComp;

#include "brad_envelopedetector.h"
EnvelopeDetector fxEnvDetect;

#include "brad_3bandeq.h"
EQSTATE fxEQ;

#include "brad_phasevoc.h"
PhaseVocoder fxPhaseVocoder;

#include "brad_dist.h"
Distortion fxDist;

// Våra coola effekter. :) Tusen.  
enum { 
		TREMOLO, DELAY, PITCH, 
		FLANGER, VIBRATO, CHORUS, 
		REVERB, FFTPITCH, OVERDRIVE, 
		FUZZ, DEATHMETALHEAD, BARBERSHOP, 
		COMPRESSOR, GATE, LIMITER, EXPANDER
	};
unsigned int fx; 



#include <csl_hpi.h>
#include "brad_input.h"

// Wavetable
WaveTable LFO;
fixedp oscVal;

// Temp result
short result;

void setup_effects() {
	// setup buffers
	memset(A, 0, PROCESS_SIZE*sizeof(fixedp));
	memset(B, 0, PROCESS_SIZE*sizeof(fixedp));
	memset(C, 0, PROCESS_SIZE*sizeof(fixedp));
	bufferindex = 0;
	bufferflag = 0;
	
	input = A;
	process = B;
	output = C;

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
	
	// Setup Delay
	delayp = 0;
	setupDelayParams(&fxDelay, &delay[0], 48000);
	
	fxDelay.delayInSamples = 48000;
	
	// Setup Pitch Shift
	PShift_setupPitchParams(&fxPitch);
	//memset(delay, 0, sizeof(fixedp)*48000);	
	
	// Setup Flanger
	setFlangerSettings(&fxFlanger, float2q(2.0f), float2q(0.8f), 0);

	// Setup Vibrato
	setVibratoSettings(&fxVibrato, float2q(0.7f), float2q(0.9f));
	
	// Setup Chorus
	setChorusSettings(&fxChorus, float2q(0.6f), float2q(0.4f), 0, float2q(0.010));
	
	// Setup envelope detector
	fxEnvDetect.attack_in_ms = 50;
	fxEnvDetect.release_in_ms = 300;
	EnvDetector_setupDetector(&fxEnvDetect);

	// Setup Dynamics processor (compressor)
	fxComp.AttackTime = 50;
	fxComp.DetectorGain = float2q(2.0f);
	fxComp.KneeWidth = float2q(2.0f);
	fxComp.OutputGain = short2q(1);
	fxComp.procType = Comp;
	fxComp.Ratio = 3;
	fxComp.ReleaseTime = 200;
	fxComp.Threshold = float2q(-20.0f);
	fxComp.timeType = DIGITAL;
	fxComp.detector = &fxEnvDetect;

	// Setup Equalizer
	init_3band_state(&fxEQ,880,5000,48000);
	
	fxEQ.hg = float2q(1.5);
	fxEQ.mg = Q1;
	fxEQ.lg = float2q(0.5);

	// Phase Vocoder
	/*
	phaseVocoder_initialize();
	phaseVocoder_setup(&phaseVoc, 4);*/

	// Setup Distortion
	fxDist.fdb = 0;
	fxDist.gain = float2q(15.0);
	fxDist.lvl1 = 30617;
	fxDist.lvl2 = 7767;
	fxDist.prev = 0;
	//fxDist.maxGain = qdiv(FIXED_INT_MASK, distParam.gain);

	// setup switch
	fx = COMPRESSOR;
}

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
		if(pInput[i] >= AUDIOMAX) pInput[i] = AUDIOMAX-1;
		else if(pInput[i] <= AUDIOMIN) pInput[i] = AUDIOMIN+1;
	}

	return;
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

void c_inpinterrupt(void) {
	Int32 p;
	Uint32 fx, adress, param;
	static int flag = 0;
	if( flag == 0) {
		DSK6416_LED_on(3);
		flag = 1;
	} else {
		DSK6416_LED_off(3);
		flag = 0;
	}
	
	adress = *((int*)(0x01880004)); // ta adressen från HPIAW, OH YEAH
	p = *((int*)adress);
	fx = (adress) & 0x00000F00;
	
	switch(fx) {
	case FX_DELAY:
		// Säg att det är delay
		// Update FX1
		// with param; p;
		// Vad kan man uppdatera för delay. 
		delay_setParam( &fxDelay, adress, p );
		break;
	case FX_FLANGER:
		flanger_setParam( &fxFlanger, adress, p );
		break;
	case FX_CHORUS:
		chorus_setParam( &fxChorus, adress, p );
		break;
	case FX_VIBRATO:
		vibrato_setParam( &fxVibrato, adress, p );
		break;
	case FX_PITCHT:
		pitcht_setParam( &fxPitch, adress, p );
		break;
	case FX_DIST:
		dist_setParam( &fxDist, adress, p );
		break;
	case FX_EQ:
		eq_setParam( &fxEQ, adress, p );
		break;
	}
	return;
}

void process_eq() {
	Uint32 n;
	for(n = 0; n < PROCESS_SIZE; n++) {
		process[n] = do_3band(&fxEQ, process[n]);
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
	
	// Do processing
	process_dist(&fxDist, process);

	process_delay(&fxDelay, process, PROCESS_SIZE);
	/*process_ModDelay(&modDelayParams, process);
	
	process_pitchshift(&pitchC, process);


	
	process_eq();*/

	return;
}

void main()
{
	
	setup_effects();
	DSK6416_LED_init();

	// Initialize interrupts
	comm_intr();
}
