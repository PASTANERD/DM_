#include <stdio.h>

int main(int argc, char *argv[]){
  FILE *stream;
  int str_len;
  int temp1, temp2, item;
  int compare = 0;
  int X[5128];
  int Y;

  int i = 0;
  stream = fopen("unique.txt", "r");
  while((str_len = fscanf(stream, "%d\n", &X[i]))) i++;

  fclose(stream);
  stream = fopen("total_edge.txt", "r");
  while((str_len = fscanf(stream, "%d %d %d\n", &temp1, &temp2, &item)) > 0 ){
    if(compare <= item){
      compare = item;
      X = temp1;
      Y = temp2;
    }
  }

  printf("most longest minimum distance is \n");
  printf("%d %d %d\n", X, Y, compare);

  return 0;
}
