
#include "wt.h"
#include "qmath.h"
#include <stdio.h>


fixedp SinTable[WT_SIZE];
fixedp SquareTable[WT_SIZE];
fixedp SawtoothTable[WT_SIZE];
fixedp TriangleTable[WT_SIZE];

fixedp pointfive;
fixedp oneOverSR;
fixedp qN;


void WaveTable_doOscillate(WaveTable *this, fixedp *pYn, fixedp *pYqn) {
	
	fixedp outSample = 0;
	fixedp quadOutSample = 0;

	// get int part
	int readIndex = qipart(this->readPointer);
	int quadPhaseReadIndex = qipart(this->quadPhaseReadIndex);

	// get frac part
	fixedp frac = qfpart(this->readPointer);

	// check for next, might wrap around
	int readNextIndex = readIndex + 1 > WT_SIZE - 1 ? 0 : readIndex + 1;
	int quadReadNextIndex = quadPhaseReadIndex + 1 > WT_SIZE - 1 ? 0 : quadPhaseReadIndex + 1;

	//printf("This: %ld, Next: %ld, Frac: %ld \n", this->table[readIndex], this->table[readNextIndex], frac);
	outSample = dLinTerp(0, Q1, this->table[readIndex], this->table[readNextIndex], frac);
	quadOutSample = dLinTerp(0, Q1, this->table[quadPhaseReadIndex], this->table[quadReadNextIndex], frac);
	// add increment
	this->readPointer = qadd(this->readPointer, this->mInc);
	this->quadPhaseReadIndex = qadd(this->quadPhaseReadIndex, this->mInc);

	// check for wrap around
	if(this->readPointer >= qN) {
		this->readPointer = qsub(this->readPointer, qN);
	}

	if(this->quadPhaseReadIndex >= qN) {
		this->quadPhaseReadIndex = qsub(this->quadPhaseReadIndex, qN);
	}

	// output
	*pYn = outSample;
	*pYqn = quadOutSample;

	if(this->invert) {
		*pYn *= -1;
		*pYqn *= -1;
	}

	if(this->unipolar) {
		*pYn = qmul(*pYn, pointfive);
		*pYn = qadd(*pYn, pointfive);

		*pYqn = qmul(*pYqn, pointfive);
		*pYqn = qadd(*pYqn, pointfive);
	}
}

void WaveTable_generateTables() {
	

	// Variables
	fixedp fi, mt1, bt1, mt2, bt2, mtf2, btf2;
	fixedp ms1, bs1, ms2, bs2;
	fixedp oneOverN;
	fixedp squareMin, squareMax;
	int i;

	// Useful variables
	oneOverSR = qinv(short2q(8000));	// 1/SR (1/8000)
	pointfive = qinv(short2q(2));		// 0.5f
	qN = short2q(WT_SIZE);				// 1024 in q (<< 16)

	oneOverN = short2q(WT_SIZE);		// N (we want 1/1024)
	oneOverN = qinv(oneOverN);			// 1/N (1/1024)


	// Triangle
	// rising edge1:
	mt1 = qinv(short2q(256));
	bt1 = 0;

	// rising edge 2:
	mt2 = qinv(short2q(256));
	bt2 = short2q(-1);

	// valling edge:
	mtf2 = qinv(short2q(512));
	mtf2 = qmul(short2q(-2), mtf2);
	btf2 = short2q(1);

	// Sawtooth
	// rising edge1:
	ms1 = qinv(short2q(512));
	//ms1 = qinv(ms1);
	bs1 = 0;

	// rising edge2:
	ms2 = qinv(short2q(512));
	//ms2 = qinv(ms2);
	bs2 = short2q(-1) + 1;

	// square
	squareMax = qmul(short2q(1), float2q(0.70795f));
	squareMin = qmul(short2q(-1), float2q(0.70795f));

	// create SineTable
	for(i = 0; i < WT_SIZE; i++) {
		fi = short2q(i);
		//printf("%lu \n", qmul(fi, oneOverN));

		// Sin table	( sin( (fi*1/n)*2pi ) * 0.70795f) (-3dB)
		SinTable[i] = qmul( qsin( qmul( qmul( fi, oneOverN ), Q2PI )), float2q(0.70795f) );	

		// Square table
		if(i > 0)
			SquareTable[i] = i < 512 ? squareMax : squareMin;
		else 
			SquareTable[i] = 0;

		//printf("%ld %f\n", SquareTable[i], q2float(SquareTable[i]) );

		// Sawtooth table
		SawtoothTable[i] = i < 512 ? qadd( qmul( ms1, fi ), bs1 ) : qadd( qmul( ms2, short2q( i-511 ) ), bs2 );
		SawtoothTable[i] = qmul( SawtoothTable[i], float2q(0.70795f));
		//printf("%ld %f\n", SawtoothTable[i], q2float(SawtoothTable[i]) );
		

		// Triangle table
		if( i < 256 )
			TriangleTable[i] = qadd( qmul( mt1, fi ), bt1 ); // mx + b; rising edge 1
		else if(i < 768)
			TriangleTable[i] = qadd( qmul( mtf2, short2q( i-256 ) ), btf2 ); // mx + b; falling edge
		else 
			TriangleTable[i] = qadd( qmul( mt2, short2q( i-768 ) ), bt2 ); // mx + b; rising edge 2c
		
		TriangleTable[i] = qmul( TriangleTable[i], float2q(0.70795f) );

		//printf("%ld %f\n", TriangleArray[i], q2float(TriangleArray[i]) );
	}
}

void WaveTable_cookFrequency(WaveTable *this, fixedp frequency) {
	this->mInc = qmul( short2q(WT_SIZE), qmul(frequency, oneOverSR));
}

void WaveTable_reset(WaveTable* this) {
	this->readPointer = 0;
}
