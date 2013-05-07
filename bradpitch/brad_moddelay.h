

#pragma once

#include "qmath.h"
#include "tistdtypes.h"
#include "wt.h"
#include "brad_delay.h"

// functions to update the member objects
void ModDelay_updateLFO();
void ModDelay_updateDDL();

// cooking function for mod type
void ModDelay_cookModType();

// convert a LFO value to a delay offset value
fixedp ModDelay_calculateDelayOffset(fixedp fLFOSample);

enum {Flanger,Vibrato,Chorus}; 	// Effect type
enum {Tri, Sine};		  		// LFO Type
enum {normal,quad,invert}; 		// LFO Phase

// these will depend on the type of mod
typedef struct ModDelayParams_t {
	fixedp fMinDelay_mSec;
	fixedp fMaxDelay_mSec;
	
	fixedp fModDepth_pct;
	fixedp fModFrequency_Hz;

	fixedp fFeedback_pct;
	fixedp fChorusOffset;

	WaveTable LFO;

	Uint32 uLFOPhase;
	Uint32 uModType;
	DelayParams delay;
} ModDelayParams;


void setFlangerSettings(ModDelayParams *this, fixedp depth, fixedp modhz, fixedp feedback);

void setVibratoSettings(ModDelayParams *this, fixedp depth, fixedp modhz);

void setChorusSettings(ModDelayParams *this, fixedp depth, fixedp modhz, fixedp feedback, fixedp chorusOffset);

void process_ModDelay(ModDelayParams *this, fixedp *process);

