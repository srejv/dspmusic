
#pragma once 

#include "qmath.h"
#include "tistdtypes.h"

#define PSHIFT_BUFSIZE 2048
typedef struct PShift_t {
	fixedp gain;
	fixedp inc;
	fixedp fdb;
	fixedp rp1;
	Uint32 wp;
	Uint32 wet;
	Uint32 dry;
	fixedp pbuf[PSHIFT_BUFSIZE];
	fixedp env[PSHIFT_BUFSIZE];
} PShift;

// Pitch Shift - Tidsplanet.
void process_pitchshift(PShift *this, fixedp *process);

void PShift_setupPitchParams(PShift *this);

void PShift_setPitchStep(PShift *this, short step);

void pitcht_setParam(PShift *t, Uint32 param, int val);
