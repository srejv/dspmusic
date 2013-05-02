//---------------------------------------------------------------------------
//
//                                3 Band EQ :)
//
// EQ.H - Header file for 3 band EQ
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

#ifndef __EQ3BAND__
#define __EQ3BAND__

#include "qmath.h"

// ------------
//| Structures |
// ------------

typedef struct
{
  // Filter #1 (Low band)

  fixedp  lf;       // Frequency
  fixedp  f1p0;     // Poles ...
  fixedp  f1p1;     
  fixedp  f1p2;
  fixedp  f1p3;

  // Filter #2 (High band)

  fixedp  hf;       // Frequency
  fixedp  f2p0;     // Poles ...
  fixedp  f2p1;
  fixedp  f2p2;
  fixedp  f2p3;

  // Sample history buffer

  fixedp  sdm1;     // Sample data minus 1
  fixedp  sdm2;     //                   2
  fixedp  sdm3;     //                   3

  // Gain Controls

  fixedp  lg;       // low  gain
  fixedp  mg;       // mid  gain
  fixedp  hg;       // high gain
  
} EQSTATE;  


// ---------
//| Exports |
// ---------

extern void   init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq);
extern fixedp do_3band(EQSTATE* es, fixedp sample);


#endif // #ifndef __EQ3BAND__
//---------------------------------------------------------------------------

