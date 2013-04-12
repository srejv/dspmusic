//pitchshift.c awesome pitch shift
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_48KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input
#include <math.h>

#define BUF_SIZE 2048
#define GAIN 0.9
int i;
float sr = 48000.f;
float gain = 0.9f, pitch = 1.0f, rp1 = 0.f, fdb = 0.f;
//int taps = 2, dsize = 360, wp = 0;
short input,output,delayed;
short delay[BUF_SIZE];
float env[BUF_SIZE];

float rp2 = 0.f;
int ep1 = 0;
int ep2 = 0;
float inc = 0; 
int rpi =0;
float frac = 0.f;
short next = 0.f;
//short pitch = 0;

interrupt void c_int11()	 // interrupt service routine
{
	input = input_left_sample();  // read new input sample from ADC 

	// Envelope pointer1 
	ep1 = (int)(rp1 - i);
	if(ep1 < 0) ep1 += BUF_SIZE;

	// read pointer 2
	rp2 = rp1 + (BUF_SIZE/2);
	if(rp2 >= BUF_SIZE) rp2 -= BUF_SIZE;
	ep2 = (int)(rp2 - i);
	if(ep2 < 0) ep2 += BUF_SIZE;

	rpi = (int)rp1;
	frac = rp1 - rpi;
	next = (rpi != BUF_SIZE-1 ? delay[rpi+1] : delay[0]);

 	delayed = (int)((delay[(int)rp1] + frac*(next-delay[rpi]))*env[ep1]);
 	
	rpi = (int)rp2;
	frac = rp2 - rpi;
	next = (rpi != BUF_SIZE-1 ? delay[rpi+1] : delay[0]);
 	delayed += (int)((delay[(int)rp2] + frac*(next-delay[rpi]))*env[ep2]);          // read delayed value from buffer
 	output = delayed + input;     // output sum of input and delayed values
 	output_left_sample(output);
  //	output_right_sample(output);     
 	
 	delay[i] = input; // + delayed*GAIN; // store new input and a fraction 
  		                            // of the delayed value in buffer
  	if(++i >= BUF_SIZE) i=0;      // test for end of buffer
	rp1 += inc;
	if(rp1 >= BUF_SIZE) rp1 = 0;
}

void main()
{
  	for(i=0 ; i<BUF_SIZE ; i++)
    	delay[i] = 0;
	
	for(i=0; i<BUF_SIZE/2; i++) env[i] = i*2./BUF_SIZE;
	for(i=BUF_SIZE/2; i >= 0; i--) env[BUF_SIZE-i-1] = i*2./BUF_SIZE;
	
	inc = pow(2.0, 7.0/12.0);
 	comm_intr();                  //init DSK, codec, McBSP
 	while(1) {
 		
		inc = pow(2.0, pitch/12.0);
 	}                     //infinite loop
}


/*int rpi, ep;
	float s=0.f,rpf, frac, next, p=(pitch*1.5) + 0.5;
 	input = input_left_sample(); //read new input sample
	for(i=0; i < taps; i++) {
		rpf = rp + i*dsize/taps;
		if(rpf > dsize) rpf -= dsize;
		rpi = (int) rpf;
		frac = rpf - rpi;
		if(rpi != dsize-1) next = delay[rpi+1];
		else next = delay[0];
		// envelop index
		ep = rpi - wp;
		if(ep < 0) ep += dsize;
		s += (delay[rpi] + frac*(next - delay[rpi]))*env[ep];
	}
	// inc reader pointer and check bounds
	rp += p;
	if(rp >= dsize) rp -= dsize;
	// feed the delay line
	delay[wp] = input + s*fdb;
	// output the signal
	output = (s/taps)*gain;
 	//output = input + delayed;    //output sum of new and delayed samples
  	//delay[i] = input;           //replace delayed sample with  
  	//if(++i >= BUF_SIZE) i=0;     //new input sample then increment
  	if(++wp >= dsize) wp = 0;
*/
