
typedef struct Delay_t {
	float fDelayInSamples;
	float fFeedback;
	float fWetLevel;
	
	void (*cookVariables)(void);
	void (*resetDelay)(void);

	int nReadIndex;
	int nWriteIndex;
	int nBufferSize;
	
	bool bUseExternalFeedback; // flag for enabling/disabling
	float fFeedbackIn;		// user supplied feedback sample value

	float *pBuffer;
} Delay;

// current FB is fb*output
float getCurrentFeedbackOutput() { 
	return fFeedback*pBuffer[nReadIndex]; 
}

// set the feedback sample
void setCurrentFeedbackInput(float f) { fFeedbackIn = f; }

// enable/disable external FB source
void setUsesExternalFeedback(bool b) { bUseExternalFeedback = b; }