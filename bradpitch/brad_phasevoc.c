
#include "qmath.h"
#include "tistdtypes.h"
#include "definebrad.h"
#include <dsp_fft.h>
#include <dsp_fft32x32.h>
#include "twiddle.coef"

//#define FRAME_SIZE		2048
//#define SEGMENT_SIZE 	512
// Storleken (antal mätpunkter) på de segment vi delar upp signalen i.
#define FRAME_SIZE 1024
//  Anger överlappning av segment.
#define SEGMENT_OVERLAP 4
#define SEGMENTS 8 // calculated by hand. sorry lol
//  Storleken på mellanrummet mellan varje segment.
#define HOP_SIZE FRAME_SIZE/SEGMENT_OVERLAP

#define toneShift (0x80001000) 

#define PSHIFT_TONE(x) qpow(Q2, qmul(x, Q1_12  ))
#define PSHIFT_CENT(x) qpow(Q2, qmul(x, Q1_1200))
#define round(x) (qfpart(x) < Q0_5 ? qipart(x) : (qipart(x) + Q1))

#include "hann.coef"

// % Storlek på buffer
#define BUFFER_SIZE		2048
#define QIFRAME_SIZE	qinv(int2q(FRAME_SIZE))
#define QIHOP_SIZE		qinv(int2q(HOP_SIZE))

#define REMOVE_PHASE_DIFFERENCE(dPhi, n) (qsub(dPhi, qmul(qmul(HOP_SIZE, Q2PI), qmul( qmul(n, Q0_5), QIFRAME_SIZE)))
#define PHASE_MOD(phi)  	(((phi + QPI) % Q2PI) - QPI)
#define TRUE_FREQ(n, phi)	 (qadd(qmul(Q2PI, _qmul(n, QIFRAME_SIZE, 0, FIXED_FRACBITS, FIXED_FRACBITS)), qmul(dpmPhi,QIHOP_SIZE)))))

#define QSQUARE (x) qmul(x,x)

typedef struct COMPLEX_T {
	Int32 real;
	Int32 imag;
} COMPLEX;

#define COMPLEX_1i ((COMPLEX){ .real = 0, .imag = 1 })
#define COMPLEX_minus1i ((COMPLEX){ .real = 0, .imag = -1})

/* based on:
 Streamlining digital signal processing: a tricks of the trade guidebook
 av Lyons, Richard G
 2012, 2nd ed., ISBN 1118316940
 eq. 27-12, 

 arctan(x) almost= (pi/4)*x - x*(abs(x) - 1)*(0.2447 + 0.0663*abs(x))
*/
#define ATANCOEF1 double2q(0.2447)
#define ATANCOEF2 double2q(0.0663);
fixedp qFastArcTan(fixedp x)
{
    return qsub(qmul(QPIO4, x), qmul(x, qmul(qsub(qabs(x), Q1), (qadd(ATANCOEF1, qmul(ATANCOEF2,qabs(x)))))));
}


fixedp qatan2(fixedp y, fixedp x) {
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

typedef struct PhaseVocoder_t {
	Uint32 hopSizeOut;
	Uint32 pShift;

	fixedp rp;
	Uint32 wp;
} PhaseVocoder;

void setup_phasevocoder(PhaseVocoder *t, fixedp toneShift);

void setup_phasevocoder(PhaseVocoder *t, fixedp toneShift) {

	//% Storleken på mellanrummet mellan varje segment hos utsignalen.
	t->hopSizeOut = _qmul(PSHIFT_TONE(toneShift), HOP_SIZE, FIXED_FRACBITS, 0, 0));
	//% Korrigerar för avrundning i hopSizeOut. what?
	t->pShift = hopSizeOut / HOP_SIZE;
}

static fixedp INPUT_BUFFER_N[BUFFER_SIZE];
static fixedp OUTPUT_BUFFER_N[BUFFER_SIZE];

void process_readPitchBuffer() {
	// Läs från buffern

	// input buffern är full, 
	// gör fftmojäng

}

void process_PitchFreq(PhaseVocoder* t, fixedp *x, fixedp *y) {
	fixedp R, angle, Phi, dPhi, dpPhi, dpmPhi;
	fixedp trueFreq;
	
	Uint32 n, i;
	Uint32 frameSize;
	Uint32 segSize;
	
	COMPLEX segments[4][FRAME_SIZE]; // ?
	COMPLEX fftFrame[FRAME_SIZE];
	COMPLEX preOutputFrame[FRAME_SIZE];
	Uint32 pPhi[FRAME_SIZE];
	Uint32 phiCulum[FRAME_SIZE];
	
	// Dela upp i segment?
	Uint32 rp = 0;
	for(n=0; n < SEGMENTS; n++) {
		for(i = 0; i < FRAME_SIZE; i++) {
			segments[n][i].real = x[rp++] * HANN[i];
			segments[n][i].imag = 0;
		}
		rp = rp - FRAME_SIZE + HOP_SIZE;
	}
	
	
	//dsp_fft32x32(w, N, input, output);
	dsp_fft32x32(w, FRAME_SIZE, segments[0], fftFrame);
	
	for(n=0; n < FRAME_SIZE; n++) {
		// absolutbelopp och fasvinkel
		R = qsqrt(qadd(QSQUARE(fftFrame[n].real), QSQUARE(fftFrame[n].imag)));
		phi = qatan2(fftFrame[n].imag, fftFrame[n].real);

		// skillnad i nuvarande fas med förra
		dPhi = phi - pPhi[n];
		pPhi[n] = phi;
		
		// bort med fasskillnad
		dpPhi = REMOVE_PHASE_DIFFERENCE(dPhi, n); // dPhi - HOP_SIZE*Q2PI*(n/2)/FRAME_SIZE
		dpmPhi = PHASE_MOD(dpPhi);
		
		trueFreq = TRUE_FREQ(n, dpmPhi);
		
		// sparas detta mellan frames?
		phiCulm[n] += _qmul(t->hopSizeOut, trueFreq, 0, FIXED_FRACBITS, FIXED_FRACBITS);

		preOutputFrame[n].real = qmul(R, qcos(phiCulm)); //R*exp(1i*phiCulm);
		preOutputFrame[n].imag = qmul(R, qsin(phiCulm));
	}

	dsp_ifft32x32(w, FRAME_SIZE, preOutputFrame, outputFrame);


	for(n = 0; n < FRAME_SIZE; n++) {
		q = hopOutIndex+n;
		if( q > PROCESS_SIZE) {
			q -= PROESS_SIZE;
		}
		if ( n < FRAME_SIZE-HOP_SIZE_OUT) {
			outputBuffer[q] = outputBuffer[q] + _qmul(outputFrame[n], HANN[n], 31, FIXED_FRACBITS, FIXED_FRACBITS));
		}
		else {
			outputBuffer[q] = (outputFrame[n] * HANN[n]);
		}
	}

	t->hopOutIndex = hopOutIndex + hopSizeOut;
	if (t->hopOutIndex > BUFFER_SIZE) {
		t->hopOutIndex = t->hopOutIndex - BUFFER_SIZE;
	}

	rpi = round(rp);

	if (readyToRead) {
		rp = rp + t->pShift;
	}

	if (round(rp) > BUFFER_SIZE) {
		rp = rp-buffersize;
	}
}