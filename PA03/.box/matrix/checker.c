#include <stdio.h>
#include <stdlib.h>
  FILE *stream;

void error_handling(char *message);

int main(int argc, char *argv[]){

  int temp;
  int total = 0;
  int str_len;

  if(argc != 2){
    printf("Usage %s <matrix>\n", argv[0]);
    exit(1);
  }

  if((stream = fopen(argv[1], "r")) == NULL){
    error_handling("file open failed.");
  }

  while((str_len = fscanf(stream, "%d\n", &temp)) > 0){
    total = total + temp;
    if(total > 0) {
      printf("there is edge in this matrix (%s)\n", argv[1]);
      break;
    }
  }

  if(total == 0) printf("*** There is no element in this matrix (%s)\n", argv[1]);

  return 0;
}


void error_handling(char* message){
  fprintf(stream, "%s\n", message);
  exit(1);
}
