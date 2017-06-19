let f v1 v2 t=
  if t=0 then
    0.
  else
    float_of_int (max (v1 - v2) (v2-v1))

let c v =
  exp (float_of_int v)
