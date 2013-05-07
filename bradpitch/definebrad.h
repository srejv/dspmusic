

#ifndef DEFINE_BRAD_H_
#define DEFINE_BRAD_H_

#include "qmath.h"
#include <tistdtypes.h>

#define SAMPLE_RATE 48000
#define PROCESS_SIZE 256

#define AUDIOMAX 32767
#define AUDIOMIN -32767

#define _Q0_2	0.2
#define Q0_2 	double2q(_Q0_2)

extern Uint32 HANN[1024];

/* Memory Area Defintions */

/* Tremolo */

/* Vibrato */

/* Pitch Shift */
// Unsure about the accuracy of this one. nvhdä.
inline fixedp lagrpol(fixedp *x, fixedp *y, int n, fixedp xbar) {
	int i,j;
	fixedp fx = 0.0;
	fixedp l = Q1;
	fixedp tmp;
	for(i=0; i<n; i++)
	{
		l = Q1;
		for(j=0; j < n; j++)
		{
			if(j != i) {
				tmp = qinv( qsub( x[i], x[j] ) );
				l = qmul( l, qmul( qsub(xbar,x[j]), tmp ) );
			}
		}
		fx = qadd(fx, qmul(l, y[i]));
	}
	return fx;
}
 
 #endif
