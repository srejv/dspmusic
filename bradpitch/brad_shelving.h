

#pragma once

#include "qmath.h"


typedef struct ModBiQuad {
	fixedp f_c; 
	int gain_db;
	fixedp Q; // Quality
//	BiQuad f; 	// filter

	fixedp a0, a1, a2, b1, b2; // Coefficients
	fixedp xz1, xz2, yz1, yz2; // z^-1 and z^-2 for x and y

	fixedp c0, d0;			   // Wet/dry for modified biquad
} ModBiQuad;

