
#include "qmath.h"
#include "definebrad.h"
#include "brad_envelopedetector.h"
#include "tistdtypes.h"

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
	EnvelopeDetector *detector;
} DynProc;

enum { Comp, Limit, Expand, Gate };
enum { DIGITAL, ANALOG };

void setupDynamics(DynProc *this, EnvelopeDetector *detect);

fixedp calcCompressorGain(DynProc *this, fixedp fDetectorValue);

void process_dynamics(DynProc *this, fixedp *process, Uint32 process_size);
