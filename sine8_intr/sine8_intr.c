//sine8_intr.c Sine generation using lookup table
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#define _MATH_DEFINES_DEFINED
#include <math.h>
#define M_PI 3.14159265359
#include <limits.h>

#include "biquad.h"
#include "csl_hpi.h"

#define LOOPLENGTH 8        // size of look up table
short sine_table[LOOPLENGTH]={0,7071,10000,7071,0,-7071,-10000,-7071};
short loopindex = 0;   		// look up table index
#define GAINADRESS 0x80001000
#define PARAM1 0x00
#define PARAM2 0x02
#define PARAM3 0x04
#define PARAM4 0x06
#define PARAM5 0x08
#define PARAM6 0x0A
#define PARAM7 0x0C
#define PARAM8 0x0E


int HPI_FLAG = 0;
BiQuad* lpf;
float fc = 200.0;
float Q = 1.707;

void calculateLPFCoeffs(BiQuad* filter) {
	float theta_c = 2*M_PI*fc/8000;
	float d = 1/Q;

	float betaNumerator = 1.0f - (d*0.5f)*sin(theta_c);
	float betaDenominator = 1.0f + (d*0.5f)*sin(theta_c);
	float beta = 0.5f*(betaNumerator/betaDenominator);

	float gamma = (0.5f + beta)*cos(theta_c);

	filter->a0 = float2q((0.5f + beta - gamma)*0.5f);
	filter->a1 = float2q(0.5f + beta - gamma);
	filter->a2 = float2q((0.5f + beta - gamma)*0.5f);
	filter->b1 = float2q(-2.0f*gamma);
	filter->b2 = float2q(2.0f * beta);
}



short chin;
fixedp in;
void c_int11()    //interrupt service routine
{
	int gain = (int)(*(unsigned volatile int *)GAINADRESS);

	in = (fixedp)input_left_sample(); 
	in = BiQuad_do(lpf, in);
	
	chin = qfpart(in);
	output_left_sample(chin);

	/*if(HPI_FLAG) { 
  		//output_left_sample(f * 16000.0); //gain*sine_table[loopindex]);	// output look up table value
  	}
	else {
		//output_left_sample(sine_table[loopindex]);
	}*/
  	if (++loopindex >= LOOPLENGTH) loopindex = 0; // check for end of look up table
  	return;					//return from interrupt
}

void main()
{
	*(unsigned volatile int *)GAINADRESS = 0x0005;
	HPI_FLAG = HPI_SUPPORT;
	lpf = BiQuad_create();
	//Uint32 controlRegister = HPI_ADDR(HPIC);
	BiQuad_FlushDelays(lpf);
	calculateLPFCoeffs(lpf);
  	comm_intr();              // initialise DSK
}
