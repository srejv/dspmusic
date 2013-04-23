
#include "qmath.h"
#include "definebrad.h"
#include "brad_envelopedetector.h"

typedef struct DynProc_t {
	fixedp DetectorGain;	// (dB)
	fixedp Threshold; 		// (dB)
	Uint32 AttackTime;		// (mSec)
	Uint32 ReleaseTime; 	// (mSec)
	Uint32 Ratio;			// Integer
	fixedp OutputGain;		// (dB)
	fixedp KneeWidth;		// (dB) 
	Uint32 procType;		// COMP, LIMIT, EXPAND, GATE
	Uint32 timeType;		// Digital, Analog
} DynProc;

enum { COMP, LIMIT, EXPAND, GATE };
enum { DIGITAL, ANALOG };

DynProc proc;

fixedp lag_x[2];
fixedp lag_y[2];

EnvelopeDetector envDetect;

fixedp calcCompressorGain(fixedp fDetectorValue, fixedp fThreshold, fixedp fRatio, fixedp fKneeWidth, bool bLimit);

fixedp calcCompressorGain(fixedp fDetectorValue, const DynProc *processor)
{
	// initialize variables used in process
	fixedp yG, CS, fThreshLower, fThreshUpper;
	// slope variable
	CS = qsub(Q1, qinv(processor->Ratio));

	// threshold calcs
	fThreshLower = qsub(processor->Threshold, qmul(processor->KneeWidth, Q1_2) );
	fThreshUpper = qadd(processor->Threshold, qmul(processor->KneeWidth, Q1_2) );

	// limitig is infinite ratio thus CS->1.0
	if(processor->procType == LIMIT) {
		CS = Q1;
	}

	// soft-knee with detection value in range?
	if (processor->KneeWidth > 0 && 
		fDetectorValue > fThreshLower &&
		fDetectorValue < fThreshUpper 
	{
		// setup for Lagrange
		lag_x[0] = fThreshLower;
		lag_x[1] = fThreshUpper;
		lag_x[1] = min(0, lag_x[1]); // top limit is 0dBFS
		lag_y[0] = 0;		// CS = 0 for 1:1 ratio
		lag_y[1] = CS; 		// current CS

		// interpolate & overwrite CS
		CS = lagrpol(&lag_x[0], &lag_y[0], 2, fDetectorValue);
	}

	// compute gain: threshold and detection values are in dB
	yG = qmul(CS, qsub(processor->Threshold, fDetectorValue)); // [EQ. 13.1]

	// clamp; this allows ratios of 1:1 to still operate
	yG = min(0, yG);

	// convert back to linear
	return qpow(Q10, qmul(yG, Q1_20));
}

bool prepareForPlay() {
	// Initialize Envelope detectors depending on Digital or Analog.
}

bool userInterfaceUpdate() {
	// set attack time on detectors
	// set release time on detectors

	// update if time constant change (mode analog or digital)
}

void process_dynamics(fixedp *in, fixedp *out, const DynProc *processor) {
	// calculate gains		statiska fram till att de ändras, borde inte vara här
	fixedp fInputGain = qpow(Q10, processor->DetectorGain);
	fixedp fOutputGain = qpow(Q10, processor->OutputGain);

	// detect left channel		- uppdateras varje gång.
	fixedp fLeftDetector = envDetect.detect(in[0]);

	// gain calc
	fixedp fGn = Q1;

	// branch
	// compressor
	if(processor->procType == COMP || processor->procType == LIMIT) {
		// compressor
		fGn = calcCompressorGain(fLeftDetector, processor);
	}
	else {	
		// downward expander
	}

	// form left output and apply make up gain
	out[0] = fGn * in[0] * fOutputGain;

	// Same for right out if stereo I guess. 
}	

