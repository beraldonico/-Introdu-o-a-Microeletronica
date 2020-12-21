function [y] = cost_function (x)
  
write_netlist_AC(x);
run_netlist_AC;
ganho = read_log_AC;
write_netlist_TRAN(x);
run_netlist_TRAN;
excur = read_log_TRAN;
y = ganho + excur;

endfunction
