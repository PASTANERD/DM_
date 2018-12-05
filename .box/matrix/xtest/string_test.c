#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CAMP1 "Hello"
#define CAMP2 "World"

int main(int argc, char* argv[]){

  if(argc != 2){
    printf("input any string one\n");
    exit(1);
  }

  char* temp = {CAMP1 + argv[1] + CAMP2};

  printf("%s", temp);

  return 0;

}
