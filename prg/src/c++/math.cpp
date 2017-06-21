unsigned int bino(unsigned int k, unsigned int n){
  unsigned int b = 1;
  if (k < 0 || n < k)
    return 0;
  while (k > 0){
    b *= n/k;
    n--;
    k--;
  }
  return b;
}

unsigned int c(unsigned int d, unsigned int s){
  return bino(d+1, (s+1)*(d+1))/(1+s*(d+1));
}

unsigned int max(unsigned int a, unsigned int b){
  if (a >= b)
    return a;
  return b;
}
