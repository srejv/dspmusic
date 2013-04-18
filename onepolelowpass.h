


#define ONE short2q(1);

typedef struct COnePoleLPF_t {
	//	y(n) =  x(n)*(1-g) + x(n-1)*g
	fixedp g;
	fixedp z1_left;
	fixedp z1_right;

	void (* processAudio)(COnePoleLPF *this, fixedp *input, fixedp *output, unsigned int frames, unsigned char numInChannels, unsigned char numOutChannels); 
} COnePoleLPF;

/* implemented on the form
//	y(n) =  (1-g)*x(n) + g*x(n-1)
// H(z) = a0+a1*z^-1
// a1 = g
// a0 = 1 - g  */
void processAudioOnePoleLPF(COnePoleLPF *this, fixedp *input, fixedp *output, unsigned int frames, unsigned char numInChannels, unsigned char numOutChannels) {
	int i;

	for(i = 0; i < frames; i += numOutChannels) {

		output[i+0] = (ONE-this->g)*input[i+0] + this->g*this->z1_left;
		this->z1_left = input[i+0];
		
		if(numOutChannels == 2) {
			if(numInChannels == 2) {
				// Stereo in, Stereo out
				output[i+1] = (ONE - this->g)*input[i+1] + this->g*this->z1_right;
				this->z1_right = input[i+1];
			} else {
				// Mono in, Stereo out
				output[i+1] = output[i+0];
			}
		}
	}
}

COnePoleLPF* OnePoleLPF_create() { 
	// Is this really a good idea?
	COnePoleLPF *lpf = (COnePoleLPF*)malloc(sizeof(COnePoleLPF));
	lpf.processAudio = processAudioOnePoleLPF;
	return lpf;
}
