module S = States

(* The function who returns a couple (sigma,delta) (a worload and a deadline) depending on the probabilities *)

let rand_couple d s id_w distribution =
  let cumulative = Array.make (d*(s+1)) 0. in
  cumulative.(0) <- distribution.(id_w).(0).(0);
  for i=1 to d*(s+1)-1 do
    cumulative.(i) <- distribution.(id_w).(i / (s+1)).(i mod (s+1)) +. cumulative.(i-1);
  done;

  let r = Random.float 1. in
  let i = ref 0 in
  while cumulative.(!i) < r do
    i := !i + 1;
  done;
  ((!i / (s+1))+1, !i mod (s+1))

(* The function which updates the array representing the work that must be done before time t *)
  
  let update_d d t delta sigma =
    for i=t+delta to Array.length d -1 do
      d.(i) <- d.(i) + sigma
    done  
         
(* MAIN FUNCTION *)

let () =
  if Array.length Sys.argv < 7 then
    begin
      Printf.printf "Use: %s T v_max distribution_file policies_file output_file nb_simulations options\n" Sys.argv.(0);
      exit 1
    end
  else
    ();

  (* Initialise the random number generator *)
  Random.init (int_of_float (Unix.time ()));
  
  (* Opening the files *)
  let dist' = open_in Sys.argv.(3) in
  let dist = (Scanf.Scanning.from_channel dist') in
  let policies' = open_in Sys.argv.(4) in
  let policies = (Scanf.Scanning.from_channel policies') in

  (* Defining important variables*)
  let bt = int_of_string Sys.argv.(1) in
  let v_max = int_of_string Sys.argv.(2) in  
  let (d,s) = Scanf.bscanf dist "%d %d\n" (fun x y -> (x,y)) in
  let space = S.state_space d s in
  let h = Hashtbl.create space in
  let nb_simulations = int_of_string Sys.argv.(6) in

  let distribution = Array.make_matrix space d [||] in
  for i=0 to space-1 do
    for j=0 to d-1 do
      distribution.(i).(j) <- Array.make (s+1) 0.
    done;
  done;

  let w_set = S.compute_w d s in

  let pol = Array.make_matrix (bt+1) space [||] in
  for i=0 to bt do
    for j=0 to space-1 do
      pol.(i).(j) <- Array.make (v_max+1) 0
    done;
  done;
  
  (* Extracting the distribution from the file, the best policies and giving a number to each of the w *)
  
  for k=0 to space-1 do
    let w = w_set.(k) in
    Hashtbl.add h (S.copy w) k; (* Giving a number to current w *)
    for i=1 to d do
      for j=0 to s do
        distribution.(k).(i-1).(j) <- Scanf.bscanf dist "%f " (fun x -> x) (* Loading distribution of all the state we can go from w *)
      done;
    done;
  done;

  for i=0 to bt do
    for j=0 to space-1 do
      for k=0 to v_max do
        pol.(i).(j).(k) <- Scanf.bscanf policies "%d " (fun x -> x) (* Loading best policies *)
      done;
    done;
  done;

  close_in policies';
  close_in dist';

  (* The simulation starts here *)
  let fd = open_out Sys.argv.(5) in

  for simu=1 to nb_simulations do
    Printf.printf "Simulation %d\n" simu;
    let w = S.new_w d in
    let speeds = Array.make bt 0 in
    let last_speed = ref 0 in
    let cost = ref 0. in
    let change_speed = ref 0 in
    let work_t = Array.make (bt+1) 0 in

    for i=0 to bt-1 do
      let (delta, sigma) = rand_couple d s (Hashtbl.find h w) distribution in
      update_d work_t i delta sigma;
      S.add_work w delta sigma;
      speeds.(i) <- pol.(i).(Hashtbl.find h w).(!last_speed);
      cost := (Funs.c speeds.(i)) +. (Funs.f !last_speed speeds.(i) i) +. !cost;
      if speeds.(i) <> !last_speed && i > 0 then
        change_speed := !change_speed + 1;
      last_speed := speeds.(i);
      S.inc_time w !last_speed
    done;

    (* Write the results in a file *)

    for i=1 to bt -1 do
      speeds.(i) <- speeds.(i) + speeds.(i-1) (* We want the cumulated speeds *)
    done;

    Printf.fprintf fd "%f %d\n" !cost !change_speed;
    
    Printf.fprintf fd "0 ";
    for i=0 to bt -2 do
      Printf.fprintf fd "%d " speeds.(i)
    done;
    Printf.fprintf fd "%d\n" speeds.(bt-1);
    
    for i=0 to bt -1 do
      Printf.fprintf fd "%d " work_t.(i)
    done;
    Printf.fprintf fd "%d\n" work_t.(bt);
  done;

  close_out fd;
  ()
  
