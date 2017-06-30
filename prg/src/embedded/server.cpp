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

#define MAX 5000
#define PORT 1337
#define THRESHOLD 0.1
#define ERROR 0.05
#define NB_PINGS 10

int ping(int conn);

int main(int argc, char *argv[]){
  struct sockaddr_in dst, srv;
  int sock;
  unsigned int d = 2, s = 2;
  socklen_t socksize = sizeof(struct sockaddr_in);
  char res[MAX], rcv[MAX];

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

  int n = ping(conn);
  
  for (unsigned int i = 0; i < d; i++)
    for (unsigned int j = 0; j <= s; j++){
      if (i == d-1 && j == s){
	sprintf(rcv, "%d\n", i*d+j);
	send(conn, rcv, strlen(rcv), 0);
      }
      else{
	sprintf(rcv, "%d ", i*d+j);
	send(conn, rcv, strlen(rcv), 0);
      }
    }
  
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
