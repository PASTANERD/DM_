#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NODESIZE 5128
#define SORTED_X "sorted_data_k1.txt"
#define SORTED_Y "sorted_data_k2.txt"
#define NODE_FILE "node_list_file.txt"
FILE *stream_k1, *stream_k2, *stream_new;

void read_all();
void rest_part();


int main(int argc, char *argv[]){

  int NODE[NODESIZE];

  stream_k1 = fopen(SORTED_X, "r");
  if(stream_k1 == NULL){
    fprintf(stderr, "stream_k1 open error;\n");
    exit(1);
  }
  stream_k2 = fopen(SORTED_Y, "r");
  if(stream_k2 == NULL){
    fprintf(stderr, "stream_k2 open error;\n");
    exit(1);
  }
  stream_new = fopen(NODE_FILE, "w");
  if(stream_new == NULL){
    fprintf(stderr, "stream_new open error;\n");
    exit(1);
  }

  read_all();
  rest_part();
  return 0;
}



void read_all(){
  int str_len;
  int dump = 0;
  int temp1, temp2;

  while((str_len = fscanf(stream_k1, "%d %d\n", &temp1, &temp2)) > 0){
    printf("*");
      if(dump == temp1) continue;
      else{
        dump = temp1;
        fprintf(stream_new, "%d\n", dump);
      }
  }

  //fseek(stream_k1, 0L, SEEK_SET);
  //printf("File point : %ld", ftell(stream_k1));

}

void rest_part(){
  int str_len, str_len_2;
  int dump = 0;
  int flag = 0;
  int trap = 0;
  int temp1, temp2;
  int temp3, temp4;

  while((str_len = fscanf(stream_k2, "%d %d\n", &temp3, &temp4)) > 0){
    while((str_len_2 = fscanf(stream_k1, "%d %d\n", &temp1, &temp2)) > 0){
      dump++;
      if((dump%21270) == 1)printf("ho");
      if(temp1 == temp4) flag++;
    }
    if(flag == 0) {
      if(trap == temp4){
        continue;
      }
      else{
        trap = temp4;
        fprintf(stream_new, "%d\n", temp4);
        printf("!");
      }
    }
    dump = 0;
    flag = 0;
    fseek(stream_k1, 0L, SEEK_SET);
  }
}
