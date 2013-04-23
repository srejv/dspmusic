
#ifndef DEFINE_BRAD_H_
#define DEFINE_BRAD_H_

#define SAMPLE_RATE 48000
#define PROCESS_SIZE 256

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
