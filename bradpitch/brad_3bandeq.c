//----------------------------------------------------------------------------
//
//                                3 Band EQ :)
//
// EQ.C - Main Source file for 3 band EQ
//
// (c) Neil C / Etanza Systems / 2K6
//
// Shouts / Loves / Moans = etanza at lycos dot co dot uk 
//
// This work is hereby placed in the public domain for all purposes, including
// use in commercial applications.
//
// The author assumes NO RESPONSIBILITY for any problems caused by the use of
// this software.
//
//----------------------------------------------------------------------------

// NOTES :
//
// - Original filter code by Paul Kellet (musicdsp.pdf)
//
// - Uses 4 first order filters in series, should give 24dB per octave
//
// - Now with P4 Denormal fix :)


//----------------------------------------------------------------------------

// ----------
//| Includes |
// ----------


#include "brad_3bandeq.h"
#include "qmath.h"
#include "brad_input.h"

// -----------
//| Constants |
// -----------

//static double vsa = (1.0 / 4294967295.0);   // Very small amount (Denormal Fix)


// ---------------
//| Initialise EQ |
// ---------------

// Recommended frequencies are ...
//
//  lowfreq  = 880  Hz
//  highfreq = 5000 Hz
//
// Set mixfreq to whatever rate your system is using (eg 48Khz)

void init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq)
{
  // Clear state 

  memset(es,0,sizeof(EQSTATE));

  // Set Low/Mid/High gains to unity

  es->lg = 1.0;
  es->mg = 1.0;
  es->hg = 1.0;

  // Calculate filter cutoff frequencies

  es->lf = qmul(Q2, qsin(qmul(QPI, qdiv(int2q(lowfreq), int2q(mixfreq))))); 
  es->hf = qmul(Q2, qsin(qmul(QPI, qdiv(int2q(highfreq), int2q(mixfreq)))));
}


// ---------------
//| EQ one sample |
// ---------------

// - sample can be any range you like :)
//
// Note that the output will depend on the gain settings for each band 
// (especially the bass) so may require clipping before output, but you 
// knew that anyway :)

fixedp do_3band(EQSTATE* es, fixedp sample)
{
  // Locals

  fixedp  l,m,h;      // Low / Mid / High - Sample Values

  // Filter #1 (lowpass)

  es->f1p0  = qadd(es->f1p0, qmul(es->lf, qsub(sample,   es->f1p0)));//  + vsa; might need this but i dont think so. who knows?
  es->f1p1  = qadd(es->f1p1, qmul(es->lf, qsub(es->f1p0, es->f1p1)));
  es->f1p2  = qadd(es->f1p2, qmul(es->lf, qsub(es->f1p1, es->f1p2)));
  es->f1p3  = qadd(es->f1p3, qmul(es->lf, qsub(es->f1p2, es->f1p3)));

  l          = es->f1p3;

  // Filter #2 (highpass)
  
  es->f2p0  = qadd(es->f2p0,qmul(es->hf, qsub(sample   , es->f2p0))); // + vsa;
  es->f2p1  = qadd(es->f2p1,qmul(es->hf, qsub(es->f2p0 , es->f2p1)));
  es->f2p2  = qadd(es->f2p2,qmul(es->hf, qsub(es->f2p1 , es->f2p2)));
  es->f2p3  = qadd(es->f2p3,qmul(es->hf, qsub(es->f2p2 , es->f2p3)));

  h          = qsub(es->sdm3, es->f2p3);

  // Calculate midrange (signal - (low + high))

  m          = qsub(es->sdm3, qadd(h, l));

  // Scale, Combine and store

  l         = qmul(l, es->lg);
  m         = qmul(m, es->mg);
  h         = qmul(h, es->hg);

  // Shuffle history buffer 

  es->sdm3   = es->sdm2;
  es->sdm2   = es->sdm1;
  es->sdm1   = sample;                

  // Return result

  return(qadd(l, qadd(m, h)));
}


//----------------------------------------------------------------------------

void eq_setParam(EQSTATE* es, Uint32 param, int val) {
	static int lowf, midf, highf;
	switch(param) {
	case EQ_ACTIVE: break;
	case EQ_HIGH_GAIN:
		es->hg = val*255;
		break;	
	case EQ_MID_GAIN:
		es->mg = val*255;
		break;
	case EQ_LOW_GAIN:
		es->lg = val*255;
		break;
	case EQ_HIGH_FREQ:
	case EQ_MID_FREQ:
	case EQ_LOW_FREQ:
		lowf = *((int*)EQ_LOW_FREQ);
		midf = *((int*)EQ_MID_FREQ);
		highf = *((int*)EQ_HIGH_FREQ);
		init_3band_state(es, lowf,	midf, highf);
		break;
	}
}
