
#include "brad_dynamics.h"
#include <math.h>

fixedp lag_x[2];
fixedp lag_y[2];

fixedp calcCompressorGain(DynProc *this, fixedp fDetectorValue)
{
	// initialize variables used in process
	fixedp yG, CS, fThreshLower, fThreshUpper, ratioinv;
	// slope variable
	ratioinv = qinv(this->Ratio);
	CS = qsub(Q1, ratioinv);

	// threshold calcs
	fThreshLower = qsub(this->Threshold, qmul(this->KneeWidth, Q0_5) );
	fThreshUpper = qadd(this->Threshold, qmul(this->KneeWidth, Q0_5) );

	// limitig is infinite ratio thus CS->1.0
	if(this->procType == Limit) {
		CS = Q1;
	}

	// soft-knee with detection value in range?
	if (this->KneeWidth > 0 && 
		fDetectorValue > fThreshLower &&
		fDetectorValue < fThreshUpper )
	{
		// setup for Lagrange
		lag_x[0] = fThreshLower;
		lag_x[1] = fThreshUpper;
		lag_x[1] = 0 < lag_x[1] ? 0 : lag_x[1]; // top limit is 0dBFS
		lag_y[0] = 0;		// CS = 0 for 1:1 ratio
		lag_y[1] = CS; 		// current CS

		// interpolate & overwrite CS
		CS = lagrpol(&lag_x[0], &lag_y[0], 2, fDetectorValue);
	}

	// compute gain: threshold and detection values are in dB
	yG = qmul(CS, qsub(this->Threshold, fDetectorValue)); // [EQ. 13.1]

	// clamp; this allows ratios of 1:1 to still operate
	yG = 0 < yG ? 0 : yG; 
	
	yG = qmul(yG, double2q(0.00022222222));
	// convert back to linear
	return qpow(Q10, yG);
}



void prepareForPlay() {
	// Initialize Envelope detectors depending on Digital or Analog.
}

void userInterfaceUpdate() {
	// set attack time on detectors
	// set release time on detectors

	// update if time constant change (mode analog or digital)
}

void process_dynamics(DynProc *this, fixedp *process, Uint32 process_size) {
	// calculate gains		statiska fram till att de ändras, borde inte vara här
	fixedp fInputGain = qpow(Q10, this->DetectorGain);
	fixedp fOutputGain = qpow(Q10, this->OutputGain);
	int i;
	
	for(i = 0; i < process_size; i++) {
		// detect left channel		- uppdateras varje gång.
		fixedp fLeftDetector = EnvDetector_detect(this->detector, &process[i]);

		// gain calc
		fixedp fGn = Q1;

		// branch
		// compressor
		if(this->procType == Comp || this->procType == Limit) {
			// compressor
			fGn = calcCompressorGain(this, fLeftDetector);
		}
		else {	
			// downward expander
		}

		// form left output and apply make up gain
		process[i] = qmul(qmul(fGn,process[i]),fOutputGain);
		if(process[i] >= AUDIOMAX) {
			process[i] = AUDIOMAX;	
		} 

		// Same for right out if stereo I guess. 
	}
}	
