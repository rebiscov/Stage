external cost : int -> float = "cost"
external overcost : int -> int -> int -> float = "overcost"

let c v =
  cost v

let f v1 v2 t =
  overcost v1 v2 t
