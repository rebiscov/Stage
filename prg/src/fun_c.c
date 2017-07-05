#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <stdio.h>
#include "c++/funs.c"

CAMLprim
value cost(value v){
  CAMLparam1(v);
  float res = (float)c((unsigned int)Int_val(v));
  CAMLreturn(caml_copy_double(res));
}

CAMLprim
value overcost(value v1, value v2, value t){
  CAMLparam3(v1, v2, t);
  CAMLreturn(caml_copy_double(f((unsigned int)Int_val(v1), (unsigned int)Int_val(v2), (unsigned int)Int_val(t))));
}
