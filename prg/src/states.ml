type w = int array

let new_w d =
  Array.make d 0

let copy w =
  Array.copy w

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
  ()

let get w i =
  w.(i-1)

let add w i e =
  let d = Array.length w in
  let rec set_aux w i e =
    if i < d then
      begin
        w.(i) <- w.(i) + e;
        set_aux w (i+1) e
      end
    else
      () in
  add_aux w (i-1) e


let inc_time w v =
  let d = Array.length w in
  for i=0 to d-2 do
    w.(i) <- max (w.(i+1)-v) 0;
  done;
  w.(d-1) <- w.(d-2);
  ()
    
  
    
