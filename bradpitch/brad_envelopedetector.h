/*
http://musicdsp.org/showArchiveComment.php?ArchiveID=136
Envelope follower with different attack and release

References : Posted by Bram

Notes : 
xxxx_in_ms is xxxx in milliseconds ;-)

Code : 
init::

attack_coef = exp(log(0.01)/( attack_in_ms * samplerate * 0.001));
release_coef = exp(log(0.01)/( release_in_ms * samplerate * 0.001));
envelope = 0.0;

loop::

tmp = fabs(in);
if(tmp > envelope)
	envelope = attack_coef * (envelope - tmp) + tmp;
else
	envelope = release_coef * (envelope - tmp) + tmp;
*/

#ifndef BRAD_ENVELOPE_DETECTOR_H_
#define BRAD_ENVELOPE_DETECTOR_H_

#include "qmath.h"

#define Q_MSEC_TO_S 	float2q(0.001f)
#define Q_TC_DIGITAL 	qlog(0.01)
#define Q_TC_ANALOG 	qlog(0.368)

typedef struct EnvelopeDetector_t {
	fixedp attack_coef, release_coef, envelope;
	fixedp attack_in_ms, release_in_ms;
} EnvelopeDetector;

void EnvDetector_setupDetector(EnvelopeDetector *this);

fixedp EnvDetector_detect(EnvelopeDetector *this, fixedp *in);

#endif
