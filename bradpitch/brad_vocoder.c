
#include "qmath.h"

/*
#define PHASE_VOCODER_BUFFER_SIZE 2048
#define PHASE_VOCODER_OVERLAP 4
#define PHASE_VOCODER_FRAME_SIZE 1024

typedef struct PhaseVocoder_t {
	
	fixedp pShift;
	fixedp prevPhi;
	fixedp phiCumulative;
	Uint32 hopIndex;
	Uint32 hopOutIndex;
	Uint32 hopSize;
	Uint32 hopSizeOut;
	Uint32 wp;	// write buffer index
	fixedp rp;	// read buffer index
	Bool readyToRead;	// ready to read the buffer
	
	fixedp *window;
	fixedp *frame;
	
} PhaseVocoder;

void setupVocoderShift(PhaseVocoder *this, fixedp *framebuffer, fixedp *window) {
	this->frame = framebuffer;
	this->hopIndex = 0;
	this->hopOutIndex = 0;
	this->hopSize = 4;
	this->hopSizeOut = 12;
	this->phiCumulative = 0;
	this->prevPhi = 0;
	this->pShift = 1;
	this->readyToRead = 0;
	this->rp = 0;
	this->window = window;
	this->wp = 0;
}*/

// function [output] = pitchShiftRT(x, toneShift)

// x = Insignal
// toneShift = Antal tonsteg vi ändrar.

// Storleken (antal mätpunkter) på de segment vi delar upp signalen i.
Uint32 frameSize = 1024;
// Anger överlappning av segment.
Uint32 overlap = 4;
// Storleken på mellanrummet mellan varje segment.
Uint32 hopSize = frameSize/overlap;
// Frekvensändring.
fixedp pShift = 2^(toneShift/12);
// Storleken på mellanrummet mellan varje segment hos utsignalen.
hopSizeOut = round(pShift*hopSize);
// Hann-fönster, minskar utsmetning i frekvensplanet.
window = hann(frameSize);
// Storlek på buffer
bufferSize = 2048;
// Cirkulär buffer
buffer = zeros(bufferSize,1);
// Cirkulär output-buffer
outputBuffer = zeros(bufferSize,1);

// Utsignal
y = zeros(length(x),1);

// Segment
frame = zeros(frameSize,1);

prevPhi = 0;
phiCumulative = 0;
// Write-point, var i buffern vi skriver.
wp = 1;
// håller reda på när vi ska bearbeta ett nytt segment.
hopIndex = -frameSize+hopSize;
// Håller reda på var i output-buffern vi ska börja skriva varje nytt segment.
hopOutIndex = 1;
// Read-point, var i output-buffern vi läser.
rp = 1;
// Anger om vi har börjat skriva till output-buffern eller inte.
Bool readyToRead = 0;



void process_VocoderShift(PhaseVocoder *this, fixedp *process) {
	% Loop, simulerar realtidsanvändning.
for k = 1:length(x)
    % Skriver till buffern.
    buffer(wp) = x(k);
    hopIndex = hopIndex+1;
    % Bearbetar ett frame. --------------------------------------------
    if (hopIndex > hopSize)
        readyToRead = 1;
        hopIndex = hopIndex-hopSize;
        % Skapar segment från buffern.
        for m = 1:frameSize
            q = wp-frameSize+m;
            if q < 1
                q = q+bufferSize;
            end
            frame(m) = buffer(q);
        end
        % Segment multiplicerat med Hann-fönster.
        windowedFrame = frame'.*window'/sqrt(2);
        % Gör fouriertransform (FFT).
        frameFFT = fft(windowedFrame);
        % Delar upp i absolutbelopp och fasvinkel.
        r = abs(frameFFT);
        phi = angle(frameFFT);
        % Skillnad i fas mellan nuvarande och föregående fönster.
        deltaPhi = phi-prevPhi;
        prevPhi = phi;
        % Tar bort den förvänta fasskillnaden.
        deltaPhiPrime = deltaPhi-hopSize*2*pi*(0:(frameSize-1))/frameSize;
        % Sätter fasskillnaden mellan -pi och pi.
        deltaPhiPrimeMod = mod(deltaPhiPrime+pi, 2*pi) - pi; 
        % Tar fram den sanna frekvensen.
        trueFreq = 2*pi*(0:(frameSize-1))/frameSize + deltaPhiPrimeMod/hopSize;
        % Fas i utsignalen.
        phiCumulative = phiCumulative+hopSizeOut*trueFreq;
        % Skapa frame till output-buffern.
        outputFrame = real(ifft(r.*exp(1i*phiCumulative))).*window'/sqrt(2);
        % Skriver frame till output-buffern
        for m = 1:frameSize
            q = hopOutIndex+m;
            if q > bufferSize
                q = q-bufferSize;
            end
            if m < frameSize-hopSizeOut
                outputBuffer(q) = outputBuffer(q)+outputFrame(m);
            else
                outputBuffer(q) = outputFrame(m);
            end
        end
        hopOutIndex = hopOutIndex+hopSizeOut;
        if hopOutIndex > bufferSize
            hopOutIndex = hopOutIndex-bufferSize;
        end
    end
    % -----------------------------------------------------------------
    rpi = round(rp);
    if rpi < 1
        rpi = 1;
    end
    % Skriver till output.
    y(k) = outputBuffer(rpi);
    wp = wp+1;
    if wp > bufferSize
        wp = wp-bufferSize;
    end
    % Läser med en hastighet som beror på storleken på transponeringen.
    if readyToRead == 1
        rp = rp+pShift;
    end
    if round(rp) > bufferSize
        rp = rp-bufferSize;
    end
end
}

