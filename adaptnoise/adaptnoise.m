%Adaptnoise.M Generates: dplusn.h (s312+s1500), refnoise.h cos(312),and sin1500.h 

for i=1:128
  desired(i) = round(100*sin(2*pi*(i-1)*1500/8000)); %sin(1500)
  addnoise(i) = round(100*sin(2*pi*(i-1)*312/8000)); %sin(312)
  refnoise(i) = round(100*cos(2*pi*(i-1)*312/8000)); %cos(312)
end
dplusn = addnoise + desired;							     %sin(312)+sin(1500)

fid=fopen('dplusn.h','w');					              %desired + noise
fprintf(fid,'short dplusn[128]={');
fprintf(fid,'%d, ' ,dplusn(1:127));
fprintf(fid,'%d' ,dplusn(128));
fprintf(fid,'};\n');   
fclose(fid);
      
fid=fopen('refnoise.h','w');								  %reference noise
fprintf(fid,'short refnoise[128]={');
fprintf(fid,'%d, ' ,refnoise(1:127));
fprintf(fid,'%d' ,refnoise(128));
fprintf(fid,'};\n');   
fclose(fid);
   
fid=fopen('sin1500.h','w');								  %desired sin(1500)
fprintf(fid,'short sin1500[128]={');
fprintf(fid,'%d, ' ,desired(1:127));
fprintf(fid,'%d' ,desired(128));
fprintf(fid,'};\n');   
fclose(fid);

   