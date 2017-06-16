open Printf
module S = States

let () =
  printf "DP algorithm, first version\n";
  if (Array.length Sys.argv < 4) then
    begin
      printf "Use: %s T v_max distribustion_file options\n" Sys.argv.(0);
      exit 1
    end
  else
    ();
  
  (* I define all the important variables, I extract the distribution in the file given by the user and I initialize the array J_t^v *)

  (* Defining time limit and maximal speed *)
  let bt = int_of_string Sys.argv.(1) in
  let v_max = int_of_string Sys.argv.(2) in

  (* Opening the file containing the distribution and extracting deadline limit and workload limit variables, defining state space *)
  let fd = open_in Sys.argv.(3) in
  let ffd = (Scanf.Scanning.from_channel fd) in
  let (d, s) = Scanf.bscanf ffd "%d %d\n" (fun x y -> (x,y)) in
  let space = S.state_space d s in

  (* Defining the hashtable, the distribution and the array of the expected consumption *)
  let h = Hashtbl.create space in
  
  let distribution = Array.make_matrix space d [||] in
  for i=0 to space-1 do
    for j=0 to d-1 do
      distribution.(i).(j) <- Array.make s 0.
    done;
  done;
  
  let opt = Array.make_matrix space (bt+1) [||] in
  for i=0 to space - 1 do
    for j=0 to bt do
      opt.(i).(j) <- Array.make (v_max+1) 0
    done;
  done;

  (* Extracting the distribution from the file and giving a number to each of the w *)
  let w = S.new_w d in
  let count = ref 0 in
  let b = ref true in
  while !b do
    Hashtbl.add h w !count;
    for i=1 to d do
      for j=1 to s do
          distribution.(!count).(i-1).(j-1) <- Scanf.bscanf ffd "%f " (fun x -> x)
      done;
    done;
    count := !count + 1;
    S.compute_next_w w d s;
    if S.is_null w then
      b := false
  done;

  
  ()
