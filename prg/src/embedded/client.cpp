#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <unordered_map>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "states.hpp"
#include "math.hpp"

#define MAX 5000
#define PORT 1338
#define NB_PINGS 10

void rcv_line(char* rcv, int sock);
int pong(int sock);
void send_w(W w, int sock);
W rcv_w(int sock);
void rcv_preds(W *preds, int sock);
void rand_couple(unsigned int id_w, unsigned int &delta, unsigned int &sigma);

unsigned int t, d, s, v_max, space, num;
std::unordered_map<W, unsigned int> h;
int n;

double ***distri;

int main(int argc, char *argv[]){
  int sock, len;
  struct sockaddr_in dst;
  char rcv[MAX];

  srand(time(NULL));

  if (argc < 6){
    printf("Use: %s t d s v_max distri_file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

    /* Assignation of the important variables */
  
  t = atoi(argv[1]);
  d = atoi(argv[2]);
  s = atoi(argv[3]);
  v_max = atoi(argv[4]);
  space = state_space(d, s);
  num = 0;

  /* Defining hashtable and w_set */

  W *w_set = NULL;
  w_set = compute_w(d, s);

  for (unsigned int i = 0; i < space; i++)
    h[w_set[i]] = i;

  /* Loading distri */
  
  distri = new double**[space];

  for (unsigned int i = 0; i < space; i++){
    distri[i] = new double*[d];
    for (unsigned int j = 0; j < d; j++)
      distri[i][j] = new double[s+1];
  }

  FILE *fd = NULL;
  fd = fopen(argv[5], "r");

  if (fd == NULL){
    perror("fopen()");
    exit(EXIT_FAILURE);
  }

  for (unsigned int k = 0; k < space; k++){
    for(unsigned int i = 1; i <= d; i++)
      for (unsigned int j = 0; j <= s; j++)
	fscanf(fd, "%lf", &distri[k][i-1][j]);
  }

  fclose(fd);

  /* Networking */
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1){
    perror("sock()");
    exit(EXIT_FAILURE);
  }

  memset(&dst, 0, sizeof(dst));
  dst.sin_family = AF_INET;
  dst.sin_addr.s_addr = inet_addr("127.0.0.1");
  dst.sin_port = htons(PORT);

  if (connect(sock, (struct sockaddr *)&dst, sizeof(sockaddr_in)) == -1){
    perror("connect()");
    exit(EXIT_FAILURE);
  }

  n = pong(sock);

  W *preds = NULL;
  preds = new W[pow(d*(s+1), (unsigned int)n)];
  
  W w(d);

  send_w(w, sock);
  rcv_preds(preds, sock);

  for (unsigned int i = 0; i < pow(d*(s+1), (unsigned int)n); i++)
    preds[i].print_w();
  
  close(sock);

  return 0;
}

void rcv_line(char* rcv, int sock){
  char* buff = rcv;
  int len;
  do{
    len = recv(sock, buff, MAX, 0);
    buff += len;
    buff[0] = '\0';
  } while(buff[-1] != '\n');
  
  buff[0] = '\0';
}

int pong (int sock){
  char buff[MAX];
  int len, res;
  for (unsigned int i = 0; i < NB_PINGS; i++){
    recv(sock, buff, MAX, 0);
    send(sock, "PONG", 4, 0);
  }
  len = recv(sock, buff, MAX, 0);
  buff[len] = '\0';
  res = atoi(buff);
  printf("number of intervals = %d\n", res+1);
  
  return res + 1;
}

void send_w(W w, int sock){
  char buff[MAX], recup[10];
  unsigned int d = w.size();
  for (unsigned int i = 1; i < d; i++){
    sprintf(buff, "%d ", w.get(i));
    send(sock, buff, strlen(buff), 0);
  }
  sprintf(buff, "%d\n", w.get(d));
  send(sock, buff, strlen(buff), 0);

  recv(sock, recup, 10, 0);
}

W rcv_w(int sock){
  char buff[MAX];
  W res(d);  
  unsigned int *tab = NULL;
  tab = new unsigned int[d];
  if (tab == NULL){
    printf("Error with allocation in rcv_w\n");
    exit(EXIT_FAILURE);
  }

  rcv_line(buff, sock);
  tab[0] = (unsigned int)atoi(strtok(buff, " \n"));
  
  for (unsigned int i = 1; i < d; i++)
    tab[i] = (unsigned int)atoi(strtok(NULL, " \n"));
  for (unsigned int i = d; i > 0; i--)
    res.set(i, tab[i-1]);
  delete tab;

  send(sock, "ok", 2, 0);
  
  return res;
}

void rcv_preds(W *preds, int sock){
  for (unsigned int i = 0; i < pow(d*(s+1), (unsigned int)n); i++){
    preds[i] = rcv_w(sock);
  }
}

void rand_couple(unsigned int id_w, unsigned int &delta, unsigned int &sigma){
  double *cumulative = new double[d*(s+1)];
  cumulative[0] = distri[id_w][0][0];
  for (unsigned int i = 0; i <= d*(s+1); i++)
    cumulative[i] = distri[id_w][i/(s+1)][i%(s+1)] + cumulative[i-1];
  double r = rand() / RAND_MAX;
  unsigned int i = 0;
  while (cumulative[i] < 0)
    i++;
  delta = (i/(s+1)) + 1;
  sigma = i % (s+1);
}


