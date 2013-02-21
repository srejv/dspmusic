%mat63.m
nfreq=[0 0.1 0.12 0.18 0.2 0.3 0.32 0.38 0.4 1];
mag = [0 0 1 1 0 0 1 1 0 0];
bp63 = firpm(62,nfreq,mag);
[h,w]=freqz(bp63,1,512);
plot(nfreq,mag,'b-')
hold on
plot(w/pi,abs(h),'r')
xlabel('normalized frequency');
ylabel('magnitude');

