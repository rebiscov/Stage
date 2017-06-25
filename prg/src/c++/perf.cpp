#include <cstdio>
#include <unordered_map>
#include "states.hpp"

int main(int argc, char *argv[]){
  unsigned int d, s, space;

  if (argc < 3){
    printf("Use %s d s\n", argv[0]);
    exit(1);
  }

  d = atoi(argv[1]);
  s = atoi(argv[2]);
  space = state_space(d, s);

  std::unordered_map<W, unsigned int> h(space);
  std::vector<W> w_set = compute_w(d, s);

  for (unsigned int k = 0; k < space; k++)
    h[w_set[k]] = k;

  unsigned int a = 56;

  for (unsigned int k = 0; k < space; k++){
    unsigned int key = h[w_set[k]];
    if (key > a)
      a = key;
  }
	
  return 0;
}
