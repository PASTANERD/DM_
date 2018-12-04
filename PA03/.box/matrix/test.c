#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(){

  int MAX_X = 557143602;
  int MAX_Y = 560452336;

  int *Size = (int*)malloc(sizeof(int)*50*50);
  printf("Size of *Size = %d\n", sizeof(Size));
  printf("LONG_MAX = %d, MAX_X = %d MAX_Y = %d\n", INT_MAX, MAX_X, MAX_Y);
  printf("X - I = %d, Y - I = %d", MAX_X - INT_MAX, MAX_Y - INT_MAX );
  return 0;
}
