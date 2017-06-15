open Printf
module S = States

let () =
  printf "DP algorithm, first version\n";
  if (Array.length Sys.argv < 3) then
    begin
      printf "Use: %s T distribustion_file options\n" Sys.argv.(0);
      exit 1
    end
  else
    ();
  let t = int_of_string Sys.argv.(1) in
  
  
   
