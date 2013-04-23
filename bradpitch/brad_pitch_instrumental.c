
#include "qmath.h"
#include "brad_pitch_instrumental.h"
#include "definebrad.h"


void process_pitchshift(PShift *this, fixedp *process) {
	int i;
	fixedp in, out, next;
	fixedp rp2, frac, fi;
	fixedp wetSignal;
	int rpi, ep1, ep2;
	
	fixedp fixedBufSize = short2q(PSHIFT_BUFSIZE);
	fixedp bufOverTwo = 2 * qinv( fixedBufSize ); 
	for(i = 0; i < PROCESS_SIZE; i++ ) {
		in = process[i];
		fi = short2q(i);

		// Envelope pointer1 
		ep1 = (int)qipart( qsub(this->rp1, fi) );
		if(ep1 < 0) ep1 = qadd(ep1, PSHIFT_BUFSIZE);
		
		// Calculate read pointer 2 and envelope pointer 2
		rp2 = qadd(this->rp1, bufOverTwo);
		if(rp2 >= fixedBufSize) rp2 = qsub(rp2, fixedBufSize);
		ep2 = (int)qipart( qsub( rp2, fi ) );
		if(ep2 < 0) ep2 = qadd(ep2, PSHIFT_BUFSIZE);

		// Get signal for read pointer 1
		rpi = (int)qipart( this->rp1 );		
		frac = qfpart( this->rp1 );
		next = (rpi != PSHIFT_BUFSIZE-1 ? this->pbuf[rpi+1] : this->pbuf[0]);
		
		wetSignal = qmul( qadd(this->pbuf[rpi], qmul( frac, qsub( next, this->pbuf[rpi] ) ) ), this->env[ep1] );
	 			
		rpi = (int)qipart( rp2 );
		frac = qfpart( rp2 );
		next = (rpi != PSHIFT_BUFSIZE-1 ? this->pbuf[rpi+1] : this->pbuf[0]);
	 	
		wetSignal = qadd(wetSignal, qmul( qadd(this->pbuf[rpi], qmul( frac, qsub( next, this->pbuf[rpi] ) ) ), this->env[ep2] ));
		
	 	out = qadd(in, wetSignal); //qadd(wetSignal, in);     // output sum of input and delayed values
	 	process[i] = out;
	 	
	 	this->pbuf[this->wp] = qadd(in, qmul(out, this->fdb)); 

	  	if(++this->wp >= PSHIFT_BUFSIZE) this->wp=0;      // test for end of buffer
		this->rp1 += this->inc;
		if(this->rp1 >= fixedBufSize) this->rp1 = 0;
	}
	return;
}

void PShift_setupPitchParams(PShift *this) {
	short i;
	fixedp stepratio;
	fixedp twooverbufsize = 2 * qinv(short2q(PSHIFT_BUFSIZE));
	stepratio = Q1_12

	// Setup pitch shift
	this->rp1 = 0;
	this->inc = qpow(Q2, qmul(short2q(7), stepratio)); // 2^pitch/12
	this->gain = float2q(0.9f);
	this->fdb = 0;
	this->wp = 0;
	
	for( i = 0; i < 1024; i++ ) 
		this->env[i] = i * twooverbufsize;
	for( i = 1024; i >= 0; i--) 
		this->env[PSHIFT_BUFSIZE-i-1] = i * twooverbufsize;
}

void PShift_setPitchStep(PShift *this, short step) {
	fixedp fstep = short2q(step);
	fixedp stepratio = Q1_12;
	this->inc =  qpow(Q2, qmul(fstep, stepratio)); // 2^pitch/12
}
