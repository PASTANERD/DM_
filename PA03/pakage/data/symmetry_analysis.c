#include <stdio.h>
#include <stdlib.h>

#define UNIQUE_ID "data/unique_symmetry.txt"
#define EDGE_SAMPLE "data/twitterFriends.txt"
#define LOG_FILE "symmetry_analysis.log"
#define NODE_SIZE 2934
#define EDGE_SIZE 3873
#define MAX_SIZE 10000

typedef struct _Node{
  int id;
  int visit;
  int partition;
  int influence;
}Node;

typedef struct _Edge{
  Node *X;
  Node *Y;
  int weight;
}Edge;

void node_finder(Node*);
void edge_finder(Node*, Edge*);
void graph(Node*, Edge*);
void init(Node*, Edge*);
int find_following(Node*, Edge*);
void enQueue(Node*);
Node *deQueue();
void enQueue_temp(Node*);
Node* deQueue_temp();
void migration(int);
void print_partitions();
void testprint(Node*, Edge*);

static int front = 0;
static int rear = 0;
static int front2 = 0;
static int rear2 = 0;

FILE *stream;
static int node_size;
static int edge_size;
static int count;
static int temp;
static Node *queue[MAX_SIZE];
static Node *queue_temp[MAX_SIZE];
static Node *node;
static Edge *edge;
static int group;

int main(int argc, char *argv[]){

  node_size = NODE_SIZE;
  edge_size = EDGE_SIZE;
  
  node = (Node*)malloc(sizeof(Node)*node_size);
  edge = (Edge*)malloc(sizeof(Edge)*edge_size);

  node_finder(node);
  edge_finder(node, edge);

  group = 0;
  for(int i = 0; i < node_size ; i++){
    if(node[i].visit == 0){
      group++;
      node[i].visit = 1;
      graph(&node[i], edge);
    }
  }
  print_partitions();
  printf("\ntotal partitions = %d\n",group);
  
  testprint(node, edge);
  free(node);
  free(edge);
  return 0;
}


void graph(Node *given_node, Edge* edgeset){
  count = 1;
  temp = 0;
  Node* current_node;
  enQueue(given_node);
  while((current_node = deQueue()) != NULL){
    //current_node = deQueue();
    current_node->partition = group;
    printf("current : user[%d]\n", current_node->id);
   find_following(current_node, edgeset);
    // temp = temp + find_following(current_node, edgeset);
    if(count == 0){
      count = temp;
      temp = 0;
      migration(count);
    }
  }  
}

void migration(int size){
  for(int i = 0; i < size ; i++){
    enQueue(deQueue_temp());    
  }
}


void enQueue_temp(Node* node)
{
    if(front2 == (rear2+1)%MAX_SIZE){
        //printf("큐가 가득찼습니다\n");
        return;
    }
    if(node->visit == 0){
      //printf("node[%d] is enqueued\n", node->id);
      node->visit = 1;
      rear2 = (rear2+1) % MAX_SIZE;
      temp++;
      queue_temp[rear2] = node;
    }
    else{
      return;
    }
    //printf("큐에 %d가 추가되었습니다\n", queue_temp[rear2]->id);
}

Node* deQueue()
{
    if(front == rear){
        //printf("큐가 비었습니다\n");
        return NULL;
    }
    front = (front+1) % MAX_SIZE;
    count--;
    return queue[front];
}

void enQueue(Node * node){
  if(front == (rear+1)%MAX_SIZE){
    return;
  }
  //node->partition = group;
  rear = (rear+1) % MAX_SIZE;
  queue[rear] = node;
}

Node* deQueue_temp(){
  if(front2 == rear2){
    //printf("temporary queue is null\n");
    return NULL;
  }
  front2 = (front2+1)%MAX_SIZE;
  return queue_temp[front2];
}


int find_following(Node* current_node, Edge* edgeset){
  for(int i = 0; i < edge_size ; i++){
    if(edgeset[i].X->id == current_node->id){
      enQueue_temp(edgeset[i].Y);
    }
    if(edgeset[i].Y->id == current_node->id){
      enQueue_temp(edgeset[i].X);
    }
  }
  //printf("total following # is %d\n", k);
  return 0;
}


void node_finder(Node* nodeset){

  int i = 0;
  int str_len;
  stream = fopen(UNIQUE_ID, "r");

  while((str_len = fscanf(stream, "%d\n", &nodeset[i].id)) > 0){
    nodeset[i].visit = 0;
    nodeset[i].partition = 0;
    nodeset[i].influence = 0;
    i++;
  }

  fclose(stream);
}

void edge_finder(Node* nodeset, Edge *edgeset){

  int temp_x, temp_y;
  int i = 0;
  int str_len;
  stream = fopen(EDGE_SAMPLE, "r");
  //fseek(stream, 0, SEEK_SET);
  
  while((str_len = fscanf(stream, "%d %d\n", &temp_x, &temp_y)) > 0){
    for(int j = 0; j < node_size ; j++){
      if(nodeset[j].id == temp_x) edgeset[i].X = &nodeset[j];
      if(nodeset[j].id == temp_y) edgeset[i].Y = &nodeset[j];
      if(edgeset[i].X != NULL && edgeset[i].Y != NULL) break;
    }
    i++;
  }

  fclose(stream);
}

void print_partitions(){
  printf("Here are partitions...\n");
  printf("----------------------\n");
  for(int i = 1 ; i <= group ; i++){
    printf("\npartition [%d] ...\n", i);
    for(int j = 0 ; j < node_size ; j++){
      if(node[j].partition == i) printf("user[%d]\n", node[j].id);
    }
  }
}

void testprint(Node *node, Edge *edge){
  stream = fopen(LOG_FILE, "w");
  //fprintf(stream, "test printf for all nodes\n");
  for(int i = 0; i < node_size ; i++)
    fprintf(stream ,"%d %d\n", node[i].id, node[i].partition);
/*
  fprintf(stream, "test printf for all edges\n");
  for(int i = 0 ; i < edge_size ; i ++){
    fprintf(stream,"X = %d\t(partition = %d),\tY = %d\t(partition = %d)\n", edge[i].X->id, edge[i].X->partition, edge[i].Y->id, edge[i].Y->partition);
  }
*/
  fclose(stream);
}
