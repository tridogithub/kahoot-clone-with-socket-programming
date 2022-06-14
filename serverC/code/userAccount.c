#include <stdio.h>

#include <userAccount.h>

int authenticated = 0;
char *currentUser;
int checkAuthenticated(char *username, char *password); // check in file user-account.txt
void login(char *username, char *password, char *result)
{
    if (checkAuthenticated(username, password) == 1)
    {
        printf("\nAuthenticated");
        authenticated = 1;
        strcpy(currentUser, username);
        strcpy(result, "Authenticated");
    }
    else
    {
        printf("\nNot Authenticated");
        authenticated = 0;
        strcpy(result, "Not Authenticated");
    }
}
User *getAllUser();
int registryNewUser(User newUser); // append to file