module S = States

(* The function who returns a couple (sigma,delta) (a worload and a deadline) depending on the probabilities *)

let rand_couple d s id_w distribution =
  let cumulative = Array.make (d*(s+1)) 0. in
  cumulative.(0) <- distribution.(id_w).(0).(0);
  for i=1 to d*(s+1)-1 do
    cumulative.(i) <- distribution.(id_w).(i mod (s+1)).(i/(s+1)) +. cumulative.(i-1)
  done;

  Random.init (int_of_float (Unix.time ()));

  let r = Random.float 1. in
  let i = ref 0 in
  while cumulative.(!i) < r do
    i := !i + 1
  done;
  ((!i mod (s+1))+1, !i / (s+1))
         
(* MAIN FUNCTION *)

let () =
  if Array.length Sys.argv < 5 then
    begin
      Printf.printf "Use: %s T v_max distribution_file policies_file options\n" Sys.argv.(0);
      exit 1
    end
  else
    ();
  
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
  
  let w = S.new_w d in

  for i=0 to bt-1 do
    let (delta, sigma) = rand_couple d s (Hashtbl.find h w) distribution in
    
  done;
  ()
  
