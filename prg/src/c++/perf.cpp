#include <cstdio>
#include <ctime>
#include <unordered_map>
#include "states.hpp"

int main(int argc, char *argv[]){
  unsigned int d, s, space;
  clock_t t1, t2, t3;

  if (argc < 3){
    printf("Use %s d s\n", argv[0]);
    exit(1);
  }

  d = atoi(argv[1]);
  s = atoi(argv[2]);
  space = state_space(d, s);

  t1 = clock();

  std::unordered_map<W, unsigned int> h(space);
  std::vector<W> w_set = compute_w(d, s);

  for (unsigned int k = 0; k < space; k++)
    h[w_set[k]] = k;

  t2 = clock();

  unsigned int a = 56;

  for (unsigned int k = 0; k < space; k++){
    unsigned int key = h[w_set[k]];
    if (key > a)
      a = key;
  }

  t3 = clock();

  printf("Time to compute w_set and the hashtable: %f\n", ((double)(t2-t1))/CLOCKS_PER_SEC);
  printf("Time to access all the elements of the hashtable %f\n", (double)(t3-t2)/CLOCKS_PER_SEC);
	
  return 0;
}
