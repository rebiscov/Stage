open Printf
module S = States

let write file w d =
  for i=0 to d-1 do
    fprintf file "%d " (S.get w i)
  done;
  fprintf file "%d" (S.get w d)

  
