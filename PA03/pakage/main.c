#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_first();
void finder(int, int, int);

int main(int argc, char *argv[]){

  int option = 0;
  char buffer[100];
  int X;
  int Y;
  while(1){
    print_first();
    fgets(buffer, sizeof(buffer), stdin);
    option = atoi(buffer);
    switch(option){
      case 1:
        printf("Please input user number between 0~5127\n");
        while(1){
          fgets(buffer, sizeof(buffer), stdin);
          X = atoi(buffer);
          if(X < 0 || X > 5127){
            printf("please number between 0 and 5127\n");
          }
          else{
            finder(1, X, 0);
            break;
          }
        }
        continue;
      case 2:
        finder(2, 0, 0);
        continue;
      case 3:
        do{
          printf("Please input start user (0~5127)\n");
          fgets(buffer, sizeof(buffer), stdin);
          X = atoi(buffer);
        }while(X < 0  || X > 5127);
        do{
          printf("Please input destination user (0~5127)\n");
          fgets(buffer, sizeof(buffer), stdin);
          Y = atoi(buffer);
        }while (Y < 0 || Y > 5127);
        finder(3, X, Y);
        continue;
      case 4:
        system("./data/symmetry");
        continue;
      case 5:
        system("./data/surfing");
        continue;
      case 6:
        return 0;
      default:
        printf("Please choose option between 1 and 6\n");
        continue;
    }
    sleep(2);
    memset(buffer, 0, sizeof(buffer));
  }

  return 0;
}

void print_first(){
  printf("\n----------------------------------------------------------\n");
  printf("2018-2 Discrete Mathetics PA03 program...\n");
  printf("Please enter number of options below to see the results.\n");
  printf("1. Find followers\n2. Check longest minimum distacne\n3. Find connectivity between X and Y\n4. Find sets of friend partitions\n5. Random Surfer\n6. Quit\n");
  printf("----------------------------------------------------------\n\n");
}

void finder(int option, int key1, int key2){
  FILE *stream;
  int str_len;
  int temp1, temp2, item;
  int compare = 0;
  int X;
  int Y;
  int i = 0;
  if(option == 1){
    stream = fopen("data/followers.txt", "r");
    while((str_len = fscanf(stream, "%d %d %d\n", &item, &temp1, &temp2)) > 0){
      if(i == key1){
        printf("\n\nuser[%d] follows %d people, and has %d followers\n", item, temp1, temp2);
        break;
      }
      i++;
    }
  }
  if(option == 2){
    stream = fopen("data/total_edge.txt", "r");
    while((str_len = fscanf(stream, "%d %d %d\n", &temp1, &temp2, &item)) > 0 ){
      if(compare <= item){
        compare = item;
        X = temp1;
        Y = temp2;
      }
    }  

    printf("\n\nmost longest minimum distance is \n");
    printf("X: %d Y: %d distance: %d\n", X, Y, compare);
  }
  if(option == 3){
    stream = fopen("data/unique.txt", "r");
    while((str_len = fscanf(stream, "%d\n", &item)) > 0){
      if(i == key1){
        X = item; 
      }
      if(i == key2){
        Y = item;
      }
      i++;
    }
    fclose(stream);
    stream = fopen("data/total_edge.txt", "r");
    while((str_len = fscanf(stream, "%d %d %d\n", &temp1, &temp2, &item)) > 0){
      if(temp1 == X && temp2 == Y){ 
        printf("\n\nuser[%d] and user[%d] are connected.\n", X, Y);
        compare = 1;
        break;
      }
      if(temp1 == Y && temp2 == X){
        printf("\n\nuser[%d] and user[%d] are connected.\n", X, Y);
        compare = 1;
        break;
      }
    }
    if(!compare) printf("\n\nThere is no connection between user[%d] and user[%d]\n", X, Y);
  }
  fclose(stream);
}
