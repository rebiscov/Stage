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

#define MAX 500
#define PORT 1337

int main(int argc, char *argv[]){
  int sock;
  struct sockaddr_in dst;
  char rcv[MAX];

  sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&dst, 0, sizeof(dst));
  dst.sin_family = AF_INET;
  dst.sin_addr.s_addr = inet_addr("127.0.0.1");
  dst.sin_port = htons(PORT);

  return 0;
}
