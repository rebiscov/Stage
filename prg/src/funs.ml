let f v1 v2 =
  float_of_int (max (v1 - v2) (v2-v1))

let c v =
  exp (float_of_int v)
