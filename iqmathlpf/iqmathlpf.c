#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_MIC; // select input

#include <IQmath.h>

_iq in;
interrupt void c_int11() {
	in = iq16(input_left_sample());
	output_left_sample(in);
}

void main() {
	comm_intr();
	while(1);
}
