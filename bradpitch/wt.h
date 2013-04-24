/**
	Super fixed-point qmath.h based WaveTableOscillator.

	Written by: Oscar Dragén
	Based on the Oscillator in Designing C++ Audio Plugins by Will Perkle. 
*/

#ifndef WT_H_
#define WT_H_

#include "qmath.h"
#include "definebrad.h"
#define WT_SIZE 1024
//#define SAMPLE_RATE 8000;

extern fixedp SinTable[];

//fixedp SinTable[N];
extern fixedp SquareTable[];
extern fixedp SawtoothTable[];
extern fixedp TriangleTable[];

typedef struct WaveTable_t {
	fixedp readPointer;
	fixedp quadPhaseReadIndex;
	fixedp mInc;
	unsigned char invert;
	unsigned char unipolar;
	fixedp *table;
} WaveTable;

// pYn = output sample
void WaveTable_doOscillate(WaveTable *this, fixedp *pYn, fixedp *pYqn);

// generates the Sin, Square, Sawtooth and Triangle tables
void WaveTable_generateTables();

// turns a frequency into an increment for the wavetable structure
void WaveTable_cookFrequency(WaveTable *this, fixedp frequency);

// resets the wavetable reading. things. 
void WaveTable_reset(WaveTable* this);


inline fixedp dLinTerp(fixedp x1, fixedp x2, fixedp y1, fixedp y2, fixedp x)
{
	fixedp dx, result, denom;
	denom = qsub(x2, x1);
	if(denom == 0)
		return y1; // should not ever happen

	// calculate decimal position of x
	dx = qdiv(qsub(x, x1), denom);

	// use weighted sum method of interpolating
	result = qadd( qmul( dx, y2 ), qmul( qsub( short2q(1), dx ), y1 ));

	return result;
}

/*inline fixedp lagrage() {

}*/

#endif
