
// A delay
// 
// - fixedp
// - fractional
// - linear interpolation
// - memory from..?
// - support for modulated read speed (write fixed)

typedef struct CDelay_t {
	
	// The delay
	Uint32 delayInSamples;
	fixedp delayInmSec;

	fixedp readIndex;
	Uint32 writeIndex;
	Uint32 size;
	fixedp *pBuffer;
} CDelay;