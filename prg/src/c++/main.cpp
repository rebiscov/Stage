#include <iostream>
#include "states.h"

int main(void){
  std::vector<W> w_set;

  w_set = compute_w(4, 3);

  std::cout << "Size: " << w_set.size() << std::endl;

  return 0;
}
