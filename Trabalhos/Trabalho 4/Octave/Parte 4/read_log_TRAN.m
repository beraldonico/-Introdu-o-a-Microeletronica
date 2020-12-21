function [excur] = read_log_TRAN ()

meas = 0;
fid=fopen('generated_netlistTRAN.log','r');

line = fgetl(fid);

 while(~feof(fid))
  line = fgetl(fid);
  elements = strsplit(line, {' ', '='});
  if (strcmp(elements{1},'excur:'))
     excur = str2num(elements{3});
  endif
 endwhile
 fclose(fid);

endfunction