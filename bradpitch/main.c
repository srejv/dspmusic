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
#define N 256
#define SR 48000

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

// Delay
fixedp delay[8000];
unsigned int delayp;

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

typedef struct DelayParams_t {
	Uint32 wp;	// write pointer
	fixedp rp;  // read pointer
	fixedp inc; // increment

	fixedp fb;	// feedback
	Uint32 delayInSamples;
	
	fixedp externalFb;
	unsigned char useExternalFeedback;

	Uint32 bufferSize;
	fixedp *buffer;
} DelayParams;
DelayParams delayP;

void setupDelayParams(DelayParams *this, fixedp *buffer, Uint32 bufferSize) {
	this->wp = 0;
	this->rp = 0;
	this->inc = 1;
	this->fb = 0;
	this->delayInSamples = 0;
	this->buffer = buffer;
	this->bufferSize = bufferSize;
}

// Utför en delay. Just nu fixed på 8000 samples (1s, 8khz samplerate)
void process_delay() {
	int i;
	fixedp out;
	Uint32 rpi; // integer part
	fixedp frac; // fractional part
	fixedp next; // next value in case of wrapping

	for(i = 0; i < N; i++) {
		
		out = delayP.buffer[delayP.rp];
		delayP.buffer[delayP.wp] = process[i] + delayP.fb*out;

		process[i] = out;
		
		delayP.wp++;
		if(delayP.wp >= delayP.delayInSamples) { delayP.wp = 0; }
	}
	return;
}

// Tremolo - Ändrar amplituden mellan [0,1].
void process_tremolo() {
	int i;
	for(i = 0; i < N; i++) {
		WaveTable_doOscillate(&LFO, &oscVal);
		result = (short)qmul(process[i],oscVal);
		process[i] = result;
	}
	return;
}

typedef struct ModDelayParams_t {
	// Stuff with ranges [0,1]
	fixedp dry, wet, fb; // BL, FF,FB
	// Struff in ms [0,2000], convert to samples
	unsigned int predelay, depth, modhz; 
} ModDelayParams;
ModDelayParams MDP;

void setFlangerSettings(ModDelayParams *this, fixedp depth, fixedp modhz) {
	this->depth = 3/1000 * SR;
	this->dry = float2q(0.70795f);
	this->wet = float2q(0.70795f);
	this->fb = float2q(0.70795f);
	this->predelay = 0;
	this->modhz = float2q(2.0f);

	LFO.table = SinTable;
	WaveTable_cookFrequency(&LFO, this->modhz);
	WaveTable_reset(&LFO);
}

void setVibratoSettings(ModDelayParams *this, fixedp depth, fixedp modhz) {
	this->depth = qmul(float2q(0.003f), SR); // [0,3] ms
	this->dry = 0;					// 0
	this->fb = 0;					// 0
	this->modhz = float2q(0.1);// [0.1, 5] Hz
	this->predelay = 0;				// 0
	this->wet = short2q(1);			// 1

	LFO.table = SinTable;
	WaveTable_cookFrequency(&LFO, this->modhz);
	WaveTable_reset(&LFO);
}

void process_vibrato() {
	int i;
	fixedp out; 
	
	fixedp frac, next;
	int rpi;
	fixedp wetSignal;

	//  z^(depth - (LFO * depth)) 
	for(i = 0; i < N; i++) {
		WaveTable_doOscillate(&LFO, &oscVal);
		
		rpi = delayp - (int)qipart( oscVal );
		frac = qfpart( oscVal );
		if(rpi < 0) rpi += MDP.depth;
		next = rpi + 1 != MDP.depth ? delay[rpi+1] : delay[0]; 
		
		wetSignal = qadd(delay[rpi], qmul( frac, qsub( next, delay[rpi] ) ) );
		
		delay[delayp] = process[i];
		if(++delayp >= MDP.depth) delayp = 0; 
		

		out = wetSignal * MDP.wet;
		process[i] = out;	
	}
	return;
}

#define PSHIFT_BUFSIZE 2048
typedef struct PShift_t {
	fixedp gain;
	fixedp inc;
	fixedp fdb;
	fixedp rp1;
	fixedp pbuf[PSHIFT_BUFSIZE];
	fixedp env[PSHIFT_BUFSIZE];
} PShift;

