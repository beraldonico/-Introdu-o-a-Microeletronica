function [meas] = read_log ()

meas = 0;
fid=fopen('generated_netlist.log','r');

line = fgetl(fid);

while(~feof(fid))
  line = fgetl(fid);
  elements = strsplit(line, {' ', '=', '(', 'd'});
  if (strcmp(elements{1},'ganho:'))
     meas = str2num(elements{4});
  endif
endwhile
fclose(fid);

endfunction