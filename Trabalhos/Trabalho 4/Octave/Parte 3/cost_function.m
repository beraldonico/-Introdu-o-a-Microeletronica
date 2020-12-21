function [y] = cost_function (x)
  
write_netlist(x);
run_netlist;
meas = read_log;
y = meas;
endfunction
