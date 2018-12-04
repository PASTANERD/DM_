//
//  main.c
//  twitterUseGraph.c
//
//  Created by 배준현 on 2018. 12. 1..
//  Copyright © 2018년 배준현. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE    1000
#define MAX_NUM_FRIEND 10000
#define TRUE    1
#define FALSE   0


typedef struct _Node{
    int id;
    //int visit
    //int partition
    int influence;
    int visited;
    int partition;
}Node;

typedef struct _Edge{
    Node *X;
    Node *Y;
    //int weight;
}Edge;

typedef struct use{
    void (*enQueue)(Node*);
    Node* (*deQueue)();
    void (*peek)();
}queue;

void enQueue();
Node* deQueue();
void peek();

void Bfs(Node* Bfsfriend_node, Edge* Bfsfriend_edge[]);
void Edge_finder(Node* S, Edge* E, Node* q1);

void following(Node* S, Edge* E[]);

static int front = 0;
static int rear = 0;

static int pair = 0;  // edge의 수 = 3874
static int num_node = 0; // node 수 = 2934
static int partition = 0; // partition = 1~448

queue q1;
Node* node[MAX_SIZE];
Node* nodeMemory[MAX_SIZE];

Node* friend_node[MAX_NUM_FRIEND];
Edge* friend_edge[MAX_NUM_FRIEND];

int main(int argc, char *argv[]){
    FILE* fl = NULL;
    //int count_friend = 0; // twitterFriends.txt의 라인 수 = 7747
    int i = 0;
    int flag1 = 0; // node malloc 개수를 알려줌
    int flag2 = 0;
    
    
    
    fl = fopen("twitterFriends.txt", "rt");
    
    // twitterFriends.txt를 읽어서 중복없이 node를 넣고 edge로 연결하기
    while(!feof(fl)){
        int tempId1, tempId2; // node의 id를 중복없이 받기 위해 사용
        
        fscanf(fl, "%d %d\n", &tempId1, &tempId2);
        
        for(i=0; i<num_node; i++){
            if(friend_node[i]->id == tempId1){
                tempId1 = i;
                flag1++;
            }
            else if(friend_node[i]->id == tempId2){
                tempId2 = i;
                flag2++;
            }
        }
        
        friend_edge[pair] = (Edge*)malloc(sizeof(Edge));
        
        if(flag1 == 1 && flag2 == 1){
            friend_edge[pair]->X = friend_node[tempId1];
            friend_edge[pair]->Y = friend_node[tempId2];
        }
        else if(flag1 == 1){
            friend_node[num_node] = (Node*)malloc(sizeof(Node));
            friend_node[num_node]->id = tempId2;
            
            friend_node[num_node]->visited = FALSE;
            
            friend_edge[pair]->X = friend_node[tempId1];
            friend_edge[pair]->Y = friend_node[num_node];
            num_node++;
        }
        else if(flag2 == 1){
            friend_node[num_node] = (Node*)malloc(sizeof(Node));
            friend_node[num_node]->id = tempId1;
            
            friend_node[num_node]->visited = FALSE;
            
            friend_edge[pair]->X = friend_node[num_node];
            friend_edge[pair]->Y = friend_node[tempId2];
            num_node++;
        }
        else{
            friend_node[num_node] = (Node*)malloc(sizeof(Node));
            friend_node[num_node+1] = (Node*)malloc(sizeof(Node));
            
            friend_node[num_node]->id = tempId1;
            friend_node[num_node+1]->id = tempId2;
            
            friend_node[num_node]->visited = FALSE;
            friend_node[num_node+1]->visited = FALSE;
            
            friend_edge[pair]->X = friend_node[num_node];
            friend_edge[pair]->Y = friend_node[num_node+1];
            
            num_node += 2;
        }
        
        pair++;
        flag1 = 0;
        flag2 = 0;
    }
    
    for(i=0; i<num_node; i++){
        memset(node, 0, sizeof(node));
        memset(nodeMemory, 0, sizeof(node));
        
        if(friend_node[i]->visited == FALSE){
            partition++;
            printf("%d\n", partition);
            Bfs(friend_node[i], friend_edge);
        }
    }
    
    printf("\n%d\n\n", partition);
    
    printf("120번째 partition : \n\n");
    for(i=0;i<num_node;i++){
        if(friend_node[i]->partition == 400){
            printf("%d\n", friend_node[i]->id);
        }
    }
    
    
    fclose(fl);
}

void Bfs(Node* Bfsfriend_node, Edge* Bfsfriend_edge[])
{
    Node* Nodetemp = NULL;
    Node* S = Bfsfriend_node;
    
    q1.enQueue = enQueue;
    q1.deQueue = deQueue;
    q1.peek = peek;
    
    q1.enQueue(S);
    
    Nodetemp = q1.deQueue();
    
    while(Nodetemp != NULL){
        following(Nodetemp, Bfsfriend_edge);

        Nodetemp = q1.deQueue();
    }
}

void following(Node* S, Edge* E[])
{
    int i = 0;
    
    q1.enQueue = enQueue;
    q1.deQueue = deQueue;
    q1.peek = peek;
    
    S->visited = TRUE;
    S->partition = partition;
    
    for(i=0; i<pair; i++){
        if(E[i]->X->id == S->id && E[i]->Y->visited == FALSE){
            q1.enQueue(E[i]->Y);
            E[i]->Y->visited = TRUE;
            E[i]->Y->partition = partition;
        }
        else if(E[i]->Y->id == S->id && E[i]->X->visited == FALSE){
            q1.enQueue(E[i]->X);
            E[i]->X->visited = TRUE;
            E[i]->X->partition = partition;
        }
    }
}

void enQueue(Node* node)
{
    if(front == (rear+1)%MAX_SIZE){
        printf("큐가 가득찼습니다\n");
        return;
    }
    
    
    rear = (rear+1) % MAX_SIZE;
    
    nodeMemory[rear] = node;
    
    printf("큐에 %d가 추가되었습니다\n", nodeMemory[rear]->id);
}

Node* deQueue()
{
    if(front == rear){
        printf("큐가 비었습니다\n");
        return NULL;
    }
    
    front = (front+1) % MAX_SIZE;
    return nodeMemory[front];
}

void peek()
{
    int i = 0;
    int count = 0;
    int t = 0;
    
    if(front == rear){
        printf("큐가 비었습니다\n");
        return;
    }
    
    printf("배열에 저장된 data는 : ");
    
    if(front < rear){
        for(i=front+1; i<=rear; i++){
            count++;
            printf("%d ", nodeMemory[i]->id);
        }
    }
    else if(front > rear){
        
        t = (front+1) % MAX_SIZE;
        if(t == 0){
            for(i=1; i<=rear; i++){
                printf("%d ", nodeMemory[i]->id);
            }
        }
        
        else{
            i = t;
            while(i != rear){
                if(i == 0){
                    i = 1;
                }
                i = i % MAX_SIZE;
                printf("%d ", nodeMemory[i]->id);
                i++;
                count++;
            }
            printf("%d ", nodeMemory[i]->id);
            count++;
        }
    }
    
    printf("\n저장된 데이터는 %d개 입니다\n", count);
}



