#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct listNode {
    struct listNode *next ;
    char id[12] ;
} node_ID;

int main(){

    FILE * read = fopen("twitter.txt", "r") ;

    char user1[12] ;
    char user2[12] ;

    int i = 0;
    int j = 0 ;

    fscanf(read, "%s %s", user1, user2);
    node_ID *head1 = malloc(sizeof(node_ID));
    strcpy(head1->id, user1);
    node_ID *head2 = malloc(sizeof(node_ID));
    strcpy(head2->id, user2);
    head1->next = NULL ;
    head2->next = NULL ;
    node_ID *curr1=head1;
    node_ID *curr2=head2;
    node_ID *last1=head1;
    node_ID *last2=head2;

    while(!feof(read)){
        fscanf(read, "%s %s", user1, user2);
        curr1->next=malloc(sizeof(node_ID));
        curr2->next=malloc(sizeof(node_ID));
        strcpy(curr1->next->id, user1);
        strcpy(curr2->next->id, user2);
        curr1=curr1->next;
        curr2=curr2->next;
        curr1->next=NULL;
        curr2->next=NULL;
    }




//put into curr1
    FILE *writeDoubleFriends = fopen("twitterDoubleFriends.txt", "w") ;
    FILE *writeDoubleFriendsNum = fopen("twitterDoubleFriendsNum.txt", "w") ;

    int x=0;
    rewind(read);

    while(!feof(read)){
        fscanf(read, "%s %s", user1, user2);
        curr1=head1;
        curr2=head2;
        while(curr1){
            if (strcmp(user1, curr2->id)==0){
                if (strcmp(user2, curr1->id)==0){
                    x++;
                    fprintf(writeDoubleFriends, "%s %s\n", curr1->id, curr2->id);
                    fprintf(writeDoubleFriendsNum, "%d. %s %s\n",x, curr1->id, curr2->id);
                    break;
                }
            }
            curr1=curr1->next;
            curr2=curr2->next;
        }
    }

    fclose(writeDoubleFriends) ;
    fclose(writeDoubleFriendsNum);


    //Final Step

    FILE *readDoubleFriends = fopen("twitterDoubleFriends.txt", "r");


    fscanf(readDoubleFriends, "%s %s", user1, user2);
    node_ID *head11 = malloc(sizeof(node_ID));
    strcpy(head11->id, user1);
    node_ID *head22= malloc(sizeof(node_ID));
    strcpy(head22->id, user2);
    head11->next = NULL ;
    head22->next = NULL ;
    node_ID *curr11=head11;
    node_ID *curr22=head22;
    node_ID *last11=head11;
    node_ID *last22=head22;

    int check = 0;

    while(!feof(readDoubleFriends)){
        fscanf(readDoubleFriends, "%s %s", user1, user2);
        check=0;
        curr11=head11;
        curr22=head22;
        while(curr11){
            if (strcmp(curr11->id, user2)==0){
                if (strcmp(curr22->id, user1)==0){
                    check=1;
                    break;
                }
            }
            curr11=curr11->next;
            curr22=curr22->next;
        }
        if (check==0){
            last11->next=malloc(sizeof(node_ID));
            last22->next=malloc(sizeof(node_ID));
            strcpy(last11->next->id, user1);
            strcpy(last22->next->id, user2);
            last11=last11->next;
            last22=last22->next;
            last11->next=NULL;
            last22->next=NULL;
        }
    }

    curr11=head11;
    curr22=head22;

    FILE *writeUniqueFriends=fopen("twitterFriends.txt", "w");



    while(head11){
        fprintf(writeUniqueFriends, "%s %s\n", head11->id, head22->id);

        head11=head11->next;
        head22=head22->next;
    }


    fclose(read) ;
    fclose(readDoubleFriends);
    fclose(writeUniqueFriends);
        printf("Done") ;
}