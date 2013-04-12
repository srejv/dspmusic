//delay.c Basic time delay
#include "DSK6416_AIC23.h"	        //codec support
Uint32 fs=DSK6416_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6416_AIC23_INPUT_LINE; // select input

#define BUF_SIZE 1024
short input[BUF_SIZE];
short insample,outsample;

#define SAMPLERATE 8000
#define VEC_SIZE 512
short saw_table[VEC_SIZE];


short* Fourier_table(int harms, float *amps, int length, float phase) {
	float a;
	float *table = new float[length+2];
	double w;
	phase *= (float)pi*2;

	memset(table, 0, ( length + 2 ) * sizeof(float) );

	for ( int i=0; i < harms; i++ ) {
		for ( int n = 0; n < length + 2; n++ ) {
			a = amps[i] ? amps[i] : 1.f;
			w = ( i + 1 ) * ( n * 2 * pi / length );
			table[n] += (float) (a * cos( w + phase ) );		
		}
	}

	normalize_table( table, length );
	return table;
}

short* saw_table(int hamrs, int length) {
	float *amps = new float[harms];
	for(int i=0; i < harms; i++) amps[i] = 1.f/(i+1);
	short *table = Fourier_table(harms, amps, length, -0.25);
	delete[] amps;
	return table;
}

short* sqr_table(int harms, int length) {
	float *amps = new float[harms];
	memset(amps, 0, sizeof(float) * harms);
	for(int i=0; i < harms; i += 2) amps[i] = 1.f/(i+1);
	float *table = Fourier_table(harms, amps, length, -0.25);
	delete[] amps;
	return table;
}

short* triang_table(int harms, int length) {
	float *amps = new float[harms];
	memset(amps, 0, sizeof(float) * harms);
	for(int i=0; i < harms; i += 2) amps[i] = 1.f/((i+1)*(i+1));
	float* table = Fourier_table(harms, amps. length);
	delete[] amps;
}

short osc(short *output, float amp, float freq,
	float *table, float *index,
	short length, short vecsize, long sr = SAMPLERATE)
{
	// increment
	float incr = freq * length / sr;
	
	// processing loop
	for(int i = 0; i < vecsize; i++) {
		// truncated lookup
		output[i] = amp * table[(int)(*index)];
		*index += incr;
		while(*index >= length) *index -= length;
		while(*index < 0) *index += length;
	}
	return *output;
}


interrupt void c_int11() {
	// Passthrouh
	insample = input_left_sample();
´	outsample = insample;
	output_left_sample(outsample);
}


void main()
{
	comm_intr();
	while(1);
}

/*short input,output,delayed;
short buffer[BUF_SIZE];
int i;
short delay = 48000;


interrupt void c_int11()	 // interrupt service routine
{
  input = input_left_sample(); //read new input sample
  delayed = buffer[i];         //read output of delay line
  output = input + delayed;    //output sum of new and delayed samples
  buffer[i] = input;           //replace delayed sample with  
  if(++i >= BUF_SIZE) i=0;     //new input sample then increment
  
  output_left_sample(output);  //buffer index
  output_right_sample(output);
  return;                      //return from ISR
}

void main()
{
  for(i=0 ; i<BUF_SIZE ; i++)
    buffer[i] = 0;
  comm_intr();                  //init DSK, codec, McBSP
  while(1);                     //infinite loop
}



*/