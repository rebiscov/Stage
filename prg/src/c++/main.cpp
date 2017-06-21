#include <cstdio>
#include <cstdlib>
#include <vector>
#include "states.h"
#include "funs.h"

int main(int argc, char* argv[]){
  printf("DP algorithm, C++ first version\n");

  if (argc < 5 ){
    printf("Use: %s T v_max distribution_file out_file options\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* I define all the important variables, I extract the distribution in the file given by the user and I initialize the array J_t^v */

  /* Defining time limit and maximal speed */
  unsigned int bt = atoi(argv[1]);
  unsigned int v_max = atoi(argv[2]);

  /* Opening the file containing the distribution and extracting deadline limit and workload limit variables, defining state space */
  FILE* fd = NULL;
  fd = fopen(argv[3], "r");
  if (fd == NULL)
    perror("fopen");

  unsigned int d, s;
  fprintf(fd, "%d %d", &d, &s);

  unsigned int space = state_space(d, s);

  /* We check if maximal speed if sufficient */
  if (v_max < d*s){
    printf("ERROR: speed is not great enough, you will encouter a major failure during computation !\n");
    exit(EXIT_FAILURE);
  }

  /* Defining the distribution, the w_set, the best policies array and the array of the expected consumption */

  std::vector<std::vector<std::vector<double>>> distribution(space,std::vector<std::vector<double>>(d));

  for (unsigned int i = 0; i < space; i++)
    for (unsigned int j = 0; j < d; j++)
      distribution[i][j] = std::vector<double>(s+1, 0);

  std::vector<W> w_set = compute_w(d, s);

  std::vector<std::vector<std::vector<unsigned int>>> pol(bt+1, std::vector<std::vector<unsigned int>>(space));

  for (unsigned int i = 0; i <= bt; i++)
    for (unsigned int j = 0; j < space; j++)
      pol[i][j] = std::vector<unsigned int>(v_max+1, 0);
  
  std::vector<std::vector<std::vector<double>>> opt(bt+1, std::vector<std::vector<double>>(space));

  for (unsigned int i = 0; i <= bt; i++)
    for (unsigned int j = 0; j < space; j++)
      opt[i][j] = std::vector<double>(v_max+1, 0.);

  /* Extracting the distribution from the file and giving a number to each of the w */
  for (unsigned int k = 0; k < space; k++){
    w_set[k].set_id(k);
    for (unsigned int i = 1; i <= d; i++)
      for (unsigned int j = 0; j <= s; j++)
	fprintf(fd, "%f", distribution[k][i-1][j]);
  }
  fclose(fd);


  /* Now we can begin the main algorithm */
  unsigned int t = bt-1;

  while (t >= 0){
    for (unsigned int k = 0; k < space; k++){
      W& w = w_set[k];

      for (unsigned int i = 0; i <= v_max; i++){
	
      }
    }
  }
  
  return 0;
}


