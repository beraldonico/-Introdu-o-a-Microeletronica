function [] = hill_climbing ()

lb = [1.5, 3, 1, 100];
ub = [10, 100, 5, 100000];
for i=1:4
  x(i) = lb(i) + (ub(i)-lb(i))*rand;
endfor
custo_atual = cost_function(x);
for i=1:100
  for j=1:4
    x_modificado(j) = x(j)*(1+(1/10*i)*randn);
  endfor
  for j=1:4
    while (x_modificado(j) < lb(j) || x_modificado(j) > ub(j) )
      x_modificado(j) = x(j)*(1+(1/10*i)*randn);
    endwhile
  endfor
  custo_novo = cost_function(x_modificado);
  fprintf('iteration: %d \t custo_novo: %f \t', i, custo_novo);
  %for j=1:4
    %printf('valor de x_%d modificado: %f\t',j, x_modificado(j));
  %endfor
  fprintf('\n');
  fflush(stdout);
  if (custo_novo > custo_atual)
    custo_atual = custo_novo;
    x = x_modificado;
  endif 
  melhor_custo(i) = custo_atual;
endfor

plot(melhor_custo, 'o-b');
endfunction
