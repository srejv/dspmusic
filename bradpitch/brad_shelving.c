
#include "brad_shelving.h"

#include "qmath.h"
#include "biquad.h"
#include "tistdtypes.h"
#include "definebrad.h"



void calculatePeak(ModBiQuad *t, fixedp frequency, int gain, fixedp Q) {
	fixedp theta_c, u, beta, c, gamma, invTheta_s, inv_1over20, qgain, beta_numerator, beta_denominator;

	t->f_c = frequency; // center frequency
	t->gain_db = gain;
	t->Q = Q;

	invTheta_s = _qinv(SAMPLE_RATE, 0, FIXED_FRACBITS);
	inv_1over20 = _qinv(20, 0, FIXED_FRACBITS);
	qgain = int2q(gain);
	
	theta_c = qmul( qmul( Q2PI, frequency ), invTheta_s );
	u = qpow(Q10, qmul(qgain, inv_1over20));
	c = qmul(Q4, qinv(qadd(Q1,u)));
	beta_numerator = qsub(Q1, qmul(c, qtan(qmul(theta_c, 2*Q))));
	beta_denominator = qadd(Q1, qmul(c, qtan(qmul(theta_c, 2*Q))));
	beta = qmul(Q0_5, qdiv(beta_numerator, beta_denominator));
	gamma = qmul(qadd(Q0_5, beta), qcos(theta_c));

	t->a0 = 0.5 - beta;
	t->a1 = 0;
	t->a2 = -(0.5 - beta);
	t->b1 = -2*gamma;
	t->b2 = 2*beta;
	t->c0 = u - Q1;
	t->d0 = Q1;
}

void flushBiquad(ModBiQuad *t) {
	t->xz1 = 0;
	t->xz2 = 0;
	t->yz1 = 0;
	t->yz2 = 0;
}

void calculateShelving(ModBiQuad *t, fixedp frequency, int gain, Uint8 isLow) {
	fixedp theta_c, u, beta, sigma, gamma, invTheta_s, inv_1over20, qgain, tmp;

	t->f_c = frequency; // center frequency
	t->gain_db = gain;

	invTheta_s = _qinv(SAMPLE_RATE, 0, FIXED_FRACBITS);
	inv_1over20 = _qinv(20, 0, FIXED_FRACBITS);
	qgain = int2q(gain);

  	
	theta_c = qmul( qmul( Q2PI, frequency ), invTheta_s );
	u = qpow( short2q(10), qmul( qgain, inv_1over20 ) ); 
	if(isLow != 0) {
		tmp = qinv( qadd ( Q1, u ) );
		beta = qmul( short2q(4), tmp );
	}

	sigma = qmul( beta, qtan( qmul( theta_c, Q0_5 ) ) );
	tmp = qinv( qadd( Q1, sigma ) );
	gamma = qmul( qsub( Q1, sigma ), tmp );

	if(isLow != 0) {
		t->a0 = qmul( qsub( Q1, gamma ), Q0_5 );
		t->a1 = t->a0;
	}
	else {
		t->a0 = qmul( qadd( Q1, gamma ), Q0_5 );
		t->a1 = -t->a0;
	}
	t->a2 = 0;
	t->b1 = -gamma;
	t->b2 = 0;

	t->c0 = qsub( u, Q1 );
	t->d0 = Q1;
}

// mono
void process_Shelving(ModBiQuad *t, fixedp *x, fixedp *y, Uint32 frames) {
	Uint32 n;

	t->yz1 = qadd( qmul( t->a0, x[0] ), qsub( qmul( t->a1, t->xz1),  qmul( t->b1, t->yz1 ) ) );
	y[0] = qadd( qmul( x[0], t->d0 ), qmul( t->c0, t->yz1 ) );

	for(n=1; n < frames; n++) {
		// output = x[n]*d0 + c0*(biquad)
		// first order though, no x[n-2], just skip it. 
		// biquad = a0*x[n] + a1*x[n-1] - b1*y[n-1]
		// output = x[n]*d0 + c0*(a0*x[n] + a1*x[n-1] - b1*y[n-1])
		// yz^-1 = biquad
		t->yz1 = qadd( qmul( t->a0, x[n] ), qsub(qmul( t->a1, x[n-1]),  qmul( t->b1, t->yz1 ) ) );
		y[n] = qadd( qmul(x[n],t->d0), qmul(t->c0, t->yz1) );
	}

	// set up for next round.
	t->xz1 = x[n-1];
}
