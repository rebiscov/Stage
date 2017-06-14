type w

val new_w : int -> w
(** Construct the state 0 (the function always equals to zero *)

val copy : w -> w
(** Copy a remaining work function *)  
  
val compute_next_w : w -> int -> int -> unit
(** Given a state, it computes the next state *)

val get : w -> int -> int
(** Give the value of w[i], w being the remaining work function *)

val add : w -> int -> int -> unit
(** Add some value starting at i (you're incrementing the step between w[i] and w[i+1]) *)

val inc_time : w -> int -> unit
(** Computes w_t from w_t-1 with s=0 and d=0 and v provided by the user (see Lemma 1 of the paper) *)

val set : w -> int -> int -> unit
(** Set the value of the cell w[x] *)

val is_null : w -> bool
(** returns true if w is the 0 function *)

