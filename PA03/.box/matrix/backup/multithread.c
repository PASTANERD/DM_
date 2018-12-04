#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

#define RES_MATRIX "matrix.txt" // original : maxtrix.txt 
#define TRANSITIVE "transitive_"
#define TRAN_LENGTH 11
#define DOT_TXT ".txt"
#define DOT_LENGTH 4
#define THREAD_INPUT 100
#define COLUMN_NUMBER 2 // actuall number of column multpilied by 2 (this time is 2 = 1(# of column) * 2)

void MatMul_MT(int dim, int *A, int *B, int *Product, int noThread);
int file_reading(FILE *stream);
int file_writing(FILE *stream, char *depth, int size, int *Product);
void matrix_setting(FILE *stream, int *A, int *B);

void TEST_MESSAGE(char* message);

int main(int argc, char *argv[])
{

  FILE *stream;
  char *src_matrix;
  char *depth;
	int dim = 0;
	int noThread = 10;

	struct timeval t1, t2;
	float elapsed_time = 0.F;

	if(argc != 3){
		printf("Usage: %s <src_matrix> <depth>\n", argv[0]);
		exit(0);
	}
  src_matrix = argv[1];
  depth = argv[2];
	srand(time(NULL));

  if((stream = fopen(src_matrix, "r")) == NULL){
    fprintf(stderr, "file open failed (%s)", argv[1]);
    exit(0);
  }
  
  dim = file_reading(stream);
  printf("dimension is %d\n", dim);
	TEST_MESSAGE("file reading finished.");
  noThread = (int) dim / THREAD_INPUT;
  printf("number of thread = %d\n", noThread); 
  
  int *R_1 = (int*)malloc(dim * dim * sizeof(int));
  int *R_2 = (int*)malloc(dim * dim * sizeof(int));
  int *Product = (int*)malloc(dim * dim * sizeof(int));
	
	if(R_1 == NULL || R_2 == NULL || Product == NULL){
		printf("Failed to allocate memory.\n");
    exit(1);
  }

  matrix_setting(stream,R_1, R_2);

  TEST_MESSAGE("matrix setting finished.");

  TEST_MESSAGE("matrix multiplication processing now.");
	gettimeofday(&t1, NULL);

	MatMul_MT(dim, R_1, R_2, Product, noThread);

	gettimeofday(&t2, NULL);
	elapsed_time = (t2.tv_sec + t2.tv_usec / 1000000.) - (t1.tv_sec + t1.tv_usec / 1000000.);
	printf("elapsed time (%d threads)\t= %15f sec\n", noThread, elapsed_time);

  TEST_MESSAGE("calculation finished, file writing now.");

  if(file_writing(stream, depth, dim, Product)){
    TEST_MESSAGE("Writing error.");
  }
  TEST_MESSAGE("all process is done. program exit.");

	free(R_1);
	free(R_2);
	free(Product);
  fclose(stream);
	return 0;
}


int file_reading(FILE *stream){

  long file_size;
  
  fseek(stream, 0, SEEK_END);
  file_size = ftell(stream)/COLUMN_NUMBER;
  fseek(stream, 0, SEEK_SET);

  double root_size = sqrt(file_size);
  
  printf("file size is %ld, dimension is %d\n", file_size, (int) root_size);

  return (int) root_size;
}

void matrix_setting(FILE *stream, int *A, int *B){
  int str_len, temp;
  int i = 0;

  while((str_len = fscanf(stream, "%d\n", &temp)) > 0 ){
    A[i] = temp;
    B[i] = temp;
    i++;
  }
}

int file_writing(FILE *stream, char *depth, int size, int *Product){

  int i;
  char* file_name;
  int name_length = TRAN_LENGTH + DOT_LENGTH + strlen(depth);
  file_name = (char*)malloc(sizeof(char)* name_length);


  strcpy(file_name, TRANSITIVE);
  strcat(file_name, depth);
  strcat(file_name, DOT_TXT);
  file_name[name_length] = '\0';


  stream = fopen(file_name, "w");
  
  if(stream  == NULL) return 1;
  for(i = 0 ; i < size * size ; i++){
    fprintf(stream, "%d\n", Product[i]);
  }

  free(file_name);

  return 0;
}

typedef struct {
	int id;					// thread index
	int noThread;
	int dim;
	int *A, *B, *Product;
} ThreadInfo;

void* MatMul_ThreadFn(void *param);

void MatMul_MT(int dim, int *A, int *B, int *Product, int noThread)
{
	pthread_t *tid = (pthread_t*)malloc(sizeof(pthread_t)*noThread);		// dynamically allocate pthread_t-type array whose length is noThread
	ThreadInfo *threadInfo = (ThreadInfo*)malloc(sizeof(ThreadInfo)*noThread);	// dynamically allocate ThreadInfo-type array whose length is noThread

  TEST_MESSAGE("Assigning threads");
	for(int i = 0; i < noThread ; i++)		// Repeat for noThread times
	{
    fprintf(stderr, "thread[%d] is running now.\n", i);
		threadInfo[i].id = i;			// Fill threadInfo[i] with appropriate values
							// ex) threadInfo[i].id = i;
		threadInfo[i].noThread = noThread;	// threadInfo[i].noThread = noThread;
		threadInfo[i].dim = dim;
		threadInfo[i].A = A;	
		threadInfo[i].B = B;
		threadInfo[i].Product = Product;	
		pthread_create(&tid[i], 0, MatMul_ThreadFn, &threadInfo[i]);
	}

  TEST_MESSAGE("Assigning is done.");
    

	for(int i = 0; i < noThread ; i++)
	{
		pthread_join(tid[i],NULL);
	}
	

  TEST_MESSAGE("All threads are terminated.");
    
		    // wait for the children threads to terminate
		    // call pthread_join t-times
    	
	free(tid);	
  	free(threadInfo);		  // deallocate pthread_t and ThreadInfo array
}

void* MatMul_ThreadFn(void *param)              // thread function
{
    ThreadInfo *pInfo = (ThreadInfo *)param;
    int dim = pInfo->dim;
    int id = pInfo->id;
    int noThread = pInfo->noThread;
    int *A = pInfo->A;
    int *B = pInfo->B;
    int *Product = pInfo->Product;

    // int assignedSize = dim*noThread;
    int i, j, k;
    for(i = id; i < dim ; i += noThread){
	for(j = 0 ; j < dim ; j++){
		Product[i*dim + j] = 0;
		for(k = 0; k < dim ; k ++){
		    Product[i*dim + j] += A[i*dim + k]*B[k*dim+j];	    
		    //printf("Product[%d] = %d (A[%d] = %d, B[%d] = %d)\n", i*dim + j, Product[i*dim + j], i*dim + k, A[i*dim + k], k*dim + j, B[k*dim+j]);
    }
	}
    }

    pthread_exit(0);
    // multiply two matrics pInfo->A and pInfo->B put the Product in pInfo->Product
    // i-th thread computes only (i + k * noThread)-th rows
}


void TEST_MESSAGE(char *message){
  fprintf(stderr, "%s\n", message);
}
