function [retval] = write_netlist_TRAN (x)

fid = fopen('generated_netlistTRAN.cir','w');
fprintf(fid,'* Fonte Comum\n');
fprintf(fid,'M1 V_out N001 0 0 N_1u l=%gu w=%gu\n', x(1), x(2));
fprintf(fid,'VDD VDD 0 5\n');
fprintf(fid,'V_gs N001 VGS AC 1\n');
fprintf(fid,'VGS VGS 0 %g\n', x(3));
fprintf(fid,'R_d VDD V_out %g\n', x(4));
fprintf(fid,'.model NMOS NMOS\n');
fprintf(fid,'.model PMOS PMOS\n');
fprintf(fid,'.include cmosedu_models.txt\n');
fprintf(fid,'.tran 20s\n');
fprintf(fid,'.meas TRAN excur FIND V(VDD)-V(VGS)+0.8 AT 0.01s\n');
fprintf(fid,'.end\n');
fclose(fid);
endfunction
