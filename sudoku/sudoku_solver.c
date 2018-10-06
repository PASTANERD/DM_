// 3 In a Row Solver ver 2.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 81
#define MAX_SIZE_ROOT 9
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
    int* data;
    int* solution;
    
    data = malloc(sizeof(int)*MAX_SIZE+1);
    solution = malloc(sizeof(int)*MAX_SIZE+1);
    
    data[MAX_SIZE+1] = 0;
    solution[MAX_SIZE+1] = 0;
    
    printf("Loading Input data. \n");
    LoadData(argv[1], data);
    printf("Test Print for input data --- \n");
    //TestPrint(data);
    TestOutput(data);
    printf("\n");
    
    printf("Generating Formula. \n");
    ConstructSolution(data);
    
    printf("Running Z3 Solver\n");
    Z3Solver();
    
    printf("Extracting Result to output file\n");
    ConvertData(solution);
    printf("Test Print for solution data --- \n");
    //TestPrint(solution);
    TestOutput(solution);
    printf("\n");
    
    MakeOutput(solution);
    
    fclose(stream);
    return 0;
}

void ConstructSolution(int* from_input){
    /*TODO * z3를 이용하여 fomular.txt를 생성하는 기능을 담당하는 모듈 */
    
    if((stream = fopen("formula_sudoku.txt", "w")) == NULL){
        perror("Error: Formula file open failed");
    }
    int x, y ,n ;
    int index = 0;
    
    //Generating 9x9 grid
    for (y = 1 ; y <= 9 ; y++)
        for (x = 1 ; x <= 9 ; x++)
            fprintf(stream, "(declare-const a%d%d Int)\n", y, x) ;
    
    //Reading input file
    
    
    //Assign proper value
    for (y = 1 ; y <= 9 ; y++)
        for (x = 1 ; x <= 9 ; x++){
            int mark = from_input[index ++];
            
            if(mark == 0) fprintf(stream, "(assert (and (<= 1 a%d%d) (<= a%d%d 9)))\n", y, x, y, x) ;
            else fprintf(stream, "(assert (= a%d%d %d))\n", y, x, mark);
        }
    
    
    for (y = 1 ; y <= 9 ; y++){  //every column disticnt
        fprintf(stream, "(assert(distinct");
        for (x = 1 ; x <= 9 ; x++){
            fprintf(stream, " a%d%d",y, x);
        }
        fprintf(stream, "))\n");
    }
    
    for (x = 1 ; x <= 9 ; x++){    //every row distinct
        fprintf(stream, "(assert(distinct");
        for (y = 1 ; y <= 9 ; y++){
            fprintf(stream, " a%d%d",y, x);
        }
        fprintf(stream, "))\n");
    }
    
    
    fprintf(stream, "(assert(distinct");
    for (y = 1 ; y <= 3; y++){//every square disticnt
        for (x = 1 ; x <= 3 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    fprintf(stream, "(assert(distinct");
    for (y = 1 ; y <= 3; y++){//every square disticnt
        for (x = 4 ; x <= 6 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    fprintf(stream, "(assert(distinct");
    for (y = 1 ; y <= 3; y++){//every square disticnt
        for (x = 7 ; x <= 9 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    fprintf(stream, "(assert(distinct");
    for (y = 4 ; y <= 6; y++){//every square disticnt
        for (x = 1 ; x <= 3 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    fprintf(stream, "(assert(distinct");
    for (y = 4 ; y <= 6; y++){//every square disticnt
        for (x = 4 ; x <= 6 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    fprintf(stream, "(assert(distinct");
    for (y = 4 ; y <= 6; y++){//every square disticnt
        for (x = 7 ; x <= 9 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    fprintf(stream, "(assert(distinct");
    for (y = 7 ; y <= 9; y++){//every square disticnt
        for (x = 1 ; x <= 3 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    fprintf(stream, "(assert(distinct");
    for (y = 7 ; y <= 9; y++){//every square disticnt
        for (x = 4 ; x <= 6 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    fprintf(stream, "(assert(distinct");
    for (y = 7 ; y <= 9; y++){//every square disticnt
        for (x = 7 ; x <= 9 ; x++)
            fprintf(stream, " a%d%d",y, x);
    }  fprintf(stream, "))\n");
    
    
    fprintf(stream, "(check-sat)\n(get-model)\n");
    
}

void Z3Solver(){
    system("z3 formula_3.txt >> result_3.txt");
}


void LoadData(char* argument, int* data){
    
    char* box;
    
    box = malloc(sizeof(char)*2);
    
    if((stream = fopen(argument, "r")) != NULL){
        for(int i = 0; i < MAX_SIZE ; i++){
            fscanf(stream, "%s", box);
            if(!strcmp(box, "?") || !strcmp(box, "0")){
                data[i] = 0;
            }
            else if(!strcmp(box, "O")){
                data[i] = 1;
            }
            else if(!strcmp(box, "X")){
                data[i] = 2;
            }
            else continue;
        }
    }
    else{
        perror("Error: input FILE");
    }
    
    free(box);
    
}


void ConvertData(int* solution){
    
    char *line, *result;
    char temp[3];
    
    int index, index1, index2, dumb, value;
    line = malloc(sizeof(char)*MAX_LENGTH);
    
    if((stream = fopen("result_3.txt", "r")) != NULL){
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
    
    if((stream = fopen("output_3.txt", "w")) != NULL){
        for(int i = 0; i < MAX_SIZE ; i++){
            switch(solution[i]){
                case 0:
                    fprintf(stream, "X ");
                case 1:
                    fprintf(stream, "O ");
                default:
                    if(i%MAX_SIZE_ROOT == MAX_SIZE_ROOT -1) fprintf(stream, "\n\r");
                    continue;
            }
        }
    }
    
}

void TestPrint(int* data){
    for(int i = 0 ; i < MAX_SIZE ; i++ ){
        printf("%d) %d\n", i, data[i]);
    }
}

void TestOutput(int *solution){
    for(int i = 0; i < MAX_SIZE ; i++){
        printf("%d ", solution[i]);
        if(i%MAX_SIZE_ROOT == MAX_SIZE_ROOT - 1) printf("\n");
    }
}

