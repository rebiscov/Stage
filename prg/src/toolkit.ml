open Printf
open Sys
module S = States

let write file w d =
  for i=0 to d-1 do
    fprintf file "%d " (S.get w i)
  done;
  fprintf file "%d\n" (S.get w d)

let () =
  printf "Welcome to the toolkit\n";
  if (Array.length Sys.argv < 4) then
    begin
      printf "Use: %s d s filename\n" Sys.argv.(0);
      exit 1
    end
  else
    ();
  let d = int_of_string Sys.argv.(1) in
  let s = int_of_string Sys.argv.(2) in
  let filename = Sys.argv.(3) in
  let file = open_out filename in
  let space = S.state_space d s in
  let w_set = S.compute_w d s in
  
  for i=0 to space-1 do
    write file w_set.(i) d;
  done;
  close_out file;
  printf "%d functions generated with d=%d and s=%d\n" space d s;
  ()
  
  
  

  
