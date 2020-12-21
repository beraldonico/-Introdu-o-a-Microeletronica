function [retval] = write_netlist_AC (x)

fid = fopen('generated_netlistAC.cir','w');
fprintf(fid,'* Fonte Comum\n');
fprintf(fid,'M1 V_out N001 0 0 N_1u l=%gu w=%gu\n', x(1), x(2));
fprintf(fid,'VDD VDD 0 5\n');
fprintf(fid,'V_gs N001 VGS AC 1\n');
fprintf(fid,'VGS VGS 0 %g\n', x(3));
fprintf(fid,'R_d VDD V_out %g\n', x(4));
fprintf(fid,'.model NMOS NMOS\n');
fprintf(fid,'.model PMOS PMOS\n');
fprintf(fid,'.include cmosedu_models.txt\n');
fprintf(fid,'.ac dec 10 1 1meg\n');
fprintf(fid,'.meas AC ganho FIND V(V_out) AT 1\n');
fprintf(fid,'.end\n');
fclose(fid);
endfunction
