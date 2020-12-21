function [ganho] = read_log_AC ()

meas = 0;
fid=fopen('generated_netlistAC.log','r');

line = fgetl(fid);

while(~feof(fid))
  line = fgetl(fid);
  elements = strsplit(line, {' ', '=', '(', 'd'});
  if (strcmp(elements{1},'ganho:'))
     ganho = str2num(elements{4});
  endif
endwhile
fclose(fid);

endfunction