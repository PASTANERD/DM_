#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void ExecFunc();

int main(int argc, char *argv[]){

  ExecFunc();

  return 0;
}

void ExecFunc(){
  
  //system("z3 formula_sudoku.txt >> result_sudoku.txt");
  
  execlp("z3", "z3", "formula_sudoku.txt", ">>", "result_sudoku.txt", NULL);

}

