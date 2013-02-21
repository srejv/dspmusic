%mat33.m
nfreq=[0 0.1 0.15 0.25 0.3 1];
mag = [0 0 1 1 0  0];
bp33 = firpm(32,nfreq,mag);
[h,w]=freqz(bp33,1,512);
plot(nfreq,mag,'b-')
hold on
plot(w/pi,abs(h),'r')
xlabel('normalized frequency');
ylabel('magnitude');

