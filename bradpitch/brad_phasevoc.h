


#pragma once

#include "qmath.h"
#include <tistdtypes.h>

//#define FRAME_SIZE		2048
//#define SEGMENT_SIZE 	512
// Storleken (antal mätpunkter) på de segment vi delar upp signalen i.
#define FRAME_SIZE 1024
//  Anger överlappning av segment.
#define SEGMENT_OVERLAP 4
#define SEGMENTS 		8 // calculated by hand. sorry lol
//  Storleken på mellanrummet mellan varje segment.
#define HOP_SIZE 		FRAME_SIZE/SEGMENT_OVERLAP

//#define toneShift (0x80001000) 

#define round(x) (qfpart(x) < Q0_5 ? qipart(x) : (qipart(x) + Q1))

// % Storlek på buffer
#define BUFFER_SIZE		2048
#define QIFRAME_SIZE	qinv(int2q(FRAME_SIZE))
#define QIHOP_SIZE		qinv(int2q(HOP_SIZE))

#define REMOVE_PHASE_DIFFERENCE(dPhi, n)	(qsub(dPhi, qmul(qmul(HOP_SIZE, Q2PI), qmul( qmul(n, Q0_5), QIFRAME_SIZE))))
#define PHASE_MOD(phi)  					qsub(qmod(qadd(phi, QPI), Q2PI), QPI)
#define TRUE_FREQ(n, phi)	 				(qadd(qmul(Q2PI, _qmul(n, QIFRAME_SIZE, 0, FIXED_FRACBITS, FIXED_FRACBITS)), qmul(dpmPhi,QIHOP_SIZE)))

#define QSQUARE(x) qmul(x,x)


typedef struct COMPLEX_T {
	Int32 r;
	Int32 i;
} COMPLEX;

/* based on:
 Streamlining digital signal processing: a tricks of the trade guidebook
 av Lyons, Richard G
 2012, 2nd ed., ISBN 1118316940
 eq. 27-12, 

 arctan(x) almost= (pi/4)*x - x*(abs(x) - 1)*(0.2447 + 0.0663*abs(x))
*/
#define ATANCOEF1 double2q(0.2447)
#define ATANCOEF2 double2q(0.0663)
inline fixedp qFastArcTan(fixedp x)
{
    return qsub(qmul(QPIO4, x), qmul(x, qmul(qsub(qabs(x), Q1), (qadd(ATANCOEF1, qmul(ATANCOEF2,qabs(x)))))));
}

inline fixedp qatan2(fixedp y, fixedp x) {
	fixedp tmp;
	if(x == 0) {
		
		if(y == 0) { return 0; }
		if(y > 0) { return QPIO2; }

		// y < 0
		return -QPIO2;
	}

	if(x > 0) {
		tmp = qinv(x);
		return qFastArcTan(qmul(y,tmp));
	}

	// x < 0
	tmp = qinv(x);
	tmp = qFastArcTan(qmul(y,tmp));
	// tmp now contains atan(y/x)
	if(y < 0) return tmp - QPI;
	return tmp + QPI;
}

typedef struct {
	Uint32 hopSizeOut;
	Uint32 pShift;
	
	Uint32 hopOutIndex;
	fixedp rp;
	Uint32 wp;

} PhaseVocoder;


void phaseVocoder_initialize();
void phaseVocoder_setup(PhaseVocoder *t, int toneShift);
void phaseVocoder_process(PhaseVocoder *t, fixedp *input);
