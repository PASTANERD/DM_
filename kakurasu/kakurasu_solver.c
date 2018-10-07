// Sudoku ver 2.1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define KAKU_SIZE 16
#define MAX_SIZE 64
#define MAX_SIZE_ROOT 8
#define MAX_LENGTH 100
FILE *stream;

void LoadData(char* argument, int* data);
void ConstructSolution(int* data);
void ConvertData(int* solution);
void MakeOutput(int* solution);

void Z3Solver();

void TestPrint(int* data);
void TestOutput(int *solution);

int main(int argc, char* argv[]){
    
    pid_t pid;
    int status;
    
    pid = fork();
    if(pid == 0){
        int* data;
        data = malloc(sizeof(int)*MAX_SIZE+1);
        data[MAX_SIZE+1] = 0;
        
        printf("Loading Input data. \n");
        LoadData(argv[1], data);
        TestPrint(data);
        ConstructSolution(data);
        printf("\n");
        
        sleep(1);
        exit(1);
    }
    else if (pid < 0){
        perror("Error: fork failed");
    }
    else{
        wait(&status);
        int* solution;
        solution = malloc(sizeof(int)*MAX_SIZE+1);
        solution[MAX_SIZE+1] = 0;
        
        Z3Solver(solution);
        //printf("Extracting Result to output file\n");
        ConvertData(solution);
        //printf("Test Print for solution data --- \n");
        //TestPrint(solution);
        printf("Solution: \n");
        TestOutput(solution);
        
        
        MakeOutput(solution);
        
        
        fclose(stream);
        return 0;
        
    }
  printf("Test Print for input data --- \n");
  
}

void ConstructSolution(int* from_input){
  /*TODO * z3를 이용하여 fomular.txt를 생성하는 기능을 담당하는 모듈 */

  if((stream = fopen("formula_kakurasu.txt", "w+t")) == NULL){
    perror("Error: Formula file open failed");
  }

    //int b[17];
    int num;
    int x,y;
    int z=9;
    
    
    //declare-const a and b
    for (y = 1 ; y <= 8 ; y++)
        for (x = 1 ; x <= 8 ; x++)
            fprintf(stream, "(declare-const a%d%d Int)\n", y, x) ;
    
    for (y = 1 ; y <= 8 ; y++)
        for (x = 1 ; x <= 8 ; x++)
            fprintf(stream, "(declare-const b%d%d Int)\n", y, x) ;
    
    
    //(assert (or (= axy 0) (= axy XorY)))
    
    for (y = 1 ; y <= 8 ; y++)
        for (x = 1 ; x <= 8 ; x++)
            fprintf(stream, "(assert (or (= a%d%d %d) (= %d a%d%d)))\n", y, x,x,0,y,x) ;
    
    for (y = 1 ; y <= 8 ; y++)
        for (x = 1 ; x <= 8 ; x++)
            fprintf(stream, "(assert (or (= b%d%d %d) (= %d b%d%d)))\n", y, x,y,0,y,x) ;
    
    
    //assert row sum
    
    for (y = 1 ; y <= 8 ; y++){         //가로합에 대한 결과값 b[1~8]
        fprintf(stream, "(assert (= (+") ;
        for (x = 1 ; x <= 8 ; x++)
            fprintf(stream, " a%d%d ", y, x) ;
        fprintf(stream, ")%d))\n",from_input[y]);
    }
    
    //assert column sum
    
    for (x = 1 ; x <= 8 ; x++){            //세로합에 대한 결과값 b[9~16]
        fprintf(stream, "(assert (= (+") ;
        for (y = 1 ; y <= 8 ; y++)
            fprintf(stream, " b%d%d ", y, x) ;
        fprintf(stream, ")%d))\n",from_input[z++]);
    }
    
    //(assert (or (= (+ a11 b11) 0) (= (+ a11 b11) x+y))))
    
    for (y = 1 ; y <= 8 ; y++)
        for (x = 1 ; x <= 8 ; x++)
            fprintf(stream, "(assert (or (= (+ a%d%d b%d%d) 0) (= (+ a%d%d b%d%d) %d)))\n", y, x,y,x,y,x,y,x,x+y) ;
    
    
    
    
    //chech-sat and print result
    fprintf(stream, "(check-sat)\n(get-model)\n") ;
}

void Z3Solver(){
  
    system("z3 formula_kakurasu.txt >> result_kakurasu.txt");
  
}


void LoadData(char* argument, int* data){
    int i;
    int box;
    
    //box = malloc(sizeof(char)*2);
    
    if((stream = fopen(argument, "r")) != NULL){
        for(i = 1; i <= KAKU_SIZE ; i++){
            fscanf(stream, "%d", &box);
            data[i] = box;
        }
        data[i] = 0;
    }
    else{
        perror("Error: input FILE");
    }
    
    //free(box);
    
}


void ConvertData(int* solution){

  char *line, *result;
  char temp[3];

  int index, index1, index2, dumb, value;
  line = malloc(sizeof(char)*MAX_LENGTH);
  
  if((stream = fopen("result_kakurasu.txt", "r")) != NULL){
    if((result = fgets(line, MAX_LENGTH, stream)) != NULL){
      if(!strncmp(result, "unsat", 5)){
        printf("This game is unsatisfiable.");
      }
      else{
        result = fgets(line, MAX_LENGTH, stream);
        do{
          // get index
          if((result = fgets(line, MAX_LENGTH, stream)) != NULL){
            memcpy(temp, &result[15], 2);
            temp[2] = 0;
            dumb = atoi(temp) - 11;
            index1 = (dumb/10)*MAX_SIZE_ROOT;
            index2 = dumb%10;
            index = index1 + index2;
          }
          //get value
          if((result = fgets(line, MAX_LENGTH, stream)) != NULL){
            memcpy(temp, &result[4], 1);
            temp[1] = 0;
            value = atoi(temp);
          }
          solution[index] = value;
        }while(!feof(stream));
        
      }
    }
  }
  else{
    perror("Error: result.txt opne failed");
  }

  free(line);
}

void MakeOutput(int* solution){

  if((stream = fopen("output_kakurasu.txt", "w+r")) != NULL){
      for(int i = 0; i < MAX_SIZE ; i++){
          if(solution[i] == 0){
              fprintf(stream, "O ");
          }
          else{
              fprintf(stream, "X ");
          }
          
          if(i%MAX_SIZE_ROOT == MAX_SIZE_ROOT -1) fprintf(stream, "\n");
      }
  }
    
  
}

void TestPrint(int* data){
  for(int i = 1 ; i <= KAKU_SIZE ; i++ ){
     printf("%d ", data[i]);
    if(i%MAX_SIZE_ROOT == 0) printf("\n");
  }
}

void TestOutput(int *solution){
  for(int i = 0; i < MAX_SIZE ; i++){
      if(solution[i] == 0){
          printf("O ");
      }
      else{
          printf("X ");
      }
      
      if(i%MAX_SIZE_ROOT == MAX_SIZE_ROOT -1) printf("\n");
  }
}
