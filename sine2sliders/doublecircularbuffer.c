


#define BUF_SIZE 1024
short inputBuffer[BUF_SIZE];
short doubleBuffer[BUF_SIZE];
short doubleOutputBuffer[BUF_SIZE];
short outputBuffer[BUF_SIZE];

short index = 0;
short leftInput;

short filledBufferFlag = 0;

void processDoubleBuffer() {
	int i;
	for(i = 0; i < BUF_SIZE; i++) {
		// Process
		doubleOutputBuffer[i] = doubleBuffer[i] * 2;
	}
}

interrupt void c_int11() {
	// Take the input and send the output
	leftInput = input_left_sample();
	inputBuffer[index] = input;
	output_left_sample(outputBuffer[index]);

	// If it's done a whole loop
	if(++index >= BUF_SIZE) {
		short *tmpAdr;
		// Switch the input buffers
		tmpAdr = &inputBuffer[0];
		&inputBuffer[0] = &doubleBuffer[0];
		&doubleBuffer[0] = tmpAdr;

		// Switch the output buffers
		tmpAdr = &outputBuffer[0];
		&outputBuffer[0] = &doubleOutputBuffer[0];
		&doubleOutputBuffer[0] = tmpAdr;
		index = 0;
	}
}

void main() {
	int i;
	// Set all to zero
	for(i = 0; i < BUF_SIZE; i++) {
		inputBuffer[i] = 0;
		doubleBuffer[i] = 0;
		doubleOutputBuffer[i] = 0;
		outputBuffer[i] = 0;
	}
	comm_intr();
	while(1) {
		while(filledBufferFlag == 0) ;
		processDoubleBuffer();
		filledBufferFlag = 0;
	}	
}