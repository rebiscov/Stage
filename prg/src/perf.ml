open Printf
module S = States

let () = 
  if Array.length Sys.argv < 3 then
    begin
      printf "Use %s d s\n" Sys.argv.(0);
      exit 1
    end
  else
    ();
  let (d, s) = (int_of_string Sys.argv.(1), int_of_string Sys.argv.(2)) in
  let space = S.state_space d s in

  let h = Hashtbl.create space in
  let w_set = S.compute_w d s in

  for k=0 to space-1 do
    Hashtbl.add h w_set.(k) k;
  done;

  let a = ref 56 in

  for k=0 to space-1 do
    let key = Hashtbl.find h w_set.(k) in
    if key > !a then
      a := key
  done;

  ()
