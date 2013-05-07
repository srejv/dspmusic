
#include "brad_envelopedetector.h"
#include "definebrad.h"

void EnvDetector_setupDetector(EnvelopeDetector *this) {
	fixedp aims, rims;
	aims = qinv(qmul(this->attack_in_ms, _qmul(SAMPLE_RATE, Q_MSEC_TO_S, 0, FIXED_FRACBITS, FIXED_FRACBITS)));
	rims = qinv(qmul(this->release_in_ms, _qmul(SAMPLE_RATE, Q_MSEC_TO_S, 0, FIXED_FRACBITS, FIXED_FRACBITS)));

	this->attack_coef = qexp(qmul(Q_TC_DIGITAL, aims));
	this->release_coef = qexp(qmul(Q_TC_DIGITAL, rims));
	this->envelope = 0;
	
	//this->attack_coef = exp(log(0.01)/(attack_in_ms * samplerate * 0.001));
	//this->release_coef = exp(log(0.01)/(release_in_ms * samplerate * 0.001));
}

fixedp EnvDetector_detect(EnvelopeDetector *this, fixedp *in) {
	fixedp tmp = abs(in[0]);
	if(tmp > this->envelope) {
		// attack
		this->envelope = qadd(qmul(this->attack_in_ms, qsub(this->envelope, in[0])), in[0]);
	}
	else {
		// release
		this->envelope = qadd(qmul(this->release_in_ms, qsub(this->envelope, in[0])), in[0]);
	}
	return this->envelope;
}

