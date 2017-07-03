#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "states.hpp"

#define MAX 5000
#define PORT 1338
#define NB_PINGS 10

void rcv_line(char* rcv, int sock);
int pong(int sock);
void send_w(W w, int sock);

unsigned int t, d, s, v_max, space;
std::unordered_map<W, unsigned int> h;
int n;

double ***distri;

int main(int argc, char *argv[]){
  int sock, len;
  struct sockaddr_in dst;
  char rcv[MAX];

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
  W a(d);
  a.print_w();

  send_w(a, sock);

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
  char buff[MAX];
  unsigned int d = w.size();
  for (unsigned int i = 1; i < d; i++){
    sprintf(buff, "%d ", w.get(i));
    send(sock, buff, strlen(buff), 0);
  }
  sprintf(buff, "%d\n", w.get(d));
  send(sock, buff, strlen(buff), 0);
}
