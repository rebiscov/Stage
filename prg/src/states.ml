type w = int array

let new_w d =
  Array.make d 0
       
let compute_next_w w s d =
  let ret = ref 1 in
  let i = ref 0 in
  let sigma_i = ref 0 in
  let sigma_i_prev = ref 0 in
  while !i < d && !ret = 1 do
    sigma_i := !sigma_i+w.(!i);
    ret := (!sigma_i+1)/((!i+1)*s+1);
    sigma_i := (!sigma_i+1)mod((!i+1)*s+1);
    w.(!i) <- !sigma_i - !sigma_i_prev;
    sigma_i_prev := !sigma_i;
    i := !i+1
  done;
  w;;

let t = new_w 5;;
let t = compute_next_w t 2 5;;

  

