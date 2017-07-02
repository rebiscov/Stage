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
void compute_preds(W *preds, W state, int prof, int index);
void rcv_line(char* rcv, int sock);
W rcv_w(int conn);

unsigned int t, d = 3, s = 2, v_max, space;
unsigned int ***opt;
int n;

int main(int argc, char *argv[]){
  struct sockaddr_in dst, srv;
  int sock;
  socklen_t socksize = sizeof(struct sockaddr_in);
  char res[MAX], rcv[MAX];

  #ifndef DEBUG
  
  if (argc < 6){
    printf("Use: %s t d s v_max opt_file\n", argv[0]);
    return 0;
  }

  /* Assignation of the important variables */
  
  t = atoi(argv[1]);
  d = atoi(argv[2]);
  s = atoi(argv[3]);
  v_max = atoi(argv[4]);
  space = state_space(d, s);

  /* Reading the optimal policies */
  FILE* fd = NULL;
  fd = fopen(argv[5], "r");

  if (fd == NULL){
    perror("fopen()");
    exit(EXIT_FAILURE);
  }

  for (unsigned int i = 0; i < t; i++)
    for (unsigned int j = 0; j < space; j++)
      for (unsigned int k = 0; k <= s; k++)
	fscanf(fd, "%u", &pol[i][j][k]);
  
  fclose(fd);

  #endif
  
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
  n = 2;

  W a = rcv_w(conn);
  a.print_w();

  W *preds = NULL;
  preds = new W[pow(d*(s+1), (unsigned int)n)];
  printf("pow: %u\n", pow(d*(s+1), (unsigned int)n));

  compute_preds(preds, a, 0, 0);

  for (unsigned int i = 0; i < pow(d*(s+1), (unsigned int) n); i++)
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

void compute_preds(W *preds, W state, int prof, int index){
  printf("p:%d\n", prof);
  for (unsigned int i = 0; i < d; i++)
    for (unsigned int j = 0 ; j <= s; j++){
      W w = state;
      int id = index;
      w.add_work(i+1, j);      
      id += (int)(pow((s+1)*d, prof) * ((s+1)*i+j));
      if (prof < n-1)
	compute_preds(preds, w, prof+1, id);
      else{
	printf("id: %d, d = %u, s = %u, prof = %d\n", id, i+1, j, prof);
	preds[id] = w;
      }
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
  
  return res;
}
