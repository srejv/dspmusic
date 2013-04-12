#include <stdlib.h>
#include "sine.h"
#include "coeff.cof"

#include "filter.h"

short mem1[COEFF],mem2[COEFF];

void ProcBlock(volatile short *input, volatile short *output, int size)
{
	short *buff;
	buff = (short *) malloc(size*sizeof(short));
	
	filter(input, buff,size,mem1,coeff,COEFF);
	MultSine(buff,size);
	filter(buff,output,size,mem2,coeff,COEFF);
	free(buff);
} 


void filter(volatile short *inp, volatile short *outp,int size, short *mem,const short *coef, const short order)
{
	int i,j;
	long acc;
	
 	for (i = 0; i < size; i++)
	{
		mem[order-1] = inp[i];
		acc = mem[0] * coef[0];
		
		for (j = 1; j < order; j++)
		{
			acc += mem[j] * coef[j];
			mem[j-1] = mem[j];
		}
		
		outp[i] = ((acc)>>16) & 0xfffe;
	}                  	 
} 


void MultSine(volatile short *buffer,int size)
{
    static int j;
    int i;
    for(i=0; i< size; i++)
    {
    	buffer[i] = (buffer[i] * sine[j++]) >> 12;
    	if(j>=80) j = 0; 
    }
}
