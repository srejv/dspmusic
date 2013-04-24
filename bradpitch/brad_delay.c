
#include "brad_delay.h"
#include "definebrad.h"

void setupDelayParams(DelayParams *this, fixedp *buffer, Uint32 bufferSize) {
	this->wp = 0;
	this->rp = 0;
	this->inc = Q1;
	this->fb = float2q(0.01f);

	this->useExternalFeedback = 0;
	this->externalFbSample = 0;
	this->currentFbSample = 0;
	
	this->wet = Q0_5;
	this->dry = Q0_5;

	this->delayInSamples = 0;
	this->buffer = buffer;
	this->bufferSize = bufferSize;

	memset(buffer, 0, sizeof(fixedp)*bufferSize);
}

// Utför en delay. Just nu fixed på 8000 samples (1s, 8khz samplerate)
void process_delay(DelayParams *this, fixedp *process, Uint32 processSize) {
	int i;
	fixedp out, in;
	int rpi; // integer part
	fixedp frac; // fractional part
	fixedp next; // next value in case of wrapping
	fixedp result;
	
	fixedp feedbackSample;

	for(i = 0; i < processSize; i++) {
		in = process[i];
		
		rpi = (int)qipart(this->rp);
		frac = qfpart(this->rp);
		next = rpi + 1 != this->bufferSize ? this->buffer[rpi+1] : this->buffer[0];
		
		out = this->buffer[rpi];

		// out = out + frac * (next - out) (linear interp)
		result = qadd(out, qmul(frac, qsub(next, out))); 
		
		// in i buffern (in + ut*feedback)
		if(this->useExternalFeedback == 0) {
			this->buffer[this->wp] = qadd(in, qmul(result, this->fb));
		}
		else {
			this->buffer[this->wp] = qadd(in, qmul(this->externalFbSample, this->fb)); // + delayP.fb*result ;
		}
		

		// Summera och släng ut! (ut * wet + in * dry)
		process[i] = qadd(qmul(result,this->wet), qmul(in,this->dry));
		
		// Increase read pointer and wrap around
		this->rp = qadd(this->rp, this->inc); 
		rpi = (int)qipart(this->rp);
		if(rpi >= this->bufferSize) this->rp = qsub(this->rp, this->bufferSize << FIXED_FRACBITS);

		// Increase write pointer and wrap around
		this->wp += 1;
		if(this->wp >= this->bufferSize) this->wp = 0;
	}
	return;
}

void setDelay(DelayParams *this, int delayInSamples) {
	// subtract to make read index
	this->rp = int2q(this->wp - delayInSamples); // cast as int!

	// check and wrap BACKWARDS if the index is negative
	if( this->rp < 0 )
		this->rp += int2q(this->bufferSize);
}
