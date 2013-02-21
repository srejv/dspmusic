//delay.c Basic time delay
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_48KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#define BUF_SIZE 8000
short input,output,delayed;
short buffer[BUF_SIZE];
int i;

interrupt void c_int11()	 // interrupt service routine
{
  input = input_left_sample(); //read new input sample
  delayed = buffer[i];         //read output of delay line
  output = input + delayed;    //output sum of new and delayed samples
  output *= 100;
  buffer[i] = input;           //replace delayed sample with  
  if(++i >= BUF_SIZE) i=0;     //new input sample then increment 
  output_left_sample(output);  //buffer index
  return;                      //return from ISR
}

void main()
{
  for(i=0 ; i<BUF_SIZE ; i++)
    buffer[i] = 0;
  comm_intr();                  //init DSK, codec, McBSP
  while(1);                     //infinite loop
}
