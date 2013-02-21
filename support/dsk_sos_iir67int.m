% DSK_SOS_IIR67INT.M
% INTEGER VERSION
% MATLAB function to write SOS IIR filter coefficients
% in format suitable for use in C6713 DSK program
% iir.c
% assumes that coefficients have been exported from
% fdatool as two matrices
% first matrix has format
% [ b10 b11 b12 a10 a11 a12
%   b20 b21 b22 a20 a21 a22
%   ...
% ] 
% where bij is the bj coefficient in the ith stage
% second matrix contains gains for each stage
%
function dsk_sos_iir67int(coeff,gain)
%
num_sections=length(gain)-1;
fname = input('enter filename for coefficients ','s');
fid = fopen(fname,'wt');
fprintf(fid,'// %s\n',fname);
fprintf(fid,'// this file was generated automatically using function dsk_sos_iir67int.m\n',fname);
fprintf(fid,'\n#define NUM_SECTIONS %d\n',num_sections);
% first write the numerator coefficients b
% i is used to count through sections
fprintf(fid,'\nint b[NUM_SECTIONS][3] = { \n');
for i=1:num_sections
  if i==num_sections
      fprintf(fid,'{%d, %d, %d} };\n',round(coeff(i,1)*gain(i)*2^15),round(coeff(i,2)*gain(i)*2^15),round(coeff(i,3)*gain(i)*2^15));
  else
      fprintf(fid,'{%d, %d, %d},\n',round(coeff(i,1)*gain(i)*2^15),round(coeff(i,2)*gain(i)*2^15),round(coeff(i,3)*gain(i)*2^15));
  end
end
% then write the denominator coefficients a
% i is used to count through sections
fprintf(fid,'\nint a[NUM_SECTIONS][2] = { \n');
for i=1:num_sections
  if i==num_sections
      fprintf(fid,'{%d, %d} };\n',round(coeff(i,5)*2^15),round(coeff(i,6)*2^15));
  else
      fprintf(fid,'{%d, %d},\n',round(coeff(i,5)*2^15),round(coeff(i,6)*2^15));
  end
end
fclose(fid);          
