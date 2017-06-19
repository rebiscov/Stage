type w = int array

let new_w d =
  Array.make d 0

let copy w =
  Array.copy w
  
let is_null w =
  Array.for_all (fun x -> x = 0) w

let state_space d s =
  Math.c d s

let print_w w =
  Array.iter (fun x -> Printf.printf "%d " x) w;
  Printf.printf "\n"
  
let rec compute_w_i w_set w d s sigma_i i index =
  if i >= d then
    begin
      w_set.(!index) <- Array.copy w;
      index := !index + 1
    end
  else
    begin
      for sigma=sigma_i to (i+1)*s do
        w.(d-i-1) <- sigma;
        compute_w_i w_set w d s sigma (i+1) index
      done
    end
  
let compute_w d s =
  let space = state_space d s in
  let w_set = Array.make space [||] in
  compute_w_i w_set (Array.make d 0) d s 0 0 (ref 0);
  for i=0 to space-1 do
    w_set.(i).(0) <- w_set.(i).(0) - w_set.(i).(1);
    for j=1 to d-2 do
      w_set.(i).(j) <- w_set.(i).(j-1) + w_set.(i).(j) - w_set.(i).(j+1)
    done;
    w_set.(i).(d-1) <- w_set.(i).(d-2) + w_set.(i).(d-1);
  done;
  w_set

let get w i =
  if i = 0 then
    0
  else
    w.(i-1)

let add w i e =
  let d = Array.length w in
  let rec add_aux w i e =
    if i < d then
      begin
        w.(i) <- w.(i) + e;
        add_aux w (i+1) e
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
    
let set w x e =
  let d = Array.length w in
  w.(x-1) <- e;
  if (x < d && w.(x) < w.(x-1)) || (x > 1 && w.(x-2) > w.(x-1)) then
    failwith("States.set: not a valid value, this is not a valid remaining function anymore")
  else
    ()
