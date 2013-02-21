%sin1500.m Generates 128 points representing sin(1500) Hz
%Creates file sin1500.h
for i=1:128
  sine(i) = round(1000*sin(2*pi*(i-1)*1500/8000)); %sin(1500)
end

fid = fopen('sin1500.h','w');		     %open/create file
fprintf(fid,'short sin1500[128]={');  %print array name,"={"
fprintf(fid,'%d, ' ,sine(1:127));     %print 127 points
fprintf(fid,'%d' ,sine(128));         %print 128th point
fprintf(fid,'};\n');                  %print closing bracket
fclose(fid);								  %close file

   