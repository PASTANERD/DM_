#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define UNIQUE_ID "data/unique.txt"
#define EDGE_SAMPLE "data/twitter_sampled.txt"
#define NODE_SIZE 5128
#define EDGE_SIZE 21270
#define SURF_LIMIT 10000

typedef struct _Node{
  int id;
  int influence;
}Node;

typedef struct _Edge{
  Node *X;
  Node *Y;
}Edge;

void node_finder(Node*);
void edge_finder(Node*, Edge*);
void testprint(Node*, Edge*);
int random_surfer(Node*, Edge*);
Node* random_node(Node*, int);
Node* random_node_queue(Node**, int);
int find_following(Node*, Node**, Edge*); 
void most_inf();

FILE *stream;
static int node_size;
static int edge_size;
static int surftime;
static int ratio;
static int queue_size;
static Node *next_queue[500];
static Node *node;
static Edge *edge;

int main(int argc, char *argv[]){

  node_size = NODE_SIZE;
  edge_size = EDGE_SIZE;

  node = (Node*)malloc(sizeof(Node)*node_size);
  edge = (Edge*)malloc(sizeof(Edge)*edge_size);

  node_finder(node);
  edge_finder(node, edge);
  sleep(1);
  //testprint(node,edge);
  surftime = 1;
  Node* init_node = random_node(node, node_size);
  srand(time(NULL));
  random_surfer(init_node, edge);
  
  printf("\nTOP20 for most influent users are... \n");
  for(int i = 0; i < 20 ; i++){
    most_inf();
  }

  free(node);
  free(edge);
  return 0;
}

int random_surfer(Node *given_node, Edge *edgeset){
    if(surftime > SURF_LIMIT){
      printf("\nsurfing finished by %d times\n", surftime-1);
      printf("------------------------------\n");
      return 0;
    }
    else {
      printf("\nsurfing %d times now...\n", surftime);
      surftime++;
    }
    Node* current_node = given_node;
    current_node->influence++;
    Node* next_node;
    printf("current_node = %d(inf = %d)\n", current_node->id, current_node->influence);
    
    queue_size = find_following(current_node, next_queue, edgeset);
    ratio = rand()%10;
    //TODO 10% 확률 또는 연결된 노드가 없을 경우 새로운 노드에서 시작한다.
    if(ratio == 1 || queue_size == 0){
      if(ratio == 1) printf("start from new one by MAGIC!\n");
      else printf("there is no following\n");
      next_node = random_node(node, node_size);
      printf("start from new user %d\n", next_node->id);
      return random_surfer(next_node, edgeset);
    }
    //TODO 90%확률로 연결된 노드 중 하나를 선택하여 Random Surfer를 시작한다.
    else{
      next_node = random_node_queue(next_queue, queue_size);
      printf("follow to next user %d\n", next_node->id);
      return random_surfer(next_node, edgeset);
    }
}

Node *random_node(Node* nodeset, int set_size){
  int random = rand()%set_size;
  printf("random number = %d\n", random);
  return &nodeset[random];
}

Node *random_node_queue(Node** nodeset, int set_size){
  int random = rand()%set_size;
  //printf("random number = %d\n", random);
  return nodeset[random];
}

int find_following(Node* current_node, Node** next_node, Edge* edgeset){
  int k = 0;
  //printf("next queue : ");
  for(int i = 0 ; i < edge_size ; i++){
    if(edgeset[i].X->id == current_node->id){
      next_node[k] = edgeset[i].Y;
      //printf("%d ", next_node[k]->id);
      k++;
    }
  }
  //printf("\nqueue size = %d\n", k);
  return k;
}

void most_inf(){
  int temp = 0;
  int tag;
  for(int i = 0 ; i < node_size ; i++){
    if(temp < node[i].influence) {
      temp = node[i].influence;
      tag = i;
    }
  }
  printf("user[%d] has %d score.\n", node[tag].id, node[tag].influence);
  node[tag].influence = 0; 
}

void node_finder(Node *nodeset){
  int i = 0;
  int str_len;
  stream = fopen(UNIQUE_ID, "r");
  
  while((str_len = fscanf(stream, "%d\n", &nodeset[i].id)) > 0){ 
    nodeset[i].influence = 0;
    i++;
  }

  fclose(stream);
}

void edge_finder(Node* nodeset, Edge *edgeset){
  int temp_X, temp_Y;
  int i = 0 ;
  int str_len;
  stream = fopen(EDGE_SAMPLE, "r");

  while((str_len = fscanf(stream, "%d %d\n", &temp_X, &temp_Y)) > 0){
    for(int j = 0 ; j < node_size ; j++){
      if(nodeset[j].id == temp_X) edgeset[i].X = &nodeset[j];
      if(nodeset[j].id == temp_Y) edgeset[i].Y = &nodeset[j];
      if(edgeset[i].X != NULL && edgeset[i].Y != NULL) break;
    }
    i++;
  }
  fclose(stream);
}

void testprint(Node *node, Edge *edge){
  printf("test printf for all nodes\n");
  for(int i = 0 ; i < node_size ; i++)
    printf("%d)\tid = %d,\tinf = %d\n", i, node[i].id, node[i].influence);

  sleep(3);

  printf("test printf for all edges\n");
  for(int i = 0; i < edge_size ; i++)
    printf("X = %d\t(inf = %d),\tY = %d\t(inf = %d)\n", edge[i].X->id, edge[i].X->influence, edge[i].Y->id, edge[i].Y->influence);
  
}
