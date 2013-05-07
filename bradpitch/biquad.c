
#include "biquad.h"


BiQuad* BiQuad_create() {
	BiQuad* filter = (BiQuad*)malloc(sizeof(BiQuad));
	return filter;
}

void BiQuad_FlushDelays(BiQuad* filter) {
	filter->xz1 = short2q(0);
	filter->xz2 = short2q(0);
	filter->yz1 = short2q(0);
	filter->yz2 = short2q(0);
}

// Do the filter: given input xn, calculate output yn and return it
fixedp BiQuad_do(BiQuad* this, fixedp xn) {
	// just do the difference equation: y(n) = a0x(n) + a1x(n-1) + a2x(n-2) - b1y(n-1) - b2y(n-2)
	fixedp a0 = qmul(this->a0, xn);
	fixedp a1 = qmul(this->a1, this->xz1);
	fixedp yn = qadd(a0, a1);
	a1 = qmul(this->a2, this->xz2);
	yn = qadd(yn, a1);
	a1 = qmul(this->b1, this->yz1);
	yn = qsub(yn, a1);
	a1 = qmul(this->b2, this->yz2);
	yn = qsub(yn, a1);
	//float yn = this->a0*xn + this->a1*this->xz1 + this->a2*this->xz2 - this->b1*this->yz1 - this->b2*this->yz2;

	// underflow check
	
	if(yn > 0.0 && yn < FLT_MIN_PLUS) yn = 0;
	if(yn < 0.0 && yn > FLT_MIN_MINUS) yn = 0;

	// shuffle the delays
	// Y delays
	this->yz2 = this->yz1;
	this->yz1 = yn;

	// X delays
	this->xz2 = this->xz1;
	this->xz1 = xn;

	return yn;
}
