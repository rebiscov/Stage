let rec bino k n =
  if k < 0 then
    0
  else if k = 0 then
    1
  else
    n*(bino (k-1) (n-1))/k
  
let c d s =
  (bino (d+1) ((s+1)*(d+1)))/(1+s*(d+1))
