type w = int array

let new_w d =
  Array.make d 0

let rec compute_next_w_i w i d s =
  w.(i) <- (w.(i)+1)mod((i+1)*s+1);
  if w.(i) = 0 && i > 0 then
    begin
      w.(i) <- compute_next_w_i w (i-1) d s;
      w.(i)
    end
  else
    w.(i)

let compute_next_w w d s =
  let _ = compute_next_w_i w (d-1) d s in
  w

let get w i =
  if i = 0 then
    w.(0)
  else
    w.(i) - w.(i-1)

