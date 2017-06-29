let c v =
  float_of_int (v*v*v)

let f v1 v2 t =
  let delta = 0.1 in
  if t=0 || v1 = v2 || v1 = 0 || v2 = 0 then
    0.
  else
    begin
      let v_min = min v1 v2 in
      let v_max = max v1 v2 in
      let delta_1 = (delta*.(float_of_int v_min)) /. ( float_of_int (v_max-v_min)) in
      delta *. (c v_max) -. (delta +. delta_1)*.(c v_min) +. delta *. (float_of_int (v1 + v2)) /. 2.
    end
