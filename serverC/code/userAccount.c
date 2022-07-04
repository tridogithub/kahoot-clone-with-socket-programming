#include <stdio.h>

#include <userAccount.h>

int authenticated = 0;
char *currentUser;
int checkAuthenticated(char *username, char *password){
    printf("\n %s", username);
    printf("\n %s", password);
    FILE *fp = fopen("user-account.txt", "r");
    char buf[1024];
    while (fgets(buf, sizeof(buf), fp) !=NULL) {
        char *token = strtok(buf, " ");
        
        if (strcmp(username, token) == 0)
        {
            token = strtok(NULL, " ");
            token[strlen(token)-1] = '\0';
            printf("\n%d", strcmp(password, token));
            if (strcmp(password, token) == 0)
            {
                printf("\nAuthenticated");
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
        strcpy(currentUser, username);
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
int registryNewUser(User newUser){
    FILE *fp = fopen("user-account.txt", "a");
    fprintf(fp, "%s %s\n", newUser.username, newUser.password);
    fclose(fp);
    return 1; //registry new user
}; // append to file