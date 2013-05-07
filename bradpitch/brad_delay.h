

#pragma once 

#include "qmath.h"
#include "tistdtypes.h"

typedef struct DelayParams_t {
	int wp;	// write pointer
	fixedp rp;  // read pointer
	fixedp inc; // increment

	fixedp fb;	// feedback
	Uint32 delayInSamples;

	fixedp wet;
	fixedp dry;
	
	unsigned char useExternalFeedback;
	unsigned char active;
	fixedp externalFbSample;
	fixedp currentFbSample;

	Uint32 bufferSize;
	fixedp *buffer;
} DelayParams;

void setupDelayParams(DelayParams *this, fixedp *buffer, Uint32 bufferSize);
void setDelay(DelayParams *this, int delayInSamples);
void process_delay(DelayParams *this, fixedp *process, Uint32 processSize);


void delay_setDelayTime(DelayParams *this, int delayInSamples);
void delay_setMix(DelayParams *this, int mix);
void delay_setFb(DelayParams *this, int fb);

