type w

(** Construct the state 0 *)
val new_w : int -> w 

(** Given a state, it computes the next state *)
val compute_next_w : w -> int -> int -> w

(** give the value of w[i], w begin the remaining work function *)
val get : w -> int -> int

(** set the value of w[i], w begin the remaining work function *)
val set : w -> int -> int -> unit
