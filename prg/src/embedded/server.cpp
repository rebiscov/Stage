#include <cstdio>
#include <cstring>
#include <ctime>
#include <unordered_map>
#include <vector>
#include <utility>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "states.hpp"
#include "math.hpp"

#define MAX 5000
#define PORT 1338
#define THRESHOLD 0.1
#define ERROR 0.05
#define NB_PINGS 10
#define DEBUG

int ping(int conn);
void compute_preds(W *preds, W state, int prof, int index, unsigned int l_speed, unsigned int c_t);
void rcv_line(char* rcv, int sock);
W rcv_w(int conn);
void send_preds(const W *preds, int conn);
void send_w(W w, int conn);

unsigned int t, d = 3, s = 2, v_max = 6, space, last_speed = 0;
unsigned int ***pol;
int n;
std::unordered_map<W, unsigned int> h;

int main(int argc, char *argv[]){
  struct sockaddr_in dst, srv;
  int sock;
  socklen_t socksize = sizeof(struct sockaddr_in);
  char res[MAX], rcv[MAX];

  if (argc < 6){
    printf("Use: %s t d s v_max pol_file\n", argv[0]);
    return 0;
  }

  /* Assignation of the important variables */
  
  t = atoi(argv[1]);
  d = atoi(argv[2]);
  s = atoi(argv[3]);
  v_max = atoi(argv[4]);
  space = state_space(d, s);

  /* Defining hashtable and w_set */

  W *w_set = NULL;
  w_set = compute_w(d, s);

  for (unsigned int i = 0; i < space; i++)
    h[w_set[i]] = i;

  /* Reading the optimal policies */
  FILE* fd = NULL;
  fd = fopen(argv[5], "r");

  if (fd == NULL){
    perror("fopen()");
    exit(EXIT_FAILURE);
  }

  pol = new unsigned int**[t+1];
  if (pol == NULL){
    printf("Error: memory allocation for pol did not work!\n");
    exit(1);
  }

  for (unsigned int i = 0; i <= t; i++){
    pol[i] = new unsigned int*[space];
    for (unsigned int j = 0; j < space; j++)
      pol[i][j] = new unsigned int[v_max+1];
  }
  
  for (unsigned int i = 1; i <= t; i++)
    for (unsigned int j = 0; j < space; j++)
      for (unsigned int k = 0; k <= v_max; k++)
	fscanf(fd, "%u", &pol[i][j][k]);
   
  fclose(fd);
  
  memset(&srv, 0, sizeof(srv));
  srv.sin_family = AF_INET;
  srv.sin_addr.s_addr = htonl(INADDR_ANY);
  srv.sin_port = htons(PORT);

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1){
    perror("sock()");
    exit(EXIT_FAILURE);
  }

  bind(sock, (struct sockaddr *)&srv, sizeof(struct sockaddr));

  listen(sock, 1);
  
  int conn = accept(sock, (struct sockaddr *)&dst, &socksize);
  if (conn == -1){
    perror("accept()");
    exit(EXIT_FAILURE);
  }

  n = ping(conn);

  W *preds = NULL;
  preds = new W[pow(d*(s+1), (unsigned int)n)];

  W w(d);

  w = rcv_w(conn);

  compute_preds(preds, w, 0, 0, last_speed, 0);
  send_preds(preds, conn);

  for (unsigned int i = 0; i < pow(d*(s+1), (unsigned int)n); i++)
    preds[i].print_w();
  
  close(conn);
  close(sock);
  
  return 0;
}

int ping(int conn){
  double max = -1.;
  clock_t t;
  char buff[MAX];
  int len, res;

  for (unsigned int i = 0; i < NB_PINGS; i++){
    t = clock();
    send(conn, "PING", 4, 0);
    len = recv(conn, buff, MAX, 0);
    t = clock() - t;
    if ((double)(t/CLOCKS_PER_SEC) > max)
      max = (double)(t/CLOCKS_PER_SEC);
  }
  printf("Longest ping: %f\n", max);
  max += ERROR;
  res = (int)(max/THRESHOLD);
  sprintf(buff, "%d", res);
  send(conn, buff, strlen(buff), 0);
  printf("number of intervals = %d\n", res+1);

  return res + 1;
}

void compute_preds(W *preds, W state, int prof, int index, unsigned int l_speed, unsigned int c_t){
  for (unsigned int i = 0; i < d; i++)
    for (unsigned int j = 0 ; j <= s; j++){
      W w = state;
      int id = index;
      w.add_work(i+1, j);
      unsigned int speed = pol[c_t+prof][h[state]][l_speed];
      w.inc_time(speed);
      id += (int)(pow((s+1)*d, prof) * ((s+1)*i+j));
      if (prof < n-1)
	compute_preds(preds, w, prof+1, id, speed, c_t);
      else
	preds[id] = w;
    }
}

void rcv_line(char* rcv, int conn){
  char* buff = rcv;
  int len;
  do{
    len = recv(conn, buff, MAX, 0);
    buff += len;
    buff[0] = '\0';
  } while(buff[-1] != '\n');
  
  buff[0] = '\0';
}

W rcv_w(int conn){
  char buff[MAX];
  W res(d);  
  unsigned int *tab = NULL;
  tab = new unsigned int[d];
  if (tab == NULL){
    printf("Error with allocation in rcv_w\n");
    exit(EXIT_FAILURE);
  }

  rcv_line(buff, conn);
  tab[0] = (unsigned int)atoi(strtok(buff, " \n"));
  
  for (unsigned int i = 1; i < d; i++)
    tab[i] = (unsigned int)atoi(strtok(NULL, " \n"));
  for (unsigned int i = d; i > 0; i--)
    res.set(i, tab[i-1]);
  delete tab;

  send(conn, "ok", 2, 0);
  
  return res;
}

void send_w(W w, int conn){
  char buff[MAX], recup[10];
  unsigned int d = w.size();
  for (unsigned int i = 1; i < d; i++){
    sprintf(buff, "%d ", w.get(i));
    send(conn, buff, strlen(buff), 0);
  }
  sprintf(buff, "%d\n", w.get(d));
  send(conn, buff, strlen(buff), 0);

  recv(conn, recup, 10, 0);
}

void send_preds(const W *preds, int conn){
  char buff[5];
  for (unsigned int i = 0; i < pow(d*(s+1), (unsigned int)n); i++){
    send_w(preds[i], conn);
  }
}
