open Printf
module S = States

let compute_sum w'' d s v opt distribution h t = (* Computes the sum in the main algorithm *)
  let w = S.copy w'' in
  S.inc_time w v;
  let id_w = Hashtbl.find h w in
  let sum = ref 0. in
  for i=1 to s do
    let w' = S.copy w in
    for j=1 to d do
      let k = d-j+1 in
      S.set w' k (s + S.get w' k);
      sum := !sum +. distribution.(id_w).(k-1).(i-1) *. opt.(t+1).(Hashtbl.find h w').(v);
    done;
  done;
  !sum +. distribution.(id_w).(0).(s) *. opt.(t+1).(id_w).(v) (* distribution.(id_w).(0).(s) proba to go from w to w' with delta=s=0 (no work has arrived) *)

  
(** MAIN FUNCTION *)
  
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

  (* We check if maximal speed is sufficient *)
  if v_max < d*s then
    failwith("ERROR: speed is not great enough, you will encouter a major failure during computation !\n")
  else
    ();


  (* Defining the hashtable, the distribution, the w_set and the array of the expected consumption *)
  let h = Hashtbl.create space in
  
  let distribution = Array.make_matrix space d [||] in
  for i=0 to space-1 do
    for j=0 to d-1 do
      distribution.(i).(j) <- Array.make (s+1) 0.
    done;
  done;

  let w_set = S.compute_w d s in
  
  let opt = Array.make_matrix (bt+1) space [||] in
  for i=0 to bt do
    for j=0 to space-1 do
      opt.(i).(j) <- Array.make (v_max+1) 0.
    done;
  done;

  (* Extracting the distribution from the file and giving a number to each of the w *)
  
  for k=0 to space-1 do
    let w = w_set.(k) in
    Hashtbl.add h (S.copy w) k; (* Giving a number to current w *)
    for i=1 to d do
      for j=1 to s do
        distribution.(k).(i-1).(j-1) <- Scanf.bscanf ffd "%f " (fun x -> x) (* Loading distribution of all the state we can go from w *)
      done;
    done;
    distribution.(k).(0).(s) <- Scanf.bscanf ffd "%f\n" (fun x -> x); (* Here we store the case where s=0 *)
  done;

  (* Now, we can begin the main algorithm *)
  let t = ref (bt-1) in

  while !t >= 1 do
    for k=0 to space-1 do (* We explore all the w *)
      let w = w_set.(k) in
      for i=0 to v_max do (* We explore all the speeds *)
        let cost = ref ((Funs.f i v_max) +. (Funs.c v_max) +. (compute_sum w d s v_max opt distribution h !t)) in
        for j=0 to v_max-1 do
          if j >= S.get w 1 then
            cost := min ((Funs.f i j) +. (Funs.c j) +. (compute_sum w d s j opt distribution h !t)) !cost
        done;
        opt.(!t).(Hashtbl.find h w).(i) <- !cost;
      done;
    done;
    t := !t-1;
  done;

  (* Case where t=0 *)
  
  for k=0 to space-1 do
    let w' = S.copy w_set.(k) in
    let cost = ref ((Funs.c v_max) +. (compute_sum w' d s v_max opt distribution h !t)) in
    for i=0 to v_max-1 do
      cost := min !cost ((Funs.c v_max) +. (compute_sum w' d s v_max opt distribution h !t))
    done;
  done;

  close_in fd;
  printf "Bye !\n";
  ()