PShift pitchC;

// Pitch Shift - Tidsplanet.
void process_pitchshift() {
	int i;
	fixedp in, out, next;
	fixedp rp2, frac, fi;
	fixedp wetSignal;
	int rpi, ep1, ep2;
	
	fixedp fixedBufSize = short2q(PSHIFT_BUFSIZE);
	fixedp bufOverTwo = 2 * qinv( fixedBufSize ); 
	for(i = 0; i < N; i++ ) {
		in = process[i];
		fi = short2q(i);

		// Envelope pointer1 
		ep1 = (int)qipart( qsub(pitchC.rp1, fi) );
		if(ep1 < 0) ep1 = qadd(ep1, PSHIFT_BUFSIZE);
		
		// Calculate read pointer 2 and envelope pointer 2
		rp2 = qadd(pitchC.rp1, bufOverTwo);
		if(rp2 >= fixedBufSize) rp2 = qsub(rp2, fixedBufSize);
		ep2 = (int)qipart( qsub( rp2, fi ) );
		if(ep2 < 0) ep2 = qadd(ep2, PSHIFT_BUFSIZE);

		// Get signal for read pointer 1
		rpi = (int)qipart( pitchC.rp1 );		
		frac = qfpart( pitchC.rp1 );
		next = (rpi != PSHIFT_BUFSIZE-1 ? pitchC.pbuf[rpi+1] : pitchC.pbuf[0]);
		
		wetSignal = qmul( qadd(pitchC.pbuf[rpi], qmul( frac, qsub( next, pitchC.pbuf[rpi] ) ) ), pitchC.env[ep1] );
	 			
		rpi = (int)qipart( rp2 );
		frac = qfpart( rp2 );
		next = (rpi != PSHIFT_BUFSIZE-1 ? pitchC.pbuf[rpi+1] : pitchC.pbuf[0]);
	 	
		wetSignal = qadd(wetSignal, qmul( qadd(pitchC.pbuf[rpi], qmul( frac, qsub( next, pitchC.pbuf[rpi] ) ) ), pitchC.env[ep2] ));
		
	 	out = qadd(in, wetSignal); //qadd(wetSignal, in);     // output sum of input and delayed values
	 	process[i] = out;
	 	
	 	pitchC.pbuf[delayp] = in; //qmul(in, pitchC.gain); // + delayed*GAIN; // store new input and a fraction 
	  		                            // of the delayed value in buffer
		
	  	if(++delayp >= PSHIFT_BUFSIZE) delayp=0;      // test for end of buffer
		pitchC.rp1 += pitchC.inc;
		if(pitchC.rp1 >= fixedBufSize) pitchC.rp1 = 0;
	}
	return;
}

// HW Interrupt 11, audio input/output
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
		process_delay();
		break;
	case PITCH:
		process_pitchshift();
		break;
	case VIBRATO:
		process_vibrato();
		break;
	default:
		process_bypass();
	}
	return;
}

void main()
{
	fixedp twooverbufsize;
	int i;
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
		
		LFO.invert = 0;
		LFO.table = SinTable;
		LFO.unipolar = 1;
		LFO.mInc = 0;
		LFO.readPointer = 0;

		WaveTable_reset(&LFO);		
		WaveTable_cookFrequency(&LFO, float2q(0.1f));
	
	// Delay
	delayp = 0;
	
	// Setup pitch shift
	pitchC.rp1 = 0;
	pitchC.inc = qpow( short2q(2), qdiv( short2q(7), short2q(12) ) ); // 2^pitch/12
	pitchC.gain = float2q(0.9f);
	pitchC.fdb = 0;
	twooverbufsize = 2 * qinv(short2q(PSHIFT_BUFSIZE));
	for( i = 0; i < 1024; i++ ) 
		pitchC.env[i] = i * twooverbufsize;
	for( i = 1024; i >= 0; i--) 
		pitchC.env[PSHIFT_BUFSIZE-i-1] = i * twooverbufsize;
	
	//memset(pitchC.pbuf, 0, sizeof(fixedp)*PSHIFT_BUFSIZE);

	// se tup switch
	fx = DELAY;

	// Initialize interrupts
	comm_intr();
}
