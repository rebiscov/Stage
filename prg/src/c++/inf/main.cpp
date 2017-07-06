#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unordered_map>
#include <thread>
#include <cstring>
#include "../states.hpp"
#include "../funs.h"
#include "../math.h"

#define NB_THREADS 4

/* some prototypes */

double compute_sum(W w, unsigned int d, unsigned int s, unsigned int v, double *u, double ***distribution, std::unordered_map<W, unsigned int> &h);

void compute_j(W *w_set,unsigned int id_thread, unsigned int d, unsigned int s, unsigned int v_max, unsigned int space, double *u_n, double *u, double ***distribution, std::unordered_map<W,unsigned int> *h);

double span(double *u, unsigned int space);
double compute_span(double *u1, double *u2, unsigned int space);

/* MAIN FUNCTION */

int main(int argc, char* argv[]){
  bool debug;
  printf("DP algorithm, C++ second version\n");

  if (argc < 5 ){
    printf("Use: %s epsilon v_max distribution_file out_file options\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  if (argc >= 6)
    debug = strcmp("d", argv[5]) == 0;
  

  /* I define all the important variables, I extract the distribution in the file given by the user and I initialize the array u */

  /* Defining epsilon and maximal speed */
  double epsilon = atof(argv[1]);
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
    exit(EXIT_FAILURE);
  }

  if (debug)
    printf("done.\n");  

  /* defining array u */
  
  double *u = NULL, *up = NULL;
  u = new double[space];
  up = new double[space];

  if (u == NULL){
    printf("Error: memory allocation for 'u' did not work!\n");
    exit(EXIT_FAILURE);
  }

  if (up == NULL){
    printf("Error: memory allocation for 'up' did not work!\n");
    exit(EXIT_FAILURE);
  }

  for (unsigned int i = 0; i < space; i++)
    u[i] = 0;
  u[0] = 1;

  for (unsigned int i = 0; i < space; i++)
    up[i] = 0;

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
  std::thread threads[NB_THREADS];
  double sp = compute_span(u, up, space);
  
  while (sp > epsilon){
    if (debug)
      printf("COMPUTATION current span = %f, epsilon = %f\n", sp, epsilon);
    double *un = new double[space];
    
    for (unsigned int k = 0; k < NB_THREADS; k++)
      threads[k] = std::thread (compute_j, w_set, k, d, s, v_max, space, un, u, distribution, &h);
    for (unsigned int k = 0; k < NB_THREADS; k++)
      threads[k].join();
    
    for (unsigned int i = 0; i < space; i++){
      up[i] = u[i];
      u[i] = un[i];
    }

    sp = compute_span(u, up, space);
  }

  
  printf("Bye!\n");
  
  return 0;
}

void compute_j(W *w_set,unsigned int id_thread, unsigned int d, unsigned int s, unsigned int v_max, unsigned int space, double *u_n, double *u, double ***distribution, std::unordered_map<W,unsigned int> *h){
  unsigned int minimum = (id_thread+1)*(space/NB_THREADS);
  if (id_thread == NB_THREADS -1)
    minimum = space;
  for (unsigned int k = id_thread*(space/NB_THREADS); k < minimum; k++){ /* We explore all states */
    W& w = w_set[k];

    for (unsigned int i = 0; i <= v_max; i++){ /* We explore all speeds */
      double cost = f(i, v_max, 1) + c(v_max) + compute_sum(w, d, s, v_max, u, distribution, *h);
      unsigned int p = v_max;

      for (unsigned int j = 0; j < v_max; j++){
	if (j >= w.get(1)){
	  double c_j = c(j);
	  if (c_j >= cost)
	    break;
	  double co = f(i,j,1) + c_j + compute_sum(w, d, s, j, u, distribution, *h);
	  if (co < cost){
	    cost = co;
	    p = j;
	  }
	}
      }
      unsigned int id = (*h)[w];
      u_n[k] = cost;
    }
  }
}


double compute_sum(W w, unsigned int d, unsigned int s, unsigned int v, double *u, double ***distribution, std::unordered_map<W,unsigned int> &h){
  w.inc_time(v);
  unsigned int id_w = h[w];
  double sum = 0;

  for (unsigned int i = 0; i <= s; i++){
    W wp = w;
    for (unsigned int j = 1; j <= d; j++){
      unsigned int k = d-j+1;
      wp.set(k, wp.get(k)+i);
      sum += distribution[id_w][k-1][i] * u[h[wp]];;
    }
  }

  return sum;
}

double span(double *u, unsigned int space){
  double min = u[0], max = u[0];
  for (unsigned int i = 0; i < space; i++){
    min = u[i] < min ? u[i] : min;
    max = u[i] > max ? u[i] : max;
  }
  return max - min;
}

double compute_span(double *u1, double *u2, unsigned int space){
  double *u = NULL;
  u = new double[space];

  if (u == NULL){
    printf("Error: failed allocation in compute_space\n");
    exit(EXIT_FAILURE);
  }

  for (unsigned int i = 0; i < space; i++)
    u[i] = u1[i] - u2[i];

  return span(u, space);
}
