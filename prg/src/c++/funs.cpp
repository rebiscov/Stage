#include <cmath>
#include <cstdio>
#include "funs.hpp"
#include "math.hpp"

double c(unsigned int v){
  return (double)(v*v*v);
}

double f (unsigned int v1, unsigned int v2, unsigned int t){
  double delta = 0.1;
  if (t == 0 || v1 == v2 || v1 == 0 || v2 == 0)
    return 0;
  else{
    unsigned int v_min = min(v1, v2);
    unsigned int v_max = max(v1,v2);
    double delta_1 = (double)(delta*(double)v_min) / (double)(v_max-v_min);
    double res = delta_1*c(v_max) - (delta + delta_1)*c(v_min);
    
    return res;
  }
}
