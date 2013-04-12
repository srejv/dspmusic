//delay.c Basic time delay
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input

#define BUF_SIZE 8000
float sr = 48000.f;
short input,output,delayed;
short delay[BUF_SIZE];
int i;
float env[BUF_SIZE];
float gain, pitch, rp, fdb;
int taps, dsize, wp;

gain = 0.5;
pitch = 1.0;
fdb = 0.0;
taps = 2;
dsize = (int)(0.045*sr);

wp = 0;
rp = 0.f;

void setDsize(float d) {
	dsize = (int)(sr*d);
	for(i = 0; i < dsize/2; i++) env[i] = i*2./dsize;
	for(i=dsize/2; i>= 0; i--) env[dsize-i-1] = i*2./dsize;
}

int rpi, ep;
float s=0.f,rpf, frac, next, p=(pitch*1.5) + 0.5;
interrupt void c_int11()	 // interrupt service routine
{
 	input = input_left_sample(); //read new input sample
	for(i=0; i < taps; i++) {
		rpf = rp + i*size/taps;
		if(rpf > dsize) rpf -= disze;
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
	rp = rp < dsize ? rp : rp - dsize;
	// feed the delay line
	delay[wp] = input + s*fdb;
	// output the signal
	output = (s/taps)*gain;
 	//output = input + delayed;    //output sum of new and delayed samples
  	//delay[i] = input;           //replace delayed sample with  
  	//if(++i >= BUF_SIZE) i=0;     //new input sample then increment
  	if(++wp >= dsize) wp = 0;
  	 
  	output_left_sample(output);  //buffer index
  	output_right_sample(output);
  	return;                      //return from ISR
}

void main()
{
  	for(i=0 ; i<BUF_SIZE ; i++)
    	buffer[i] = 0;
	
	for(i=0; i<dsize/2; i++) env[i] = i*2./dsize;
	for(i=dsize/2; i >= 0; i--) env[dsize-i-1] = i*2./dsize;
	
	rp = 0.;
	wp = 0;

 	comm_intr();                  //init DSK, codec, McBSP
 	while(1);                     //infinite loop
}
