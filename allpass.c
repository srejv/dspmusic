
typedef struct APF_t {
	fixedp frequency = 0;
	BiQuad *filter; 	
	void (* calculateCoeffs)(APF *this, fixedp frequency);
	void (* processAudio)(APF *this, fixedp *input, fixedp *output, unsigned int frames, unsigned char numInChannels, unsigned char numOutChannels);

} APF;

void calculateAPF(APF *this, fixedp frequency) {
	this.frequency = frequency;

	fixedp alpha = ;

	this->filter->a0 = 0;
	this->filter->a1 = 0;
	this->filter->a2 = 0;
	this->filter->b1 = 0;
	this->filter->b2 = 0;
}
void processAudioAPF(APF *this, fixedp *input, fixedp *output, unsigned int frames, unsigned char numInChannels, unsigned char numOutChannels) {
	
}

APF* createAPF() {
	APF* apf = (APF*)malloc(sizeof(APF) + sizeof(BiQuad));
	apf.filter = createBiQuad();
	apf.calculateCoeffs = calculateAPF;

	
}

void calculateParametric

typedef struct Comb_t {
	fixedp g = 0;
	delay?;
}

