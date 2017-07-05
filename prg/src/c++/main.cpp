#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unordered_map>
#include <thread>
#include <cstring>
#include "states.hpp"
#include "funs.h"
#include "math.h"

#define NB_THREADS 4

/* some prototypes */

double compute_sum(W w, unsigned int d, unsigned int s, unsigned int v, double ***opt, double ***distribution, std::unordered_map<W, unsigned int> &h, unsigned int t);

void compute_j(W *w_set,unsigned int id_thread, unsigned int d, unsigned int s, unsigned int v_max, unsigned int space, double ***opt, unsigned int ***pol, double ***distribution, std::unordered_map<W,unsigned int> *h, unsigned int t);

/* MAIN FUNCTION */

int main(int argc, char* argv[]){
  bool debug;
  printf("DP algorithm, C++ first version\n");

  if (argc < 5 ){
    printf("Use: %s T v_max distribution_file out_file options\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  if (argc >= 6)
    debug = strcmp("d", argv[5]) == 0;
  

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
  fscanf(fd, "%u %u", &d, &s);

  unsigned int space = state_space(d, s);


  /* We check if maximal speed if sufficient */
  if (v_max < d*s){
    printf("ERROR: speed is not great enough, you will encouter a major failure during computation !\n");
    exit(EXIT_FAILURE);
  }

  /* Defining the hashtable, the distribution, the w_set, the best policies array and the array of the expected consumption */
  std::unordered_map<W, unsigned int> h(space);

  double ***distribution = NULL;

  distribution = new double**[space];

  for (unsigned int i = 0; i < space; i++){
    distribution[i] = new double*[d];
    for (unsigned int j = 0; j < d; j++)
      distribution[i][j] = new double[s+1];
  }

  if (debug)
    printf("COMPUTE W set...");
  
  W *w_set = NULL;
  w_set = compute_w(d, s);
  if (w_set == NULL){
    printf("Error: memory allocation for w_set did not work!\n");
    exit(1);
  }

  if (debug)
    printf("done.\n");  

  unsigned int ***pol = NULL;
  pol = new unsigned int**[bt+1];
  if (pol == NULL){
    printf("Error: memory allocation for pol did not work!\n");
    exit(1);
  }

  for (unsigned int i = 0; i <= bt; i++){
    pol[i] = new unsigned int*[space];
    for (unsigned int j = 0; j < space; j++)
      pol[i][j] = new unsigned int[v_max+1];
  }
    
  
  double ***opt = NULL;
  opt = new double**[bt+1];
  if (opt == NULL){
    printf("Error: memory allocation for opt did not work!\n");
    exit(1);
  }

  for (unsigned int i = 0; i <= bt; i++){
    opt[i] = new double*[space];
    for (unsigned int j = 0; j < space; j++)
      opt[i][j] = new double[v_max+1];
  }

  /* Extracting the distribution from the file and giving a number to each of the w */
  if (debug)
    printf("Loading distribution... ");
  
  for (unsigned int k = 0; k < space; k++){
    h[w_set[k]] = k;
    for (unsigned int i = 1; i <= d; i++)
      for (unsigned int j = 0; j <= s; j++)
	fscanf(fd, "%lf", &distribution[k][i-1][j]);
  }

  if (debug)
    printf("done.\n");
  fclose(fd);
  
  /* Now we can begin the main algorithm */
  int t = bt-1;

  std::thread threads[NB_THREADS];
  
  while (t >= 0){
    if (debug)
      printf("COMPUTATION: t = %d\n", t);
    for (unsigned int k = 0; k < NB_THREADS; k++)
      threads[k] = std::thread (compute_j, w_set, k, d, s, v_max, space, opt, pol, distribution, &h, t);
    for (unsigned int k = 0; k < NB_THREADS; k++)
      threads[k].join();
    t--;
  }

  /* Output best policies array */
  fd = NULL;
  fd = fopen(argv[4], "w");
  if (fd == NULL){
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  for (unsigned int i = 0; i <= bt; i++)
    for (unsigned int j = 0; j < space; j++)
      for (unsigned int k = 0; k <= v_max; k++)
	if (j == space-1 && k == v_max)
	  fprintf(fd, "%d\n", pol[i][j][k]);
	else
	  fprintf(fd, "%d ", pol[i][j][k]);

  fclose(fd);
  
  printf("Bye!\n");
  
  return 0;
}

void compute_j(W *w_set,unsigned int id_thread, unsigned int d, unsigned int s, unsigned int v_max, unsigned int space, double ***opt, unsigned int ***pol, double ***distribution, std::unordered_map<W,unsigned int> *h, unsigned int t){
  unsigned int minimum = (id_thread+1)*(space/NB_THREADS);
  if (id_thread == NB_THREADS -1)
    minimum = space;
  for (unsigned int k = id_thread*(space/NB_THREADS); k < minimum; k++){ /* We explore all states */
    W& w = w_set[k];

    for (unsigned int i = 0; i <= v_max; i++){ /* We explore all speeds */
      double cost = f(i, v_max, t) + c(v_max) + compute_sum(w, d, s, v_max, opt, distribution, *h, t);
      unsigned int p = v_max;

      for (unsigned int j = 0; j < v_max; j++){
	if (j >= w.get(1)){
	  double c_j = c(j);
	  if (c_j >= cost)
	    break;
	  double co = f(i,j,t) + c_j + compute_sum(w, d, s, j, opt, distribution, *h, t);
	  if (co < cost){
	    cost = co;
	    p = j;
	  }
	}
      }
      unsigned int id = (*h)[w];
      opt[t][id][i] = cost;
      pol[t][id][i] = p;
    }
  }
}


double compute_sum(W w, unsigned int d, unsigned int s, unsigned int v, double ***opt, double ***distribution, std::unordered_map<W,unsigned int> &h, unsigned int t){
  w.inc_time(v);
  unsigned int id_w = h[w];
  double sum = 0;

  for (unsigned int i = 0; i <= s; i++){
    W wp = w;
    for (unsigned int j = 1; j <= d; j++){
      unsigned int k = d-j+1;
      wp.set(k, wp.get(k)+i);
      sum += distribution[id_w][k-1][i] * opt[t+1][h[wp]][v];
    }
  }

  return sum;
}
