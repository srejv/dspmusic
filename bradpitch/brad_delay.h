

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
	fixedp externalFbSample;
	fixedp currentFbSample;

	Uint32 bufferSize;
	fixedp *buffer;
} DelayParams;

void setupDelayParams(DelayParams *this, fixedp *buffer, Uint32 bufferSize);
void setDelay(DelayParams *this, int delayInSamples);
void process_delay(DelayParams *this, fixedp *process, Uint32 processSize);
