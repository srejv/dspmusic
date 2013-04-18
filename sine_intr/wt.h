/**
	Super fixed-point qmath.h based WaveTableOscillator.

	Written by: Oscar Dragén
	Based on the Oscillator in Designing C++ Audio Plugins by Will Perkle. 
*/

#ifndef WT_H_
#define WT_H_

#include "qmath.h"

#define N 1024
#define SAMPLE_RATE 8000;

extern fixedp SinTable[];

//fixedp SinTable[N];
extern fixedp SquareTable[];
extern fixedp SawtoothTable[];
extern fixedp TriangleTable[];

typedef struct WaveTable_t {
	fixedp readPointer;
	fixedp mInc;
	unsigned char invert;
	unsigned char unipolar;
	fixedp *table;
} WaveTable;

// pYn = output sample
void WaveTable_doOscillate(WaveTable *this, fixedp *pYn);

// generates the Sin, Square, Sawtooth and Triangle tables
void WaveTable_generateTables();

// turns a frequency into an increment for the wavetable structure
void WaveTable_cookFrequency(WaveTable *this, fixedp frequency);

// resets the wavetable reading. things. 
void WaveTable_reset(WaveTable* this);

#endif
