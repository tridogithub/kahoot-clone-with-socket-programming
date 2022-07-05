#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userAccount.h"
#define MAXLINE 4096

int authenticated = 0;
char *currentUser;
int checkAuthenticated(char *username, char *password){
    FILE *fp = fopen("../user-account.txt", "r");
    char buf[1024];
    while (fgets(buf, sizeof(buf), fp) !=NULL) {
        char *token = strtok(buf, " ");
        
        if (strcmp(username, token) == 0)
        {
            token = strtok(NULL, " ");
            token[strlen(token)-1] = '\0';
            // token[strlen(token) - 1] = 13;
            password[strlen(password) - 1] = '\0';
            printf("\n%d", strcmp(password, token));
            if (strcmp(password, token) == 0)
            {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}; // check in file user-account.txt
void login(char *username, char *password, char *result)
{
    if (checkAuthenticated(username, password) == 1)
    {
        printf("\nAuthenticated");
        authenticated = 1;
        // strcpy(currentUser, username);
        strcpy(result, "authenticated");
    }
    else
    {
        printf("\nNot Authenticated");
        authenticated = 0;
        strcpy(result, "unauthenticated");
    }
}
User *getAllUser();
int registryNewUser(User* newUser){
    FILE *fp = fopen("../user-account.txt", "a");
    fprintf(fp, "%s %s", newUser->username, newUser->password);
    fclose(fp);
    return 1; //registry new user
}; // append to file