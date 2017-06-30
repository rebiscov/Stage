#include "math.hpp"

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

unsigned int pow(unsigned int e, unsigned int n){
  if (n == 0)
    return 1;
  else if (n == 1)
    return e;
  else{
    if (n%2 == 0){
      unsigned int f = pow(e, n/2);
      return f*f;
    }
    else{
      unsigned int f = pow(e, n/2);
      return e*f*f;
    }
  }
}
