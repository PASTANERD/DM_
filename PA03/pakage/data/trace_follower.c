#include <stdio.h>
#include <stdlib.h>

#define UNIQUE_ID "unique.txt"
#define EDGE_SAMPLE "twitter_sampled.txt"
#define NODE_SIZE 5128
#define EDGE_SIZE 21270

typedef struct _Node{
  int id;
  int following;
  int follower;
}Node;

typedef struct _Edge{
  Node* X;
  Node* Y;
}Edge;


void node_finder(Node*);
void edge_finder(Node*, Edge*);
void calculation(Node*, Edge*, int);
void print_node();

static int node_size;
static int edge_size;
static Node *node;
static Edge *edge;


FILE *stream;

int main(int argc, char *argv[]){
  node_size = NODE_SIZE;
  edge_size = EDGE_SIZE;

  node = (Node*)malloc(sizeof(Node)*node_size);
  edge = (Edge*)malloc(sizeof(Edge)*edge_size);
    node_finder(node);
    edge_finder(node, edge);
    calculation(node, edge, edge_size);
    print_node();
}

void calculation(Node *nodeset, Edge *edgeset, int edge_size){
    for(int i = 0 ; i < edge_size ; i++){
        edgeset[i].X->following++;
        edgeset[i].Y->follower++;
    }
}



void node_finder(Node* nodeset){
    
    int i = 0;
    int str_len;
    stream = fopen(UNIQUE_ID, "r");
    
    while((str_len = fscanf(stream, "%d\n", &nodeset[i].id)) > 0){
        nodeset[i].follower = 0;
        nodeset[i].following = 0;
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

void print_node(){
    stream = fopen("followers.txt", "w");
    for(int i = 0 ; i < node_size ; i++){
        //printf("user[%d] follow: %d follower: %d\n", node[i].id, node[i].following, node[i].follower);
        fprintf(stream, "%d %d %d\n", node[i].id, node[i].following, node[i].follower);
    }
    fclose(stream);
}
