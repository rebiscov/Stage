#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <thread>
#include <cstring>
#include "states.hpp"
#include "funs.hpp"
#include "math.hpp"

#define NB_THREADS 4

/* some prototypes */

double compute_sum(W w, unsigned int d, unsigned int s, unsigned int v, std::vector<std::vector<std::vector<double>>> &opt, std::vector<std::vector<std::vector<double>>> &distribution, std::map<W, unsigned int> &h, unsigned int t);

void compute_j(std::vector<W> *w_set,unsigned int id_thread, unsigned int d, unsigned int s, unsigned int v_max, unsigned int space, std::vector<std::vector<std::vector<double>>> *opt, std::vector<std::vector<std::vector<unsigned int>>> *pol, std::vector<std::vector<std::vector<double>>> *distribution, std::map<W,unsigned int> *h, unsigned int t);

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
  std::map<W, unsigned int> h;

  std::vector<std::vector<std::vector<double>>> distribution(space,std::vector<std::vector<double>>(d));

  for (unsigned int i = 0; i < space; i++)
    for (unsigned int j = 0; j < d; j++)
      distribution[i][j] = std::vector<double>(s+1, 0);

  if (debug)
    printf("COMPUTE W set...");
  
  std::vector<W> w_set = compute_w(d, s);

  if (debug)
    printf("done.\n");  

  std::vector<std::vector<std::vector<unsigned int>>> pol(bt+1, std::vector<std::vector<unsigned int>>(space));
  
  for (unsigned int i = 0; i <= bt; i++)
    for (unsigned int j = 0; j < space; j++)
      pol[i][j] = std::vector<unsigned int>(v_max+1, 0);
  
  std::vector<std::vector<std::vector<double>>> opt(bt+1, std::vector<std::vector<double>>(space));

  for (unsigned int i = 0; i <= bt; i++)
    for (unsigned int j = 0; j < space; j++)
      opt[i][j] = std::vector<double>(v_max+1, 0.);

  /* Extracting the distribution from the file and giving a number to each of the w */
  if (debug)
    printf("Loading distribution... ");
  
  for (unsigned int k = 0; k < space; k++){
    h[w_set[k]] = k;
    for (unsigned int i = 1; i <= d; i++)
      for (unsigned int j = 0; j <= s; j++)
	fscanf(fd, "%f", &distribution[k][i-1][j]);
  }

  if (debug)
    printf("done.\n");
  fclose(fd);
  
  /* Now we can begin the main algorithm */
  int t = bt-1;

  while (t >= 0){
    if (debug)
      printf("COMPUTATION: t = %d\n", t);
    std::vector<std::thread> threads;    
    for (unsigned int k = 0; k < NB_THREADS; k++)
      threads.push_back(std::thread (compute_j, &w_set, k, d, s, v_max, space, &opt, &pol, &distribution, &h, t));
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

void compute_j(std::vector<W> *w_set,unsigned int id_thread, unsigned int d, unsigned int s, unsigned int v_max, unsigned int space, std::vector<std::vector<std::vector<double>>> *opt, std::vector<std::vector<std::vector<unsigned int>>> *pol, std::vector<std::vector<std::vector<double>>> *distribution, std::map<W,unsigned int> *h, unsigned int t){
  for (unsigned int k = id_thread*(space/NB_THREADS); k < min((id_thread+1)*(space/NB_THREADS), space); k++){ /* We explore all states */
    W& w = (*w_set)[k];

    for (unsigned int i = 0; i <= v_max; i++){ /* We explore all speeds */
      double cost = f(i, v_max, t) + c(v_max) + compute_sum(w, d, s, v_max, *opt, *distribution, *h, t);
      unsigned int p = v_max;

      for (unsigned int j = 0; j < v_max; j++){
	if (j >= w.get(1)){
	  double co = f(i,j,t) + c(j) + compute_sum(w, d, s, j, *opt, *distribution, *h, t);
	  if (co < cost){
	    cost = co;
	    p = j;
	  }
	}
      }
      unsigned int id = (*h)[w];
      (*opt)[t][id][i] = cost;
      (*pol)[t][id][i] = p;
    }
  }
}


double compute_sum(W w, unsigned int d, unsigned int s, unsigned int v, std::vector<std::vector<std::vector<double>>> &opt, std::vector<std::vector<std::vector<double>>> &distribution, std::map<W,unsigned int> &h, unsigned int t){
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
