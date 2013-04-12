#ifndef FILTER
#define FILTER


void ProcBlock(volatile short *input, volatile short *output,int size);
void filter(volatile short *inp, volatile short *outp,int size, short *mem,const short *coef, const short order);
void MultSine(volatile short *buffer, int size);

#endif
              

 
