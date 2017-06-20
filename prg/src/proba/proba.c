#include <stdio.h>
#include <stdlib.h>

unsigned int bino(unsigned int k, unsigned int n);
unsigned int C(unsigned int d, unsigned int s);
void uniform_distribution(unsigned int d, unsigned int s, FILE* fd);
void simple_distribution(unsigned int d, unsigned int s, FILE* fd);

int main(int argc, char *argv[]){
  unsigned int d, s, c;
  char *filename = NULL;
  FILE* fd = NULL;
  
  if (argc < 4){
    printf("Use: %s d s filename options\n", argv[0]);
    exit(1);
  }
  d = atoi(argv[1]);
  s = atoi(argv[2]);
  filename = argv[3];
  
  if (filename == NULL){
    perror("filename");
    exit(1);
  }

  fd = fopen(filename, "w");
  if (fd == NULL){
    perror("fopen");
    exit(1);
  }
  fprintf(fd, "%u %u\n", d, s);

  printf("Which distribution do you need ?\n0.Simple distribution\n1.Uniform distribution\n");
  scanf("%u", &c);

  switch(c){
  case 0:
    simple_distribution(d, s, fd);
    break;
  case 1:
    uniform_distribution(d, s, fd);
  default:
    exit(1);
  }

  fclose(fd);

  return 0;
}

void simple_distribution(unsigned int d, unsigned int s, FILE* fd){
  unsigned int i ,j;

  for (i = 0; i < C(d, s); i++){
    for (j = 0; j < (s+1)*d-1; j++)
      if (j/(s+1) == 0 && j%(s+1) == 1)
	fprintf(fd, "1 ");
      else
	fprintf(fd, "0 ");
    if (((s+1)*d-1)/(s+1) == 0 && ((s+1)*d-1)%(s+1) == 1)
      fprintf(fd, "1\n");
    else
      fprintf(fd, "0\n");
  }
}

void uniform_distribution(unsigned int d, unsigned int s, FILE* fd){
  unsigned int i, j;
  double p = 1/(float)((s+1)*d);
  
  for (i = 0; i < C(d, s); i++){
    for (j = 0; j < (s+1)*d-1; j++)
      fprintf(fd, "%f ", p);
    fprintf(fd, "%f\n", p);
  }
}

unsigned int bino(unsigned int k, unsigned int n){
  if (k < 0)
    return 0;
  else if (k == 0)
    return 1;
  else
    return n*bino(k-1,n-1)/k;
}

unsigned int C(unsigned int d, unsigned int s){
  return bino(d+1, (s+1)*(d+1))/(1+s*(d+1));
}
