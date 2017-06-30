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
#define PORT 1337
#define NB_PINGS 10

void rcv_line(char* rcv, int sock);
int pong(int sock);

int main(int argc, char *argv[]){
  int sock, len;
  unsigned int d = 2, s = 2;
  struct sockaddr_in dst;
  char rcv[MAX];

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

  int n = pong(sock);

  rcv_line(rcv, sock);
  printf("%s", rcv);

  close(sock);

  return 0;
}

void rcv_line(char* rcv, int sock){
  char* buff = rcv;
  int len;
  do{
    len = recv(sock, rcv, MAX, 0);
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
