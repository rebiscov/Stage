#include <cmath>
#include <algorithm>
#include "funs.hpp"

#define delta 0.1

double c(unsigned int v){
  return (double)(v*v*v);
}

double f (unsigned int v1, unsigned int v2, unsigned int t){
  if (t == 0 || v1 == v2)
    return 0;
  else{
    unsigned int v_min = std::min(v1,v2);
    unsigned int v_max = std::max(v1,v2);
    double delta_1 = (delta*(double)v_min) / (double)std::abs((long)(v1-v2));
    
    return delta_1*(double)c(v_max)* - (delta + delta_1)*(double)c(v_min);
  }
}
