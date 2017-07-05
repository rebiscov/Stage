#include "math.h"

unsigned int bino(unsigned int k, unsigned int n){
  if (k < 0)
    return 0;
  else if (k == 0)
    return 1;
  else
    return n*bino(k-1, n-1)/k;
}

unsigned int c(unsigned int d, unsigned int s){
  return bino(d+1, (s+1)*(d+1))/(1+s*(d+1));
}

unsigned int max(unsigned int a, unsigned int b){
  if (a >= b)
    return a;
  return b;
}

unsigned int min(unsigned int a, unsigned int b){
  if (a <= b)
    return a;
  return b;
}

unsigned int minus(unsigned int a, unsigned int b){
  if (a >= b)
    return a - b;
  else
    return 0;
}
