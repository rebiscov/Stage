type w

(** Construct the state 0 (the function always equals to zero *)
val new_w : int -> w 

(** Copy a remaining work function *)
val copy : w -> w
  
(** Given a state, it computes the next state *)
val compute_next_w : w -> int -> int -> unit

(** Give the value of w[i], w being the remaining work function *)
val get : w -> int -> int

(** add some value starting at i (you're incrementing the step between w[i] and w[i+1]) *)
val add : w -> int -> int -> unit

(** Computes w_t from w_t-1 with s=0 and d=0 and v provided by the user (see Lemma 1 of the paper) *)
val inc_time : w -> int -> unit

