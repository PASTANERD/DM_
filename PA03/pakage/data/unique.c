//to find unique Id list

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(){
 
    FILE * write = fopen("unique_symmetry.txt", "w"); 
    FILE * read = fopen("twitterFriends.txt", "r");
    
    char user1[12];
    char user2[12];

    char Idlist[2934][12];
    int i = 0;
    while(!feof(read))
    {
        
        int checker = 0;

        fscanf(read, "%s %s", user1, user2);

        for(int j = 0; j < i; j++)
        {       
            if(strcmp(Idlist[j], user1)==0) checker++;
        }

        if(checker==0)
        {
            strcpy(Idlist[i], user1);           
            i++;
        }
        checker = 0;

        for(int j = 0; j < i; j++)
        {
            if(strcmp(Idlist[j], user2)==0) checker++;
        }
                
        if(checker==0)
        {   
            strcpy(Idlist[i], user2);
            i++;
        }



    }
    for(i = 0; i < 2934; i++)
    {
        fprintf(write, "%s", Idlist[i]);
        fprintf(write, "\n");
    }   
    
    fclose(write);
    fclose(read);

return 0;
}
