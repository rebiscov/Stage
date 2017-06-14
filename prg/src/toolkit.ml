open Printf
open Sys
module S = States

let write file w d =
  for i=0 to d-1 do
    fprintf file "%d " (S.get w i)
  done;
  fprintf file "%d" (S.get w d)

let () =
  printf "Welcome to the toolkit\n";
  if (Array.length Sys.argv < 3) then
    begin
      printf "Use: %s d s\n" Sys.argv.(0);
      exit 1
    end
  else
    ();
  let d = int_of_string Sys.argv.(1) in
  let s = int_of_string Sys.argv.(2) in
  ()
  
  

  
