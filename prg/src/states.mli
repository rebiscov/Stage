type w = int array

(** Construct the state 0 *)
val new_w : int -> w 

(** Given a state, it computes the next state *)
val compute_next_w : w -> int -> int -> w
