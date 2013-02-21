//sinegen_table.c generates a sinusoid using a look-up table 

#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_48KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input
#include <math.h>
#define table_size (short)10	 //set table size
short sine_table[table_size];	 //sine table array
int i;
#define LEFT 1
#define RIGHT 0
union {
	Uint32 uint;
	short channel[2];
} AIC_da;

interrupt void c_int11()	 //interrupt service routine
{
 //output_left_sample(sine_table[i]); //output each sine value
 AIC_da.uint = input_sample();
 
 AIC_da.channel[LEFT]  = sine_table[i];
 AIC_da.channel[RIGHT] = sine_table[i];
 
 output_sample(AIC_da);
 if (i < table_size - 1) ++i;  //incr index until end of table
    else i = 0;			 //reinit index if end of table
 return;				 //return from interrupt
}
 
void main()
{  
float pi=3.14159;

for(i = 0; i < table_size; i++)
  sine_table[i]=10000*sin(2.0*pi*i/table_size); //scaled values

i = 0;
comm_intr();                   //init DSK, codec, McBSP
while(1);                      //infinite loop
}
