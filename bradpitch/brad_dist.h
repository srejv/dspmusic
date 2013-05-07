
#pragma once

#include "qmath.h"
#include "tistdtypes.h"


typedef struct Distortion_t {
	fixedp gain;
	fixedp fdb;
	fixedp lvl1;
	fixedp lvl2;
	
	fixedp maxGain;
	fixedp prev;
} Distortion;

void process_dist(Distortion *t, fixedp *x);
void process_fuzz(Distortion *t, fixedp *x);
void process_thunderFuzz( Distortion *t, fixedp *x );
void process_overdrive(Distortion *t, fixedp *x);
void process_squareDist(Distortion *t, fixedp *x);

void dist_setParam(Distortion *t, Uint32 param, int val);
