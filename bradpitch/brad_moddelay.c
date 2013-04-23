

#include "brad_moddelay.h"
#include "wt.h"
#include "brad_delay.h"

// 48000 * 0.1s = 4800
fixedp modDelayBuffer[4800];

void setFlangerSettings(ModDelayParams *this, fixedp depth, fixedp modhz, fixedp feedback) {
	this->fChorusOffset = 0;
	this->fFeedback_pct = float2q(0.25); // mellan -1 - 1
	this->fMaxDelay_mSec = 7;
	this->fMinDelay_mSec = 0;
	this->fModDepth_pct = depth;
	this->fModFrequency_Hz = modhz;
	this->uModType = Flanger;

	// Set up Delay Line
	setupDelayParams(&this->delay, modDelayBuffer, 4800);
	this->delay.wet = Q0_5;
	this->delay.dry = Q0_5;
	this->delay.fb = this->fFeedback_pct;

	// Set up LFO
	this->LFO.table = TriangleTable;
	WaveTable_cookFrequency(&this->LFO, this->fModFrequency_Hz);
	WaveTable_reset(&this->LFO);
}

void setVibratoSettings(ModDelayParams *this, fixedp depth, fixedp modhz) {
	this->fChorusOffset = 0;
	this->fFeedback_pct = 0;
	this->fMaxDelay_mSec = 7;
	this->fMinDelay_mSec = 0;
	this->fModDepth_pct = depth;
	this->fModFrequency_Hz = modhz;
	this->uModType = Vibrato;

	// Set up Delay Line
	setupDelayParams(&this->delay, modDelayBuffer, 4800);
	this->delay.wet = Q1;
	this->delay.dry = 0;
	this->delay.fb = this->fFeedback_pct;

	// Set up LFO
	this->LFO.table = TriangleTable;
	WaveTable_cookFrequency(&this->LFO, this->fModFrequency_Hz);
	WaveTable_reset(&this->LFO);
}

void setChorusSettings(ModDelayParams *this, fixedp depth, fixedp modhz, fixedp feedback, fixedp chorusOffset) {
	this->fChorusOffset = chorusOffset;
	this->fFeedback_pct = feedback;
	this->fMaxDelay_mSec = 30;
	this->fMinDelay_mSec = 5;
	this->fModDepth_pct = depth;
	this->fModFrequency_Hz = modhz;
	this->uModType = Chorus;

	// Set up Delay Line
	setupDelayParams(&this->delay, modDelayBuffer, 4800);
	this->delay.wet = Q1;
	this->delay.dry = 0;
	this->delay.fb = this->fFeedback_pct;

	// Set up LFO
	this->LFO.table = TriangleTable;
	WaveTable_cookFrequency(&this->LFO, this->fModFrequency_Hz);
	WaveTable_reset(&this->LFO);
}

void process_vibrato(ModDelayParams *this, fixedp *process) {
	int i;
	fixedp out; 
	
	fixedp frac, next;
	int rpi;
	fixedp wetSignal;
/*
	//  z^(depth - (LFO * depth)) 
	for(i = 0; i < PROCESS_SIZE; i++) {
		WaveTable_doOscillate(&this->LFO, &oscVal);
		
		rpi = delayp - (int)qipart( oscVal );
		frac = qfpart( oscVal );
		if(rpi < 0) rpi += MDP.depth;
		next = rpi + 1 != MDP.depth ? delay[rpi+1] : delay[0]; 
		
		wetSignal = qadd(delay[rpi], qmul( frac, qsub( next, delay[rpi] ) ) );
		
		delay[delayp] = process[i];
		if(++delayp >= MDP.depth) delayp = 0; 
		

		out = wetSignal * MDP.wet;
		process[i] = out;	
	}*/
	return;
}

void process_flanger(ModDelayParams *this, fixedp *process) {
}

void process_chorus(ModDelayParams *this, fixedp *process) {
}

// calculateDelayOffset():
/*
	fLFOSample: a value from 0.0 to 1.0 from the LFO object

	returns: the calculated delay time in mSec for each effect

	NOTES: - the range for the flanger/vibrato is simply mapped from min to max
			 starting at min: 
			 fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) + m_fMinDelay_mSec;

		   - the range for the Chorus includes the starting offset
			 fStart = m_fMinDelay_mSec + m_fChorusOffset;
*/
fixedp calculateDelayOffset(ModDelayParams *this, fixedp fLFOSample)
{
	if(this->uModType == Flanger || this->uModType == Vibrato) 
	{
		// flanger 0->1 gets mapped to 0->maxdelay
		// return (fModDepth_pct/100.0)*(fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) + m_fMinDelay_mSec;
		return qadd(qmul(this->fModDepth_pct,qmul(fLFOSample, qsub(this->fMaxDelay_mSec, this->fMinDelay_mSec))), this->fMinDelay_mSec);
	}
	else if(this->uModType == Chorus)
	{
		// Chorus adds starting offset to move delay range
		fixedp fStart = qadd(this->fMinDelay_mSec, this->fChorusOffset);
		return qadd(qmul(this->fModDepth_pct,qmul(fLFOSample, qsub(this->fMaxDelay_mSec, this->fMinDelay_mSec))), fStart);
	}

	return 0;
}

void process_ModDelay(ModDelayParams *this, fixedp *process) {
	int delayInSamples, i;
	fixedp fDelay, fYn, fYqn;
	fYn = 0, fYqn = 0;


	for(i = 0; i < PROCESS_SIZE; i++) {
		// 1. Get LFO Values, normal and quad phase
		WaveTable_doOscillate(&this->LFO, &fYn); // need quad phase addition

		// 2. calculate delay offset
		fDelay = 0;
		if(this->uLFOPhase == quad) {
			fDelay = calculateDelayOffset(this, fYqn); // Quadtrature
		}
		else {
			fDelay = calculateDelayOffset(this, fYn);	// normal
		}

		// 3. set the delay & cook
		delayInSamples = _qmul(fDelay, SAMPLE_RATE, FIXED_FRACBITS, 0, 0); // INTE RÄTT
		this->delay.rp = delayInSamples; // Osäker på detta :\

		// 4. get the delay output one channel in
		process_delay(&this->delay, &process[i], 1);
	}
}
